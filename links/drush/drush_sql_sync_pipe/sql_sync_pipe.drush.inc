<?php

include_once dirname(__FILE__) . '/update.inc';

/**
 * Implements hook_drush_command().
 */
function sql_sync_pipe_drush_command() {
  $items['sql-sync-pipe'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => 'Copy and import source database to target database. Transfers as a gzipped pipe via SSH.',
    'aliases' => array(
      'ssp',
      'pipe',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush sql-sync-pipe @prod @dev' => 'Copy the DB defined in sites/prod to the DB in sites/dev.',
    ),
    'arguments' => array(
      'source' => 'Name of the site-alias to use as a source.',
      'destination' => 'Name of the site-alias to use as the destination.',
    ),
    'options' => array(
      'dump' => 'Save the source database dump to destination. If not a temporary file, the file will remain in gzip compression.',
      'progress' => 'If the Pipe Viewer command is installed, show the progress of the sync. See: http://www.ivarch.com/programs/pv.shtml. Common installs: UNIX - "yum install pv" or "apt-get install pv", MacPorts - "port install pv", HomeBrew - "brew install pv". See URL for a complete list of installation instructions.',
      'temp' => 'If using --dump, this will force the dump file to be a temporary file instead. After sync is complete, this file will be deleted. NOTE: This is kind of counter-productive to this command. You should only use this if you are having errors where the piped commands are causing timeout failures.',
      'include-cache-tables' => 'Include the cache tables. By default, tables that contain "cache" anywhere in the name will only be transferred as a structured table without any data.',
      'ssh-options' => 'A string of extra options that will be passed to the ssh command (e.g. "-p 100")',
      'sanitize' => 'Obscure email addresses and reset passwords in the user table post-sync. Optional.',
      'enable' => 'Enable the specified modules in the target database after the sync operation has completed.',
      'disable' => 'Disable the specified modules in the target database after the sync operation has completed.',
      'permission' => 'Add or remove permissions from a role in the target database after the sync operation has completed. The value of this option must be an array, so it may only be specified in a site alias record or drush configuration file.  See `drush topic docs-example-sync-extension`.',
      'no-cache' => 'Pulls a fresh copy of the source database, otherwise if the dump file is less that 24 hours old, it will be imported. Only available when --dump option is used.',
      'no-drop' => 'By default the destination\'s tables will be dropped before the source database is imported. Optional.',
      'skip-tables-key' => 'A key in the $skip_tables array. @see example.drushrc.php. Optional.',
      'skip-tables-list' => 'A comma-separated list of tables to exclude completely. Optional.',
      'structure-tables-key' => 'A key in the $structure_tables array. @see example.drushrc.php. Optional.',
      'structure-tables-list' => 'A comma-separated list of tables to include for structure, but not data. Optional.',
      'tables-key' => 'A key in the $tables array. Optional.',
      'tables-list' => 'A comma-separated list of tables to transfer. Optional.',
    ),
    'sub-options' => array(
      'sanitize' => array(
        'sanitize-password' => 'The password to assign to all accounts in the sanitization operation, or "no" to keep passwords unchanged.  Default is "password".',
        'sanitize-email' => 'The pattern for test email addresses in the sanitization operation, or "no" to keep email addresses unchanged.  May contain replacement patterns %uid, %mail or %name.  Default is "user+%uid@localhost".',
        'confirm-sanitizations' => 'Prompt yes/no after importing the database, but before running the sanitizations',
      ),
    ),
    'topics' => array('docs-sql-sync-pipe'),
  );
  $items['docs-sql-sync-pipe'] = array(
    'description' => 'README.txt for the sql-sync-pipe command.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/README.txt'),
    'aliases' => array('docs-ssp', 'docs-pipe'),
  );
  return $items;
}

function drush_sql_sync_pipe_init($source = NULL, $destination = NULL) {
  // Automatically check for updates.
  _sql_sync_pipe_update_check();
  // Ignore hook if command was updated or the simulate option was provided.
  if (drush_get_context('DRUSH_SQL_SYNC_PIPE_UPDATED', FALSE) || drush_get_context('DRUSH_SIMULATE', FALSE)) {
    return FALSE;
  }
  drush_command_include('sql-sync');
  drush_include(DRUSH_BASE_PATH . '/commands/sql', 'sql.drush.inc');
  return drush_sql_sync_init($source, $destination);
}

function drush_sql_sync_pipe_validate($source = NULL, $destination = NULL) {
  // Ignore hook if command was updated.
  if (drush_get_context('DRUSH_SQL_SYNC_PIPE_UPDATED', FALSE)) {
    return FALSE;
  }
  // Ignore hook if the simulate option was provided.
  if (drush_get_context('DRUSH_SIMULATE', FALSE)) {
    return drush_set_error('DRUSH_SIMULATE_NOT_SUPPORTED', dt('Simulation for the sql-sync-pipe command is not yet supported.'));
  }
  $source_alias = drush_sitealias_overlay_options(drush_sitealias_get_record($source), 'source-');
  if (empty($source_alias)) {
    return drush_set_error('DRUSH_ALIAS_NOT_FOUND', dt('Error: no alias record could be found for !source', array('!source' => $source)));
  }
  $source_database = drush_sitealias_get_db_spec($source_alias, FALSE, 'source-');
  if (empty($source_database)) {
    return drush_set_error('DRUSH_DATABASE_NOT_FOUND', dt('Error: no database record could be found for !source', array('!source' => $source)));
  }
  $destination_alias = drush_sitealias_overlay_options(drush_sitealias_get_record($destination), 'target-');
  if (empty($destination_alias)) {
    return drush_set_error('DRUSH_ALIAS_NOT_FOUND', dt('Error: no alias record could be found for !destination', array('!destination' => $destination)));
  }
  $destination_database = drush_sitealias_get_db_spec($destination_alias, FALSE, 'target-');
  if (empty($destination_database)) {
    return drush_set_error('DRUSH_DATABASE_NOT_FOUND', dt('Error: no database record could be found for !destination', array('!destination' => $destination)));
  }
}

function drush_sql_sync_pipe($source = NULL, $destination = NULL) {
  $source_alias = drush_sitealias_overlay_options(drush_sitealias_get_record($source), 'source-');
  $source_database = drush_sitealias_get_db_spec($source_alias, FALSE, 'source-');

  $destination_alias = drush_sitealias_overlay_options(drush_sitealias_get_record($destination), 'target-');
  $destination_database = drush_sitealias_get_db_spec($destination_alias, FALSE, 'target-');

  $target_dump = FALSE;
  if (drush_get_option('dump', FALSE)) {
    if (drush_get_option('temp', FALSE)) {
      if ($destination_alias['path-aliases']['%dump']) {
        unset($destination_alias['path-aliases']['%dump']);
      }
    }
    $target_dump = drush_sql_dump_file($destination_alias);
  }

  // If the source is remote, but the database server is hosted elsewhere then
  // that database server allows remote connections. It would be redundant
  // to connect to the source server just to dump the database.
  // However, if the source is remote and the database server is also hosted
  // there, then the dump should be piped through an SSH command as it may not
  // allow remote database connections.
  $source_database['db-host'] = FALSE;
  if (!empty($source_database['remote-host'])) {
    $source_database['db-host'] = $source_database['remote-host'];
    if (drush_sitealias_get_option($source_alias, 'db-allows-remote', FALSE) && $source_database['remote-host'] !== $source_database['host'] && $source_database['host'] !== 'localhost' && $source_database['host'] !== '127.0.0.1' && !isset($source_database['socket'])) {
      $source_database['db-host'] = $source_database['host'];
    }
  }
  $destination_database['db-host'] = FALSE;
  if (!empty($destination_database['remote-host'])) {
    $destination_database['db-host'] = $destination_database['remote-host'];
    if (drush_sitealias_get_option($destination_alias, 'db-allows-remote', FALSE) && $destination_database['remote-host'] !== $destination_database['host'] && $destination_database['host'] !== 'localhost' && $destination_database['host'] !== '127.0.0.1' && !isset($destination_database['socket'])) {
      $destination_database['db-host'] = $destination_database['host'];
    }
  }
  // Build textual representations of the database host.
  $txt_source = ($source_database['db-host'] ? $source_database['db-host'] . '/' : '') . $source_database['database'];
  $txt_destination = ($destination_database['db-host'] ? $destination_database['db-host'] . '/' : '') . $destination_database['database'];

  // Create the drop/create MySQL command.Re-create the local DB table.
  drush_print(dt('You will destroy data in !target and replace with data from !source.', array(
    '!source' => $txt_source,
    '!target' => $txt_destination,
  )));
  // If any sanitization operations are to be done, then get the
  // sanitization messages and print them as part of the confirmation.
  // If --sanitize was specified but there were no sanitize messages,
  // then warn that sanitization operations will be accumulated and
  // processed after the sync completes.
  $messages = _drush_sql_get_post_sync_messages();
  if ($messages) {
    drush_print();
    drush_print($messages);
  }
  elseif (drush_get_option('deferred-sanitization', FALSE) && !drush_get_option('confirm-sanitizations', FALSE)) {
    drush_print();
    drush_print("WARNING: --sanitize was specified, but deferred (e.g. the source site is remote).  The sanitization operations will be determined after the database is copied to the local system and will be run without further confirmation.  Run with --confirm-sanitizations to force confirmation after the sync.");
  }
  // TODO: actually make the backup if desired.
  drush_print();
  drush_print(dt("You might want to make a backup first, using the sql-dump command.\n"));
  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_user_abort();
  }
  
  // Drop tables on destination as long as the 'no-drop' option isn't specified.
  if (!drush_get_option('no-drop', FALSE)) {
    drush_sql_empty_db($destination_database);
  }

  $table_selection = drush_sql_get_table_selection();
  // Add cache tables to the $table_selection['structure'] array so they get
  // created, but without any data.
  if (!drush_get_option('include-cache-tables', FALSE)) {
    $cache_tables = _sql_sync_pipe_get_tables($source_alias, $source_database, '*cache*');
    $table_selection['structure'] = array_merge($cache_tables, $table_selection['structure']);
  }
  
  // Build the dump command.
  $dump_cmd = _sql_sync_pipe_build_dump_command($table_selection, $source_database);
  
  // Build the import command.
  $import_command = _drush_sql_connect($destination_database);
  switch (_drush_sql_get_scheme($destination_database)) {
    case 'mysql':
      $import_command .= ' --compress --silent';
      break;
    case 'pgsql':
      $import_command .= ' -q';
      break;
  }

  $commands = array();
  $remote_gzip = FALSE;
  if (!empty($source_database['remote-host']) && $source_database['db-host'] === $source_database['remote-host']) {
    $remote_gzip = TRUE;
    $commands[] = drush_shell_proc_build($source_alias, $dump_cmd . ' | gzip -c9');
  }
  else {
    $commands[] = $dump_cmd;
  }
  // Dump to a file, gunzip and then import.
  if (drush_get_option('dump', FALSE) && $target_dump) {
    if (!file_exists($target_dump) || drush_get_option('no-cache', FALSE) || (time() - filemtime($target_dump)) > 86400) {
      // Dump
      drush_print(dt("\nSaving dump from !source...", array(
        '!source' => $txt_source,
      )));
      _sql_sync_pipe_pv($commands);
      $commands[] = 'tee > ' . $target_dump . ($remote_gzip ? '.gz' : '');
      $cmd = implode(' | ', $commands);
      if (!empty($destination_database['remote-host']) && $destination_database['db-host'] === $destination_database['remote-host']) {
        $cmd = drush_shell_proc_build($destination_alias, $cmd);
      }
      drush_shell_exec($cmd);
    }
    else {
      drush_print(dt("\nAn existing dump of !source already exists at !target_dump. It is less than 24 hours old and will be used for import instead. Use --no-cache if you wish to pull a fresh copy.", array(
        '!source' => $txt_source,
        '!target_dump' => $target_dump,
      )));
    }

    // Importing
    drush_print(dt("\nImporting dump into !target...", array(
      '!target' => $txt_destination,
    )));
    $commands = array();
    if ($remote_gzip) {
      $commands[] = 'gunzip -c ' . $target_dump . '.gz';
    }
    _sql_sync_pipe_pv($commands, ' -s $(gzip -ql ' . $target_dump . '.gz | awk \'{ print $2; }\') ');
    $commands[] = $import_command;
    $cmd = implode(' | ', $commands);
    // Destination is remote, wrap it a SSH.
    if (!empty($destination_database['remote-host']) && $destination_database['db-host'] === $destination_database['remote-host']) {
      $cmd = drush_shell_proc_build($destination_alias, $cmd);
    }
    drush_shell_exec($cmd);

    // Remove files if necessary.
    $commands = array();
    if (drush_get_option('temp', FALSE) || $remote_gzip) {
      if (file_exists($target_dump)) {
        $commands[] = 'rm -f ' . $target_dump;
      }
    }
    if (drush_get_option('temp', FALSE)) {
      if (file_exists($target_dump . '.gz')) {
        $commands[] = 'rm -f ' . $target_dump . '.gz';
      }
    }
    if (!empty($commands)) {
      $cmd = implode('; ', $commands);
      // Destination is remote, wrap it a SSH.
      if (!empty($destination_database['remote-host'])) {
        $cmd = drush_shell_proc_build($destination_alias, $cmd);
      }
      drush_shell_exec($cmd);
    }
  }
  // This is a single piped import.
  else {
    if ($remote_gzip) {
      $commands[] = 'gunzip -c';
    }
    _sql_sync_pipe_pv($commands);
    $commands[] = $import_command;
    $cmd = implode(' | ', $commands);
    // Destination is remote, wrap it in a SSH proc.
    if (!empty($destination_database['remote-host'])) {
      $cmd = drush_shell_proc_build($destination_alias, $cmd);
    }
    drush_shell_exec($cmd);
  }

  // After the database is imported into the destination, we
  // will check and see if we did not collect sanitization
  // operations at the beginning of the function (i.e. because the source
  // site was remote), and if the destination site is local,
  // then we will call the sanitization hooks now.
  // This presumes an important precondition, that the code
  // files were sync'ed before the database was sync'ed.
  if (drush_get_option('deferred-sanitization', FALSE) && (drush_has_boostrapped(DRUSH_BOOTSTRAP_DRUPAL_SITE) == FALSE)) {
    $bootstrapped = drush_bootstrap_max_to_sitealias($destination_alias);
    if ($bootstrapped) {
      drush_command_invoke_all('drush_sql_sync_sanitize', $destination);
      $messages = _drush_sql_get_post_sync_messages();
      if ($messages) {
        drush_print();
        drush_print($messages);
      }
      if (!drush_confirm(dt('Do you really want to sanitize the current database?'))) {
        return drush_user_abort();
      }
      drush_sql_bootstrap_further();
      drush_include(DRUSH_BASE_PATH . '/commands/sql', 'sync.sql');
      drush_command_invoke_all('drush_sql_sync_sanitize', 'default');
      $options = drush_get_context('post-sync-ops');
      $sanitize_query = '';
      foreach ($options as $id => $data) {
        $sanitize_query .= $data['query'] . " ";
      }
      if (!empty($sanitize_query)) {
        if (!drush_get_context('DRUSH_SIMULATE')) {
          drush_sql_query($sanitize_query);
        }
        else {
          drush_print("Executing: $sanitize_query");
        }
      }
    }
  }
}

function _sql_sync_pipe_pv(array &$commands, $flags = '') {
  static $pv;
  // Initialize PipeViewer command, if installed.
  if (!isset($pv)) {
    $pv = FALSE;
    if (drush_get_option('progress', FALSE)) {
      // Destination is local.
      if (empty($destination_database['remote-host'])) {
        drush_shell_exec('which pv');
        $pv = drush_shell_exec_output();
        $pv = reset($pv);
        if ($pv && is_executable($pv)) {
          $pv .= ' -c';
          // Determine PipeViewr version for specific options.
          drush_shell_exec('pv --version | head -n1 | sed \'s/^\(pv \)\([0-9].[0-9].[0-9]\)\(.*\)$/\2/\'');
          $version = drush_shell_exec_output();
          $version = reset($version);
          if (version_compare($version, '1.4.0', '>=')) {
            $pv .= ' -E';
          }
        }
        else {
          $pv = FALSE;
        }
      }
    }
  }
  if ($pv) {
    $commands[] = $pv . $flags;
  }
}

function _sql_sync_pipe_get_tables($alias, $database, $pattern = NULL) {
  $tables = array();
  $cmd = _sql_sync_pipe_query('show tables;', $database);
  if ($database['db-host'] === $database['remote-host']) {
    $cmd = drush_shell_proc_build($alias, $cmd);
  }
  drush_shell_exec($cmd);
  $output = drush_shell_exec_output();
  if (is_array($output) && !empty($output)) {
    // Remove the first item from the array, it's the table header.
    array_shift($output);
    // Match a specific pattern against table names.
    if (!empty($pattern)) {
      foreach ($output as $table) {
        if (preg_match($pattern, $table)) {
          $tables[] = $table;
        }
      }
    }
    // Include all tables.
    else {
      $tables = $output;
    }
  }
  return $tables;
}

function drush_sql_sync_pipe_post_sql_sync_pipe($source = NULL, $destination = NULL) {
  $modules_to_enable = drush_get_option_list('enable');
  if (!empty($modules_to_enable)) {
    drush_log(dt("Enable !modules", array('!modules' => implode(',', $modules_to_enable))), 'ok');
    drush_invoke_process($destination, 'pm-enable', $modules_to_enable, array('yes' => TRUE));
  }
  $modules_to_disable = drush_get_option_list('disable');
  if (!empty($modules_to_disable)) {
    drush_log(dt("Disable !modules", array('!modules' => implode(',', $modules_to_disable))), 'ok');
    drush_invoke_process($destination, 'pm-disable', $modules_to_disable, array('yes' => TRUE));
  }
  $permissions_table = drush_get_option('permission');
  if (!empty($permissions_table)) {
    foreach ($permissions_table as $role => $actions) {
      $values = drush_invoke_process($destination, 'sql-query', array("select perm from permission, role where role.name='$role' and role.rid = permission.rid;"), array(), array('integrate' => FALSE));
      // Remove the first line and explode
      $sql_output = preg_replace('/^.+\n/', '', trim($values['output']));
      $permissions = explode(', ', $sql_output);
      $original_permissions = $permissions;
      if (array_key_exists('add', $actions)) {
        $permissions_to_add = is_array($actions['add']) ? $actions['add'] : explode(', ', $actions['add']);
        drush_log(dt("Add !permissions to !role", array('!permissions' => implode(', ', $permissions_to_add), '!role' => $role)), 'ok');
        $permissions = array_unique(array_merge($permissions, $permissions_to_add));
      }
      if (array_key_exists('remove', $actions)) {
        $permissions_to_remove = is_array($actions['remove']) ? $actions['remove'] : explode(', ', $actions['remove']);
        drush_log(dt("Remove !permissions from !role", array('!permissions' => implode(', ', $permissions_to_remove), '!role' => $role)), 'ok');
        $permissions = array_diff($permissions, $permissions_to_remove);
      }
      if ($permissions != $original_permissions) {
        $permissions_string = implode(', ', $permissions);
        $values = drush_invoke_process($destination, 'sql-query', array("update permission, role set perm='$permissions_string' where role.name='$role' and role.rid = permission.rid;"), array(), array('integrate' => FALSE));
      }
    }
  }
}

function _sql_sync_pipe_build_dump_command($table_selection, $db_spec = NULL) {
  $skip_tables = $table_selection['skip'];
  $structure_tables = $table_selection['structure'];
  $tables = $table_selection['tables'];

  $ignores = array();
  $skip_tables  = array_merge($structure_tables, $skip_tables);
  $data_only = drush_get_option('data-only');
  // The ordered-dump option is only supported by MySQL for now.
  // @todo add documention once a hook for drush_get_option_help() is available.
  // @see drush_get_option_help() in drush.inc
  $ordered_dump = drush_get_option('ordered-dump');

  if (is_null($db_spec)) {
    $db_spec = _drush_sql_get_db_spec();
  }
  $database = $db_spec['database'];

  switch (_drush_sql_get_scheme($db_spec)) {
    case 'mysqli':
    case 'mysql':
      $exec = 'mysqldump';
      // Don't use --opt as this locks the tables on the server and can impact
      // production use and even possibly slow down the dump.
      $extra = ' -CceKqQ --add-drop-table --add-locks --create-options --no-autocommit --single-transaction' . str_replace('--database=', ' ', _drush_sql_get_credentials($db_spec));
      if (isset($data_only)) {
        $extra .= ' --no-create-info';
      }
      if (isset($ordered_dump)) {
        $extra .= ' --skip-extended-insert --order-by-primary';
      }
      $exec .= $extra;

      if (!empty($tables)) {
        $exec .= ' ' . implode(' ', $tables);
      }
      else {
        // Append the ignore-table options.
        foreach ($skip_tables as $table) {
          $ignores[] = "--ignore-table=$database.$table";
        }
        $exec .= ' ' . implode(' ', $ignores);

        // Run mysqldump again and append output if we need some structure only tables.
        if (!empty($structure_tables)) {
          // A sub-shell must be created so it can be piped.
          $exec = "( mysqldump --no-data $extra " . implode(' ', $structure_tables) . " && $exec; ) ";
        }
      }
      break;
    case 'pgsql':
      $create_db = drush_get_option('create-db');
      $exec = 'pg_dump ';
      // Unlike psql, pg_dump does not take a '--dbname=' before the database name.
      $extra = str_replace('--dbname=', ' ', _drush_sql_get_credentials($db_spec));
      if (isset($data_only)) {
        $extra .= ' --data-only';
      }
      $exec .= $extra;
      $exec .= (!isset($create_db) && !isset($data_only) ? ' --clean' : '');

      if (!empty($tables)) {
        foreach ($tables as $table) {
          $exec .= " --table=$table";
        }
      }
      else {
        foreach ($skip_tables as $table) {
          $ignores[] = "--exclude-table=$table";
        }
        $exec .= ' ' . implode(' ', $ignores);
        // Run pg_dump again and append output if we need some structure only tables.
        if (!empty($structure_tables)) {
          $schemaonlies = array();
          foreach ($structure_tables as $table) {
            $schemaonlies[] = "--table=$table";
          }
          $exec = "( pg_dump --schema-only " . implode(' ', $schemaonlies) . $extra . " && $exec; )";
        }
      }
      break;
    case 'sqlite':
      // Dumping is usually not necessary in SQLite, since all database data
      // is stored in a single file on the filesystem which can be copied just
      // like any other file. But it still has a use in migration purposes and
      // building human-readable diffs and such, so let's do it anyway.
      $exec = _drush_sql_connect($db_spec);
      // SQLite's dump command doesn't support many of the features of its
      // Postgres or MySQL equivalents. We may be able to fake some in the
      // future, but for now, let's just support simple dumps.
      $exec .= ' ".dump"';
      break;
    case 'sqlsrv':
      $exec =  "sqlcmd -U \"" . $db_spec['username'] . "\" -P \"" . $db_spec['password'] . "\" -S \"" . $db_spec['host'] . "\" -Q \"BACKUP DATABASE [" . $db_spec['database'] . "]\"";
      break;
    case 'oracle':
      $create_db = drush_get_option('create-db');
      $exec = 'exp ' . _drush_sql_get_credentials($db_spec);
      if (!empty($tables)) {
        $exec .= ' tables="(' . implode(',', $tables) . ')"';
      }
      else {
        $exec .= ' owner=' . $db_spec['username'];
      }

      break;
  }
  return trim($exec);
}

function _sql_sync_pipe_query($query, $db_spec = NULL) {
  $suffix = '';
  $scheme = _drush_sql_get_scheme($db_spec);
  // Inject table prefixes as needed.
  if (drush_has_boostrapped(DRUSH_BOOTSTRAP_DRUPAL_DATABASE)) {
    // Enable prefix processing which can be dangerous so off by default. See http://drupal.org/node/1219850.
    if (drush_get_option('db-prefix')) {
      if (drush_drupal_major_version() >= 7) {
        $query = Database::getConnection()->prefixTables($query);
      }
      else {
        $query = db_prefix_tables($query);
      }
    }
  }
  // is this an oracle query
  if ($scheme == 'oracle') {
    $query = drush_sql_format_oracle($query);
    $suffix = '.sql';
  }
  // Convert mysql 'show tables;' query into something pgsql understands
  if (($scheme == 'pgsql') && ($query == 'show tables;')) {
    $query = drush_sql_show_tables_pgsql();
  }
  return drush_sql_sync_pipe_build_exec($db_spec, $query);
}

function drush_sql_sync_pipe_build_exec($db_spec, $query) {
  $scheme = _drush_sql_get_scheme($db_spec);
  $exec = '';
  switch ($scheme) {
    case 'mysql':
      $exec = 'mysql';
      $exec .= _drush_sql_get_credentials($db_spec);
      $exec .= ' ' . drush_get_option('extra');
      $exec .= " -e " . drush_escapeshellarg($query);

      break;
    case 'pgsql':
      $exec = 'psql -q ';
      $exec .= _drush_sql_get_credentials($db_spec);
      $exec .= ' ' . (drush_get_option('extra') ? drush_get_option('extra') : "--no-align --field-separator='\t' --pset footer=off");
      $exec .= " -c " . drush_escapeshellarg($query);
      break;
    case 'sqlite':
      $exec = 'sqlite3';
      $exec .= ' ' . drush_get_option('extra');
      $exec .= _drush_sql_get_credentials($db_spec);
      $exec .= " " . drush_escapeshellarg($query);
      break;
    case 'sqlsrv':
      $exec = 'sqlcmd';
      $exec .= ' ' . drush_get_option('extra');
      $exec .= _drush_sql_get_credentials($db_spec);
      $exec .= ' -h -1 -Q ' . drush_escapeshellarg($query);
      break;
    case 'oracle':
      $exec = "echo " . drush_escapeshellarg($query) . " | ";
      $exec .= 'sqlplus';
      $exec .= ' ' . drush_get_option('extra');
      $exec .= _drush_sql_get_credentials($db_spec);
      break;
  }
  return $exec;
}

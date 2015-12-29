<?php

function _sql_sync_pipe_update_check() {
  $cid = 'update_check';

  // Set threshold to check for an update and report usage to drupal.org at
  // least once per daily usage.
  if (($cache = drush_cache_get($cid, 'sql_sync_pipe')) && isset($cache->created)) {
    $threshold = 86400; // 1 day.
    if ((time() - (int)$cache->created) <= $threshold) {
      drush_log(dt("The last update check for sql-sync-pipe was !time. Insufficient time has elapsed to initiate another automated update check.", array(
        '!time' => date('Y-M-d H:m:s', (int)$cache->created),
      )), 'notice');
      return;
    }
  }

  $path = dirname(__FILE__);
  $project = drush_drupal_parse_info_file($path . '/drush_sql_sync_pipe.info');
  if ($project === FALSE) {
    $project = array();
  }
  // The update_status engine needs the path and label for project in the array.
  else if ($project) {
    $project['path'] = $path;
    $project['label'] = 'drush_sql_sync_pipe';
  }

  // Retrieve the update_status engine and determine if there is a new release.
  /** @type \Drush\UpdateService\StatusInfoDrush $update_status */
  $update_status = drush_get_engine('update_status');
  $info = $update_status->getStatus(array(
    'drush_sql_sync_pipe' => array_merge(pm_parse_request('drush_sql_sync_pipe'), $project),
  ), TRUE);
  $info = reset($info);

  // Skip update check if no update status information could be determined.
  if (!$info) {
    drush_log(dt('Unable to determine the status of available update data.'), 'warning');
    drush_cache_set($cid, time(), 'sql_sync_pipe');
    return;
  }

  if (version_compare($info['existing_version'], $info['recommended'], '<')) {
    drush_print(dt("\nNew version of !title is available: !version\nCurrent version installed is: !currentversion.\n", array(
      '!title' => $info['title'],
      '!version' => $info['recommended'] . " (" . date('Y-M-d', $info['releases'][$info['recommended']]['date']) . ")",
      '!currentversion' => $info['existing_version'] . " (" . date('Y-M-d', $info['datestamp']) . ")",
    )));
    if (drush_confirm(dt('Would you like to download the new version of !title?', array('!title' => $info['title'])))) {
      // Download the latest version.
      drush_invoke_process('@self', 'pm-download', array('drush_sql_sync_pipe'), array('destination' => dirname($path)), array('interactive' => TRUE));
      // Clear Drush's cache.
      drush_cache_clear_drush();
      // Get the current command's arguments.
      $args = drush_get_arguments();
      // Remove the first argument, which is just the current command name.
      array_shift($args);
      // Re-dispatch this command with the arguments so it uses the newly
      // downloaded code.
      drush_do_command_redispatch('sql-sync-pipe', $args);
      // Flag this dispatch as updated, so it exits quietly.
      drush_set_context('DRUSH_SQL_SYNC_PIPE_UPDATED', TRUE);
    }
  }
  else {
    drush_log(dt("!title self-update check: version !version is up-to-date.", array(
      '!title' => $info['title'],
      '!version' => $info['existing_version']
    )), 'notice');
  }
  drush_cache_set($cid, $info['recommended'], 'sql_sync_pipe');
}

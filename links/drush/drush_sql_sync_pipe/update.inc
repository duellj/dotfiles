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
  $command_dir = dirname(__FILE__);
  $command_parent_dir = dirname($command_dir);
  // Don't check unless we have a datestamp in drush.info
  $project = parse_ini_file($command_dir . '/drush_sql_sync_pipe.info');
  if (($project === FALSE) || (!array_key_exists('datestamp', $project))) {
    drush_log(dt('Cannot determine the release date for the current installation of the drush_sql_sync_pipe command.'), 'notice');
    return;
  }
  $is_dev = FALSE;
  // Retrieve release information for drush_sql_sync_pipe.
  $releases = _sql_sync_pipe_update_get_releases($project);
  // Check for newer releases based on the datestamp.
  // We add 60 seconds to the .info datestamp because of a drupal.org WTF.
  // @see http://drupal.org/node/1019356
  $datestamp = $project['datestamp'] + 60;
  $update_version = FALSE;
  foreach ($releases['releases'] as $version => $release) {
    // We deliberately skip any dev releases unless the current release is a dev release.
    if ((!array_key_exists('version_extra', $release) || ($release['version_extra'] != 'dev'))) {
      if ($release['date'] > $datestamp) {
        $update_version = $release['version'];
        $datestamp = $release['date'];
        $is_dev = isset($release['version_extra']) && $release['version_extra'] == 'dev';
        if ($is_dev) {
          $update_version .= " (" . date('Y-M-d', $datestamp) . ")";
        }
      }
    }
  }
  if ($update_version) {
    drush_print(dt("\nA newer version of !title is available: !version. The current version installed is !currentversion.\n", array(
      '!title' => $releases['title'],
      '!version' => $update_version . " (" . date('Y-M-d', $datestamp) . ")",
      '!currentversion' => $project['version'] . " (" . date('Y-M-d', $project['datestamp']) . ")",
    )));
    if (drush_confirm(dt('Would you like to download the new version of !title?', array('!title' => $releases['title'])))) {
      // Download the latest version.
      drush_invoke_process($destination, 'pm-download', array('drush_sql_sync_pipe'), array('destination' => $command_parent_dir), array('interactive' => TRUE));
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
      '!title' => $releases['title'],
      '!version' => $project['version']
    )), 'notice');
  }
  drush_cache_set($cid, $update_version, 'sql_sync_pipe');
}

function _sql_sync_pipe_update_get_releases(array $project = array()) {
  drush_include_engine('release_info', 'updatexml');
  $_project = pm_parse_project_version(array('drush_sql_sync_pipe'));
  $project = array_merge(reset($_project), $project);
  $xml = _sql_sync_pipe_update_parse_xml($project);
  if (!$xml) {
    return array();
  }
  return updatexml_get_releases_from_xml($xml, 'drush_sql_sync_pipe');
}

function _sql_sync_pipe_update_parse_xml($project) {
  $status_url = isset($project['status url']) ? $project['status url'] : RELEASE_INFO_DEFAULT_URL;
  $url = $status_url . '/' . $project['name'] . '/' . $project['drupal_version'];
  drush_log('Downloading release history from ' . $url);
  // Append a unique hash as the "site_key" and this command's version so
  // drupal.org can track usage.
  $hash = _sql_sync_pipe_update_hash();
  if (!empty($hash)) {
    $url .= (strpos($url, '?') !== FALSE) ? '&' : '?';
    $url .= 'site_key=';
    $url .= rawurlencode($hash);
    if (!empty($project['version'])) {
      $url .= '&version=';
      $url .= rawurlencode($project['version']);
    }
  }
  // Some hosts have allow_url_fopen disabled.
  if ($path = drush_download_file($url, drush_tempnam($project['name']), drush_get_option('cache-duration-releasexml', 24 * 3600))) {
    $xml = simplexml_load_file($path);
  }
  if (!$xml) {
    // We are not getting here since drupal.org always serves an XML response.
    return drush_set_error('DRUSH_PM_DOWNLOAD_FAILED', dt('Could not download project status information from !url', array('!url' => $url)));
  }
  if ($error = $xml->xpath('/error')) {
    // Don't set an error here since it stops processing during site-upgrade.
    drush_log($error[0], 'warning'); // 'DRUSH_PM_COULD_NOT_LOAD_UPDATE_FILE',
    return FALSE;
  }
  // Unpublished project?
  $project_status = $xml->xpath('/project/project_status');
  if ($project_status[0][0] == 'unpublished') {
    return drush_set_error('DRUSH_PM_PROJECT_UNPUBLISHED', dt("Project !project is unpublished and has no releases available.", array('!project' => $project['name'])), 'warning');
  }
  return $xml;
}

function _sql_sync_pipe_update_hash() {
  // Return hash from cache, if it exists.
  $cid = 'unique_hash';
  if (($cache = drush_cache_get($cid, 'sql_sync_pipe')) && !empty($cache->data)) {
    return $cache->data;
  }
  // If no cached hash exists, generate a new one.
  $hash = sha1(uniqid(mt_rand(), TRUE)) . sha1(uniqid(mt_rand(), TRUE));
  drush_cache_set($cid, $hash, 'sql_sync_pipe');
  return $hash;
}

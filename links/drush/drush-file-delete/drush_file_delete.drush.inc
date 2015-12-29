<?php

/**
 * @file
 * Cleans up unused files in a Drupal install public files directory.
 */

/**
 * Implements hook_drush_command().
 */
function drush_file_delete_drush_command() {
  $items = array();

  $items['delete_files'] = array(
    'description' => 'Delete unused files in public directory',
    'callback' => 'drush_file_delete_callback',
  );
}

/**
 * Callback to delete unused files.
 */
function drush_file_delete_callback() {
  $public_dir = variable_get('file_public_path', conf_path() . '/files');

}

/**
 * List files in a directory.
 */

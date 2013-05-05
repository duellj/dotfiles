<?php
// $Id:

/**
 * @file
 *   Some commands for vim integration
 */

/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing each command.
 */
function drush_vim_drush_command() {
  $items = array();

  $items['vim-hook'] = array(
    'description' => 'List implementations and locations of a given hook',
    'arguments' => array(
      'hook' => 'The name of the hook to explore.'
    ),
    'hidden' => TRUE,
  );

  return $items;
}

/**
 * Command handler. Show hook implementations.
 */
function drush_drush_vim_vim_hook($hook) {
  // Get implementations in the .install files as well.
  include_once './includes/install.inc';
  drupal_load_updates();

  if ($hook_implementations = module_implements($hook)) {
    foreach ($hook_implementations as $implementation) {
      $function_name = $implementation . '_' . $hook;
      if (strpos($function_name, '::') === FALSE) {
        if (!function_exists($function_name)) {
          return drush_set_error(dt('Function not found'));
        }
        $reflect = new ReflectionFunction($function_name);
      }
      else {
        list($class, $method) = explode('::', $function_name);
        if (!method_exists($class, $method)) {
          return drush_set_error(dt('Method not found'));
        }
        $reflect = new ReflectionMethod($class, $method);
      }
      $func_info = array('!file' => $reflect->getFileName(), '!startline' => $reflect->getStartLine(), '!function' => $function_name);
      drush_print(dt("!file:!startline:0:!function", $func_info));
    }
  }
  else {
    drush_log(dt('No implementations.'), 'ok');
  }
}

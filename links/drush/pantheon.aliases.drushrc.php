<?php
/**
 * Pantheon drush alias file, to be placed in your ~/.drush directory or the aliases
 * directory of your local Drush home. Once it's in place, clear drush cache:
 *
 * drush cc drush
 *
 * To see all your available aliases:
 *
 * drush sa
 *
 * See http://helpdesk.getpantheon.com/customer/portal/articles/411388 for details.
 */
$aliases['metro.test'] = array(
  'uri' => 'test-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:25e18fca7d1340e2a2bc99fe182db7ca@dbserver.test.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:11672/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.test.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'test.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
  ),
);
$aliases['metro.live'] = array(
  'uri' => 'live-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:808b2701773c46d8b0b49b0c8385f875@dbserver.live.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:12318/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.live.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'live.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
  ),
);
$aliases['metro.dev'] = array(
  'uri' => 'dev-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:81dcc8f1cb8c4cee90f74123e5f0cf58@dbserver.dev.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:13358/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.dev.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'dev.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
  ),
);
$aliases['metro.ombu'] = array(
  'uri' => 'ombu-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:aac12ca001e549ab94ecb6f8893f962d@dbserver.ombu.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:12185/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.ombu.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'ombu.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
  ),
);

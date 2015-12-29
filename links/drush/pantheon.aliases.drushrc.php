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
$aliases['metro.news'] = array(
  'uri' => 'news-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:2133733499d9404da36bbb25eccf1aac@dbserver.news.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:14830/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.news.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'news.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['metro.14'] = array(
  'uri' => '14-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:d2d099b057ae4f96b8dc22a4fc9050d4@dbserver.14.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:18447/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.14.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => '14.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['metro.test'] = array(
  'uri' => 'test-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:1832e77c220646bfa322edc6e91cea93@dbserver.test.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:15233/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.test.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'test.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['metro.ombu'] = array(
  'uri' => 'ombu-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:e9ab18312b9d4f3ca2f67b456931af19@dbserver.ombu.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:18183/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.ombu.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'ombu.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['metro.dev'] = array(
  'uri' => 'dev-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:4b47f28b5e884cd19140f183c9860fc4@dbserver.dev.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:26743/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.dev.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'dev.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['metro.live'] = array(
  'uri' => 'live-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:cedd0dbee9854adc91776b46763854db@dbserver.live.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:22076/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.live.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => 'live.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['metro.141'] = array(
  'uri' => '141-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:3cb0120c302248a4bb0c5e675c512b4e@dbserver.141.a1231a51-6716-ded7-853b-07bb96cae667.drush.in:18136/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.141.a1231a51-6716-ded7-853b-07bb96cae667.drush.in',
  'remote-user' => '141.a1231a51-6716-ded7-853b-07bb96cae667',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['orcc.test'] = array(
  'uri' => 'test-orcc.pantheon.io',
  'db-url' => 'mysql://pantheon:13ec7ad0a7154b9c8ef702a671ee9907@dbserver.test.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in:10230/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.test.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in',
  'remote-user' => 'test.479deb38-37d3-4dcc-b180-99f9c5be193f',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['orcc.live'] = array(
  'uri' => 'www.epoint.oregoncc.org',
  'db-url' => 'mysql://pantheon:0fd939fb23824cfcad152b2425290234@dbserver.live.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in:14039/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.live.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in',
  'remote-user' => 'live.479deb38-37d3-4dcc-b180-99f9c5be193f',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['orcc.tracker'] = array(
  'uri' => 'tracker-orcc.pantheon.io',
  'db-url' => 'mysql://pantheon:09be6e7490ae443a8358e591f009877c@dbserver.tracker.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in:11823/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.tracker.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in',
  'remote-user' => 'tracker.479deb38-37d3-4dcc-b180-99f9c5be193f',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['orcc.dev'] = array(
  'uri' => 'dev-orcc.pantheon.io',
  'db-url' => 'mysql://pantheon:08c4f1fbf03c49b1b8588ce845342340@dbserver.dev.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in:26299/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.dev.479deb38-37d3-4dcc-b180-99f9c5be193f.drush.in',
  'remote-user' => 'dev.479deb38-37d3-4dcc-b180-99f9c5be193f',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['expo.ombu'] = array(
  'uri' => 'ombu-expo.gotpantheon.com',
  'db-url' => 'mysql://pantheon:2cdcfce3b99c48049585147784d86f5b@dbserver.ombu.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in:16734/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.ombu.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in',
  'remote-user' => 'ombu.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['expo.test'] = array(
  'uri' => 'test-expo.gotpantheon.com',
  'db-url' => 'mysql://pantheon:5d2872b5ae194fcb924a57c8a1050a78@dbserver.test.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in:10011/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.test.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in',
  'remote-user' => 'test.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['expo.dev'] = array(
  'uri' => 'dev-expo.gotpantheon.com',
  'db-url' => 'mysql://pantheon:2e15fdf483e74137b6c1db35a1f6c3b2@dbserver.dev.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in:13246/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.dev.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in',
  'remote-user' => 'dev.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['expo.live'] = array(
  'uri' => 'expocenter.org',
  'db-url' => 'mysql://pantheon:7b744dac896a44fe99849be21f42c9af@dbserver.live.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in:14164/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.live.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3.drush.in',
  'remote-user' => 'live.d5c78cde-a97b-4a97-a5ef-3059f2c3dcd3',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['ombu-metro.dev'] = array(
  'uri' => 'dev-ombu-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:560f920420774aab92bbf3617deb4ba4@dbserver.dev.62eb5add-973c-25bf-2622-912a69bb204d.drush.in:14614/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.dev.62eb5add-973c-25bf-2622-912a69bb204d.drush.in',
  'remote-user' => 'dev.62eb5add-973c-25bf-2622-912a69bb204d',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['ombu-metro.test'] = array(
  'uri' => 'test-ombu-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:fc72658c520f43a79cf0ee4f35b9c4b1@dbserver.test.62eb5add-973c-25bf-2622-912a69bb204d.drush.in:12448/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.test.62eb5add-973c-25bf-2622-912a69bb204d.drush.in',
  'remote-user' => 'test.62eb5add-973c-25bf-2622-912a69bb204d',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);
$aliases['ombu-metro.live'] = array(
  'uri' => 'live-ombu-metro.gotpantheon.com',
  'db-url' => 'mysql://pantheon:ab3c813ab76245fb9d03f7971799752f@dbserver.live.62eb5add-973c-25bf-2622-912a69bb204d.drush.in:12176/pantheon',
  'db-allows-remote' => TRUE,
  'remote-host' => 'appserver.live.62eb5add-973c-25bf-2622-912a69bb204d.drush.in',
  'remote-user' => 'live.62eb5add-973c-25bf-2622-912a69bb204d',
  'ssh-options' => '-p 2222 -o "AddressFamily inet"',
  'path-aliases' => array(
    '%files' => 'code/sites/default/files',
    '%drush-script' => 'drush',
  ),
);

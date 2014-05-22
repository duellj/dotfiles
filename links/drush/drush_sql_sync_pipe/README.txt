drush sql-sync-pipe (ssp, pipe)
=============
Drush command for copying and importing source database to target database.
Transfers via pipes whenever possible.


Standard `sql-sync` requires that you save the database dump on the server,
transfer it via rsync and then import it into your destination's database. This
normally would probably be fine if you have a small site. But lets say that
you have a database that's huge, 1GB+ huge. This is a very inefficient method
of transportation. Not only do I have to wait for the source to dump and
transfer just the tiny bit that was changed, but I then have to wait again for
the database to then import on the destination. Also, it dumps all the cache
tables too, we normally don't need those. If you do, you can easily pass the
`--include-cache-tables` option. This command also allows alias files to
specify the option of `db-allows-remote` so it connects directly to the
database server instead of first trying to log into the source server via SSH.
Finally something that will work with Pantheon with out having to install
backend commands in the repo :)

## Benchmarks
Below are examples of syncing the same 1.05Gib database using the two different
methods. Keep in mind that these "benchmarks" are relative to the connection
speed of both the source (server) and destination (local machine).

#### drush sql-sync
The standard `sql-sync` command shipped with drush.

NOTE: This benchmark simply measures the time elapsed from the start of the
command to the end. There is no real way to measure the individual dump,
transfer and import steps as they are done silently. In theory you _could_
watch the debug `-d` output, if you wanted to... I didn't.

```
Command:            drush sql-sync @alias.dev @alias.sandbox --no-cache
Transfer size:      88.1MiB (compressed using rsync)
Import size:        1.05GiB
Total time elapsed: 46 minutes 47 seconds
```

#### drush sql-sync-pipe
This command pipes the entire syncing process. `mysqldump` is passed the
following options to make the quickest and smallest dump as possible:
`-CceKq --single-transaction`. It then pipes the dump via `gzip`, passes
through the SSH tunnel, `gunzip` the pipe and then imports directly into the
destination's database.

```
Command:                 drush sql-sync-pipe @alias.dev @alias.sandbox --progress
Transfer size:           88.1MiB (sent compressed using gzip)
Import size:             1.05GiB
Import & transfer time:  27 minutes 05 seconds
Total time elapsed:      30 minutes 35 seconds
```

#### drush sql-sync-pipe --dump
If, for whatever reason, you need or want to save the dump to the destination's
HDD, specify the `--dump` option. It is similar to using sql-sync, however
there are a few distinct differences. It will still pass the same options to
`mysqldump` as above. Where it differs, is that it will `gzip` the dump and
then pass it via the ssh tunnel so it saves to the destination's HDD (local
machine) instead of the source's (server). After the dump has been saved, it
will decompress the dump using `gunzip` and then pipe it to mysql for import.

NOTE: If the destination alias has a specified dump file, that will be used and
remain on the destination's HDD. If the destination does not have a dump file
specified or the `--temp` option is used, a temporary file will be created
and then removed afterwards.

```
Command:            drush sql-sync-pipe @alias.dev @alias.sandbox --progress --dump --temp
Transfer Size:      88.1MiB (compressed using gzip)
Transfer Time:      5 minutes 14 seconds (includes time taken to gunzip)
Import Size:        1.05GiB
Import Time:        29 minutes 23 seconds
Total Time Elapsed: 37 minutes 36 seconds
```

## drush sql-sync-pipe --progress
If you have [Pipe Viewer](http://www.ivarch.com/programs/pv.shtml) installed,
you can pass the `--progress` option to show syncing progress between the
source and destination. Due to how the default functionality of this command
pipes the entire syncing process, an ETA will not be known and only generic
information will be shown (such as data process and time elapsed). If you pass
the `--dump` option, the initial dump transfer ETA will not be known, but the
import process ETA will be shown.

Below are some common commands if you wish to install the
[Pipe Viewer](http://www.ivarch.com/programs/pv.shtml) command (see link for
other options):
* For UNIX: `yum install pv` or `apt-get install pv`
* For Mac: `port install pv` or `brew install pv`

## drush --help sql-sync-pipe
```
Copy and import source database to target database. Transfers as a gzipped pipe via SSH.

Examples:
 drush sql-sync-pipe @prod @dev            Copy the DB defined in sites/prod to the DB in sites/dev.


Arguments:
 source                                    Name of the site-alias to use as a source.
 destination                               Name of the site-alias to use as the destination.


Options:
 --disable                                 Disable the specified modules in the target database after the sync operation has completed.
 --dump                                    Save the source database dump to destination. If not a temporary file, the file will remain in gzip compression.
 --enable                                  Enable the specified modules in the target database after the sync operation has completed.
 --include-cache-tables                    Include the cache tables. By default, tables that contain "cache" anywhere in the name will only be transferred
                                           as a structured table without any data.
 --permission                              Add or remove permissions from a role in the target database after the sync operation has completed. The value of
                                           this option must be an array, so it may only be specified in a site alias record or drush configuration file.
                                           See `drush topic docs-example-sync-extension`.
 --progress                                If the Pipe Viewer command is installed, show the progress of the sync. See:
                                           http://www.ivarch.com/programs/pv.shtml. Common installs: UNIX - "yum install pv" or "apt-get install pv",
                                           MacPorts - "port install pv", HomeBrew - "brew install pv". See URL for a complete list of installation
                                           instructions.
 --sanitize                                Obscure email addresses and reset passwords in the user table post-sync. Optional.
   --sanitize-password                     The password to assign to all accounts in the sanitization operation, or "no" to keep passwords unchanged.
                                           Default is "password".
   --sanitize-email                        The pattern for test email addresses in the sanitization operation, or "no" to keep email addresses unchanged.
                                           May contain replacement patterns %uid, %mail or %name.  Default is "user+%uid@localhost".
   --confirm-sanitizations                 Prompt yes/no after importing the database, but before running the sanitizations
 --ssh-options                             A string of extra options that will be passed to the ssh command (e.g. "-p 100")
 --temp                                    If using --dump, this will force the dump file to be a temporary file instead. After sync is complete, this file
                                           will be deleted. NOTE: This is kind of counter-productive to this command. You should only use this if you are
                                           having errors where the piped commands are causing timeout failures.


Topics:
 docs-sql-sync-pipe                        README.txt for the sql-sync-pipe command.


Aliases: ssp, pipe
```

#/bin/sh
#
# Install Website Development Stack (nginx php mysql)

info "installing php"
brew tap josegonzalez/php
brew install php53 --with-fpm --without-apache --with-mysql --with-pgsql
brew install php53-xdebug php53-xhprof
ln -sfv /usr/local/opt/php53/*.plist ~/Library/LaunchAgents
launchctl load ~/Library/LaunchAgents/homebrew.mxcl.php53.plist

/usr/local/bin/pear install PHP_CodeSniffer
ln -sv $DOTFILES_DIR/links/drush/coder/coder_sniffer/Drupal $(/usr/local/bin/pear config-get php_dir)/PHP/CodeSniffer/Standards/
brew unlink php53 && php link php53

info "installing nginx"
brew install nginx --with-debug
ln -sfv /usr/local/opt/nginx/*.plist ~/Library/LaunchAgents
launchctl load ~/Library/LaunchAgents/homebrew.mxcl.nginx.plist

brew install dnsmasq
ln -sfv /usr/local/opt/dnsmasq/*.plist ~/Library/LaunchAgents
launchctl load ~/Library/LaunchAgents/homebrew.mxcl.dnsmasq.plist

info "installing mysql"
brew install mysql
tmptmp=$TMPDIR
unset TMPDIR
mysql_install_db --verbose --user=`whoami` --basedir="$(brew --prefix mysql)" --datadir=/usr/local/var/mysql --tmpdir=/tmp
mysqladmin -u root password meh
ln -sfv /usr/local/opt/mysql/*.plist ~/Library/LaunchAgents
launchctl load ~/Library/LaunchAgents/homebrew.mxcl.mysql.plist

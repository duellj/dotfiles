#/bin/sh
#
# Install Website Development Stack (nginx php mysql)

info "installing php"
echo ""
mkdir -p $HOME/Library/LaunchAgents
brew tap josegonzalez/php
brew install php53 --with-fpm --without-apache --with-mysql --with-pgsql
brew install php53-xdebug php53-xhprof
ln -sfv /usr/local/opt/php53/*.plist $HOME/Library/LaunchAgents
launchctl load $HOME/Library/LaunchAgents/homebrew.mxcl.php53.plist

/usr/local/bin/pear install PHP_CodeSniffer 
ln -sv $DOTFILES_DIR/links/drush/coder/coder_sniffer/Drupal $(/usr/local/bin/pear config-get php_dir)/PHP/CodeSniffer/Standards/
/usr/local/bin/pear channel-discover pear.phpmd.org
/usr/local/bin/pear channel-discover pear.pdepend.org
/usr/local/bin/pear install --alldeps phpmd/PHP_PMD
brew unlink php53 && brew link php53

info "installing nginx"
brew install nginx --with-debug
ln -sfv /usr/local/opt/nginx/*.plist $HOME/Library/LaunchAgents
launchctl load $HOME/Library/LaunchAgents/homebrew.mxcl.nginx.plist

brew install dnsmasq
sudo cp -fv /usr/local/opt/dnsmasq/*.plist /Library/LaunchDaemons
sudo launchctl load /Library/LaunchDaemons/homebrew.mxcl.dnsmasq.plist

info "installing mysql"
brew install mysql
ln -sfv /usr/local/opt/mysql/*.plist $HOME/Library/LaunchAgents
launchctl load $HOME/Library/LaunchAgents/homebrew.mxcl.mysql.plist
unset TMPDIR
/usr/local/bin/mysql_install_db --verbose --user=`whoami` --basedir="$(brew --prefix mysql)" --datadir=/usr/local/var/mysql --tmpdir=/tmp
/usr/local/bin/mysqladmin -u root password meh

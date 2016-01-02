#/bin/sh
#
# Install Website Development Stack (nginx php mysql)

info "installing php"
echo ""
mkdir -p $HOME/Library/LaunchAgents
brew install php55 --with-fpm --without-apache --with-mysql --with-pgsql
brew install php55-xdebug php55-xhprof
ln -sfv /usr/local/opt/php55/*.plist $HOME/Library/LaunchAgents
launchctl load $HOME/Library/LaunchAgents/homebrew.mxcl.php55.plist

/usr/local/bin/pear install PHP_CodeSniffer 
ln -sv $DOTFILES_DIR/links/drush/coder/coder_sniffer/Drupal $(/usr/local/bin/pear config-get php_dir)/PHP/CodeSniffer/Standards/
/usr/local/bin/pear channel-discover pear.phpmd.org
/usr/local/bin/pear channel-discover pear.pdepend.org
/usr/local/bin/pear install --alldeps phpmd/PHP_PMD
brew unlink php55 && brew link php55

info "installing Apache"
sudo launchctl unload /System/Library/LaunchDaemons/org.apache.httpd.plist 2>/dev/null
brew tap homebrew/dupes
brew install -v homebrew/apache/httpd24 --with-brewed-openssl --with-mpm-event
brew install -v homebrew/apache/mod_fastcgi --with-brewed-httpd24

sed -i '' '/fastcgi_module/d' $(brew --prefix)/etc/apache2/2.4/httpd.conf
sed -i '' 's/^#\(LoadModule ssl_module libexec/mod_ssl.so\)/\1/' $(brew --prefix)/etc/apache2/2.4/httpd.conf
sed -i '' 's/^#\(LoadModule vhost_alias_module libexec/mod_vhost_alias.so\)/\1/' $(brew --prefix)/etc/apache2/2.4/httpd.conf

(export USERHOME=$(dscl . -read /Users/`whoami` NFSHomeDirectory | awk -F"\: " '{print $2}') ; export MODFASTCGIPREFIX=$(brew --prefix mod_fastcgi) ; cat >> $(brew --prefix)/etc/apache2/2.4/httpd.conf <<EOF

# Load PHP-FPM via mod_fastcgi
LoadModule fastcgi_module    ${MODFASTCGIPREFIX}/libexec/mod_fastcgi.so

<IfModule fastcgi_module>
  FastCgiConfig -maxClassProcesses 1 -idle-timeout 1500

  # Prevent accessing FastCGI alias paths directly
  <LocationMatch "^/fastcgi">
    <IfModule mod_authz_core.c>
      Require env REDIRECT_STATUS
    </IfModule>
  </LocationMatch>

  FastCgiExternalServer /php-fpm -host 127.0.0.1:9000 -pass-header Authorization -idle-timeout 1500
  ScriptAlias /fastcgiphp /php-fpm
  Action php-fastcgi /fastcgiphp

  # Send PHP extensions to PHP-FPM
  AddHandler php-fastcgi .php

  # PHP options
  AddType text/html .php
  AddType application/x-httpd-php .php
  DirectoryIndex index.php index.html
</IfModule>

# Include our VirtualHosts
Include ${USERHOME}/Sites/apache/httpd-vhosts.conf
EOF
)

mkdir -pv ~/Sites/apache/{logs,ssl}
touch ~/Sites/apache/httpd-vhosts.conf
(export USERHOME=$(dscl . -read /Users/`whoami` NFSHomeDirectory | awk -F"\: " '{print $2}') ; cat > ~/Sites/apache/httpd-vhosts.conf <<EOF
#
# Listening ports.
#
#Listen 8080  # defined in main httpd.conf
Listen 8443

#
# Use name-based virtual hosting.
#
NameVirtualHost *:8080
NameVirtualHost *:8443

#
# Set up permissions for VirtualHosts in ~/Sites
#
<Directory "${USERHOME}/Sites">
    Options Indexes FollowSymLinks MultiViews
    AllowOverride All
    <IfModule mod_authz_core.c>
        Require all granted
    </IfModule>
</Directory>

# For http://localhost in the users' Sites folder
<VirtualHost _default_:8080>
    ServerName localhost
    DocumentRoot "${USERHOME}/Sites"
</VirtualHost>
<VirtualHost _default_:8443>
    ServerName localhost
    Include "${USERHOME}/Sites/apache/ssl/ssl-shared-cert.inc"
    DocumentRoot "${USERHOME}/Sites"
</VirtualHost>

#
# VirtualHosts
#

## Manual VirtualHost template for HTTP and HTTPS
#<VirtualHost *:8080>
#  ServerName project.dev
#  CustomLog "${USERHOME}/Sites/apache/logs/project.dev-access_log" combined
#  ErrorLog "${USERHOME}/Sites/apache/logs/project.dev-error_log"
#  DocumentRoot "${USERHOME}/Sites/project.dev"
#</VirtualHost>
#<VirtualHost *:8443>
#  ServerName project.dev
#  Include "${USERHOME}/Sites/apache/ssl/ssl-shared-cert.inc"
#  CustomLog "${USERHOME}/Sites/apache/logs/project.dev-access_log" combined
#  ErrorLog "${USERHOME}/Sites/apache/logs/project.dev-error_log"
#  DocumentRoot "${USERHOME}/Sites/project.dev"
#</VirtualHost>

#
# Automatic VirtualHosts
#
# A directory at ${USERHOME}/Sites/webroot can be accessed at http://webroot.dev
# In Drupal, uncomment the line with: RewriteBase /
#

# This log format will display the per-virtual-host as the first field followed by a typical log line
LogFormat "%V %h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\"" combinedmassvhost

# Auto-VirtualHosts with .dev
<VirtualHost *:8080>
  ServerName dev
  ServerAlias *.dev

  CustomLog "${USERHOME}/Sites/apache/logs/dev-access_log" combinedmassvhost
  ErrorLog "${USERHOME}/Sites/apache/logs/dev-error_log"

  VirtualDocumentRoot ${USERHOME}/Sites/%-2+
</VirtualHost>
<VirtualHost *:8443>
  ServerName dev
  ServerAlias *.dev
  Include "${USERHOME}/Sites/apache/ssl/ssl-shared-cert.inc"

  CustomLog "${USERHOME}/Sites/apache/logs/dev-access_log" combinedmassvhost
  ErrorLog "${USERHOME}/Sites/apache/logs/dev-error_log"

  VirtualDocumentRoot ${USERHOME}/Sites/%-2+
</VirtualHost>
EOF
)
(export USERHOME=$(dscl . -read /Users/`whoami` NFSHomeDirectory | awk -F"\: " '{print $2}') ; cat > ~/Sites/apache/ssl/ssl-shared-cert.inc <<EOF
SSLEngine On
SSLProtocol all -SSLv2 -SSLv3
SSLCipherSuite ALL:!ADH:!EXPORT:!SSLv2:RC4+RSA:+HIGH:+MEDIUM:+LOW
SSLCertificateFile "${USERHOME}/Sites/apache/ssl/selfsigned.crt"
SSLCertificateKeyFile "${USERHOME}/Sites/apache/ssl/private.key"
EOF
)
openssl req \
  -new \
  -newkey rsa:2048 \
  -days 3650 \
  -nodes \
  -x509 \
  -subj "/C=US/ST=State/L=City/O=Organization/OU=$(whoami)/CN=*.dev" \
  -keyout ~/Sites/apache/ssl/private.key \
  -out ~/Sites/apache/ssl/selfsigned.crt

sudo bash -c 'export TAB=$'"'"'\t'"'"'
cat > /Library/LaunchDaemons/co.echo.httpdfwd.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
${TAB}<key>Label</key>
${TAB}<string>co.echo.httpdfwd</string>
${TAB}<key>ProgramArguments</key>
${TAB}<array>
${TAB}${TAB}<string>sh</string>
${TAB}${TAB}<string>-c</string>
${TAB}${TAB}<string>echo "rdr pass proto tcp from any to any port {80,8080} -> 127.0.0.1 port 8080" | pfctl -a "com.apple/260.HttpFwdFirewall" -Ef - &amp;&amp; echo "rdr pass proto tcp from any to any port {443,8443} -> 127.0.0.1 port 8443" | pfctl -a "com.apple/261.HttpFwdFirewall" -Ef - &amp;&amp; sysctl -w net.inet.ip.forwarding=1</string>
${TAB}</array>
${TAB}<key>RunAtLoad</key>
${TAB}<true/>
${TAB}<key>UserName</key>
${TAB}<string>root</string>
</dict>
</plist>
EOF'
sudo launchctl load -Fw /Library/LaunchDaemons/co.echo.httpdfwd.plist



brew install dnsmasq
echo 'address=/.dev/127.0.0.1' > $(brew --prefix)/etc/dnsmasq.conf
echo 'listen-address=127.0.0.1' >> $(brew --prefix)/etc/dnsmasq.conf
echo 'port=35353' >> $(brew --prefix)/etc/dnsmasq.conf
sudo cp -fv /usr/local/opt/dnsmasq/*.plist /Library/LaunchDaemons
sudo launchctl load /Library/LaunchDaemons/homebrew.mxcl.dnsmasq.plist
brew services start dnsmasq
sudo mkdir -v /etc/resolver 
sudo bash -c 'echo "nameserver 127.0.0.1" > /etc/resolver/dev'
sudo bash -c 'echo "port 35353" >> /etc/resolver/dev'

info "installing mysql"
brew install mysql
ln -sfv /usr/local/opt/mysql/*.plist $HOME/Library/LaunchAgents
launchctl load $HOME/Library/LaunchAgents/homebrew.mxcl.mysql.plist
unset TMPDIR
/usr/local/bin/mysql_install_db --verbose --user=`whoami` --basedir="$(brew --prefix mysql)" --datadir=/usr/local/var/mysql --tmpdir=/tmp
/usr/local/bin/mysqladmin -u root password meh

#/bin/sh
#
# Install ZSH

info "installing ZSH"
brew install zsh

# Link prezto files
for source in $HOME/.zprezto/runcoms/*
do
  dest="$HOME/.`basename $source`"
  confirm_link $source $dest
done

# Fix MacOSX path problem.
sudo chmod ugo-x /usr/libexec/path_helper

# Add zsh to available shells.
info "adding zsh to available shells (sudo)"
sudo echo "\n/usr/local/bin/zsh" >> /etc/shells

# Make zsh default shell
chsh -s /usr/local/bin/zsh

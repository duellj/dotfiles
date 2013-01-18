#!/bin/sh
#
# Install Homebrew and packages.

if [[ ! "$(type -P brew)" ]]; then
  info "installing Homebrew"
  true | /usr/bin/ruby -e "$(/usr/bin/curl -fsSL https://raw.github.com/mxcl/homebrew/master/Library/Contributions/install_homebrew.rb)"
fi

if [[ "$(type -P brew)" ]]; then
  info "updating Homebrew"
  brew update
fi

##
# Recipes
##
recipes="git hub mercurial tmux reattach-to-user-namespace tree the_silver_searcher python sqlite drush fasd"
brew install $recipes

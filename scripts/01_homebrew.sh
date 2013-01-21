#!/bin/sh
#
# Install Homebrew and packages.

if [[ ! "$(type -P brew)" ]]; then
  info "installing Homebrew"
  echo ""
  true | ruby -e "$(curl -fsSkL raw.github.com/mxcl/homebrew/go)"
fi

if [[ "$(type -P brew)" ]]; then
  info "updating Homebrew"
  echo ""
  brew update
fi

##
# Recipes
##
recipes="git hub mercurial tmux reattach-to-user-namespace tree the_silver_searcher python sqlite drush fasd"
brew install $recipes

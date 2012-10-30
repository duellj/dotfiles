# Path to oh-my-zsh configuration.
export ZSH=$HOME/.dotfiles/oh-my-zsh

# Set to the name theme to load.
export ZSH_THEME="duellj-powerline"

# Comment this out to disable weekly auto-update checks
export DISABLE_AUTO_UPDATE="true"

# Enable zsh plugins
plugins=(vi-mode brew tmuxinator)

source $ZSH/oh-my-zsh.sh

# PATHS
export PATH=/usr/local/sbin:/usr/local/bin:$PATH
export PATH=/usr/local/share/python:"$(brew --prefix python)/bin":$PATH
export PATH="$(brew --prefix php54)/bin:$PATH)"
export PATH=~/bin:$PATH
export NODEPATH=/usr/local/lib/node
export PYTHONPATH=/usr/local/lib/python2.7/site-packages:$PYTHONPATH

# PostgreSQL
export PGDATA=/Library/PostgreSQL/8.4/data

# Tmuxinator
[[ -s $HOME/.tmuxinator/scripts/tmuxinator ]] && source $HOME/.tmuxinator/scripts/tmuxinator

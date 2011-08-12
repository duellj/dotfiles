# Path to your oh-my-zsh configuration.
export ZSH=$HOME/.oh-my-zsh

# Set to the name theme to load.
# Look in ~/.oh-my-zsh/themes/
export ZSH_THEME="duellj"

# Set to this to use case-sensitive completion
# export CASE_SENSITIVE="true"

# Comment this out to disable weekly auto-update checks
# export DISABLE_AUTO_UPDATE="true"

# Uncomment following line if you want to disable colors in ls
# export DISABLE_LS_COLORS="true"

# Enable zsh plugins
plugins=(git osx vi-mode github autojump brew)

source $ZSH/oh-my-zsh.sh

# Customize to your needs...
export PATH=$PATH:/Library/PostgreSQL/8.4/bin:/usr/local/Cellar/python/2.7.1/bin
export NODEPATH=/usr/local/lib/node

# Always default to mvim
alias vim='mvim'
alias vi='vim'
alias vimdiff='mvim -d'

# Imports from .profile
export EDITOR='mvim -f'
export SVN_EDITOR="vim --noplugin"

# Use vi bindings on the command line
set -o vi

# Nice man page output default is 'more -s!'
# export PAGER=vimpager
# alias less=$PAGER

# now some handy stuff
alias l='ls'
alias la='ls -a'
alias ll='ls -lh'
alias lll='ll|less'
alias llc='echo Total number of files `ll | wc -l` in `pwd`'

# get confirmation on these commands
alias cp='cp -i'
alias rm='rm -i'
alias mv='mv -i'

# clear tty
alias c='clear'

# show man page m command or args command
alias m='man $*'

# Reload all zsh functions (for easy development)
reloadfun() {
  local f
  f=(~/.oh-my-zsh/functions/*(.))
  unfunction $f:t 2> /dev/null
  autoload -U $f:t
}

# PostgreSQL
export PGDATA=/Library/PostgreSQL/8.4/data

# Ctags
# Build ctags for drupal projects
alias dctags='ctags --langmap=php:.install.inc.module.theme.php.profile --php-kinds=cdfi --languages=php --recurse'

hgdiff() {
    hg cat $1 | vim - -c  ":vert diffsplit $1" -c "map q :qa!<CR>";
}

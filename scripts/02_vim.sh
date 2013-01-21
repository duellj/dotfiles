#/bin/bash
#
# Install VIM.
#

info "installing vim"
echo ""

# Use MacVim as system wide vim.
brew tap homebrew/dupes
brew install macvim --override-system-vim
link_files /usr/local/opt/macvim/MacVim.app /Applications

# Fix python versioning problem
install_name_tool -change /System/Library/Frameworks/Python.framework/Versions/2.7/Python /usr/local/Cellar/python/2.7.3/Frameworks/Python.framework/Versions/Current/Python /Applications/MacVim.app/Contents/MacOS/Vim

# Link extra vimrc.
confirm_link $DOTFILES_ROOT/links/vim/vimrc $HOME/.vimrc
confirm_link $DOTFILES_ROOT/links/vim/gvimrc $HOME/.gvimrc

# Create cache directories
mkdir -p $HOME/Library/Vim/swap
mkdir -p $HOME/Library/Vim/backup
mkdir -p $HOME/Library/Vim/undo

# Install plugins via Vundle
if [ ! -d $HOME/.vim/bundle ]; then
  mkdir $HOME/.vim/bundle/
  /usr/local/bin/git clone https://github.com/gmarik/vundle.git $HOME/.vim/bundle/vundle
  /usr/local/bin/vim +BundleInstall +qall

  # Install old version of powerline
  /usr/local/bin/git --git-dir=$HOME/.vim/bundle/vim-powerline/.git checkout -b develop origin/develop
fi

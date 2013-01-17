#/bin/sh
#
# Install VIM.
#

info "installing vim"

# Use MacVim as system wide vim.
brew tap homebrew/dupes
brew install macvim --override-system-vim
link_files /usr/local/opt/macvim/MacVim.app /Applications

# Link extra vimrc.
confirm_link $DOTFILES_ROOT/links/vim/vimrc $HOME/.vimrc
confirm_link $DOTFILES_ROOT/links/vim/gvimrc $HOME/.gvimrc

# Create cache directories
mkdir -p $HOME/.cache/vim/swap
mkdir -p $HOME/.cache/vim/backup
mkdir -p $HOME/.cache/vim/undo

# Install plugins via Vundle
mkdir $HOME/links/vim/bundles/
git clone https://github.com/gmarik/vundle.git $HOME/links/vim/bundles/vundle
vim +BundleInstall +qall

# Install old version of powerline
git --git-dir=$HOME/.vim/bundle/powerline checkout -b develop origin/develop

# The following lines were added by compinstall

zstyle ':completion:*' menu select=2
zstyle :compinstall filename '/home/rob/.zshrc'

autoload -Uz compinit
compinit
# End of lines added by compinstall


### SETUP ALIASES
alias ll='ls -l'
alias ls='ls --color=auto'
alias gti=git
alias gerp=grep
alias vi=vim
alias ss='svn status --ignore-externals'
alias grep='grep --color=always'
alias less='less -r'
alias ssh='TERM=xterm-256color ssh' # prevent tmux/screen termtype from being forwarded to host

### SETUP ENVIRONMENT ###
export SVN_EDITOR=vim
export PATH=$PATH:~/bits-and-pieces/scripts/
# lib path for iVNT
export LD_LIBRARY_PATH=.:../deps/xerces:../deps/qt:../deps/protobuf:../deps/oracle:../deps/log4cxx:../deps/libxl:../deps/gtest:../deps/gsoap:../deps/boost:../deps/xerces/lib:../deps/qt/lib:../deps/protobuf/lib:../deps/oracle/lib:../deps/log4cxx/lib:../deps/libxl/lib:../deps/gtest/lib:../deps/boost/lib


### SETUP OPTIONS ###
setopt INC_APPEND_HISTORY
setopt HIST_IGNORE_ALL_DUPS
setopt AUTO_CD
setopt interactivecomments # allow comments on command-line
# need to understand extended globbing before enabling this:
#setopt EXTENDED_GLOB 


### SETUP KEYS ###
# note: at zsh prompt hit Ctrl+V then special key to find the code
bindkey "^\eOC" forward-word      # Ctrl+Right
bindkey "^\eOD" backward-word     # Ctrl+Left
bindkey "^[[1~" beginning-of-line # Home
bindkey "^[[4~" end-of-line       # End
bindkey "^[[3~" delete-char       # Delete


### COMPLETION OPTIONS ###



### SETUP PROMPT ###
autoload colors zsh/terminfo
colors

# create more readable color codes for prompt variable
for color in red green yellow blue magenta cyan white; do
	eval pr_lt_$color="%{$terminfo[bold]$fg[$color]%}"
	eval pr_$color="%{$fg[$color]%}"
done
pr_no_color="%{$terminfo[sgr0]%}"

prompt="$pr_lt_green%n$pr_no_color{$pr_lt_blue%1~ $pr_lt_red%?$pr_no_color} $pr_blue%h$pr_no_color# " 

# Lines configured by zsh-newuser-install
HISTFILE=~/.histfile
HISTSIZE=1000
SAVEHIST=1000
bindkey -e
# End of lines configured by zsh-newuser-install

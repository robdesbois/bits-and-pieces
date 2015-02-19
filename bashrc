# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# Fix common finger stupidities
alias gti=git
alias gerp=grep

alias vi=vim


export EDITOR=vim
export SVN_EDITOR=vim
export GCC_COLORS=1


function find-code {
	find $1 -name '*.cpp' -or -name '*.h' -or -name '*.tpl' -or -name '*.inl' | grep -v '/contrib/'
}

function find-non-ut-code {
	find-code $1 | grep -v '/UT_'
}


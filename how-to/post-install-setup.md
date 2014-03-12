* import github environment;
* enable font-antialiasing:
   ln -s /usr/share/fontconfig/conf.avail/10-autohint.conf /etc/fonts/conf.d/
   ln -s /usr/share/fontconfig/conf.avail/10-sub-pixel-rgb.conf /etc/fonts/conf.d/
* yum update

== Packages ==
* May need to remove vim-minimal before installing vim
* Setup rpmfusion free & non-free repositories from rpmfusion.org before proceeding

ant
boost boost-devel
gcc-c++
git
gnome-tweak-tool
kernel-devel
meld
mono-winforms
strace
subversion
tmux
valgrind
vim
xorg-x11-drv-nvidia

== Project-specific packages ==
libuuid-devel
cppunit cppunit-devel
inotify-tools inotify-tools-devel
openssl openssl-devel openssl-devel.i686
valgrind-devel


=== Non-repository packages ===
* google-chrome - due frequent update & slow repository pull from upstream
* sublime text - sublimetext.com 
* Virtual Box - virtualbox.org
* Inconsolata font - http://www.levien.com/type/myfonts/inconsolata.html


=== Debug info ===


== Sublime packages ==
* Package Control - sublime.wbond.net/installation
Alignment
BracketHighlighter
GitGutter
SidebarEnhancements
TomorrowColorSchemes

# My options #
#
# enable printing derived type using vtable info
set print object on

# nice structure printing
set print pretty on

# coloured prompt (\001 and \002 are to indicate non-printing characters to gdb; see http://dirac.org/linux/gdb/03-Initialization,_Listing,_And_Running.php
# note that coloured prompt doesn't work in TUI mode :-(
#set prompt \001\033[31m\002(gdb) \001\033[0m\002


# ---------------------------------------------------------------------------------- #
# python section to import pretty printers for Qt 4
python

import sys

sys.path.insert(0, '/home/rob/bits-and-pieces')
#from qt4 import register_qt4_printers
from qt4_gdb_pretty_printers import register_qt4_printers
register_qt4_printers (None)

end
# ---------------------------------------------------------------------------------- #


# Prints QString from Qt 4
# Taken from http://silmor.de/qtstuff.printqstring.php [Qt 5 version also available]
define printqstring
    printf "(QString)0x%x (length=%i): \"",&$arg0,$arg0.d->size
    set $i=0
    while $i < $arg0.d->size
        set $c=$arg0.d->data[$i++]
        if $c < 32 || $c > 127
                printf "\\u0x%04x", $c
        else
                printf "%c", (char)$c
        end
    end
    printf "\"\n"
end



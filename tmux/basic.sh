#!/bin/sh

TM_SESSION=basic

tmux new-session -d -c "$HOME" -n "admin" -s $TM_SESSION
tmux send-keys yum Space check-update

#  -------------------
# | b&p.git | b&p.git |
#  -------------------
cd "$HOME/bits-and-pieces"
tmux new-window 
tmux rename-window "bits n pieces"
tmux split-window -h -p40 # create RHS
tmux last-pane            # to LHS
#tmux split-window -v      # create bottom LHS
#tmux last-pane            # to top LHS
tmux send-keys cd Space "$HOME" Enter

tmux new-window -c "$HOME/workspace/" -n "workspace"

tmux attach-session -t $TM_SESSION

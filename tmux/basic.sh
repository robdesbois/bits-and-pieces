#!/bin/sh

TM_SESSION=basic

tmux new-session -d -c "$HOME" -n "admin" -s $TM_SESSION
tmux send-keys yum Space check-update

#  -------------------
# |  dd.git |         |
# |---------| env.git |
# | env.git |         |
#  -------------------
cd "$HOME/environment.git"
tmux new-window 
tmux rename-window "bits n pieces"
tmux split-window -h -p40 # create RHS
tmux last-pane            # to LHS
tmux split-window -v      # create bottom LHS
tmux last-pane            # to top LHS
tmux send-keys cd Space "$HOME/bits/dd-bits.git" Enter

tmux new-window -c "$HOME/dedupe" -n "dedupe"

tmux attach-session -t $TM_SESSION

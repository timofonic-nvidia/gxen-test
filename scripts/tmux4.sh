#!/bin/sh

PASS=$1

tmux start-server
tmux new-session -d -s test -n abc
tmux split-window -v
tmux split-window -h
tmux select-pane -t 0
tmux split-window -h

tmux select-pane -t 0
tmux send-keys -t test "ssh vm" C-m
tmux select-pane -t 1
tmux send-keys -t test "ssh vm1" C-m
tmux select-pane -t 2
tmux send-keys -t test "ssh vm2" C-m
tmux select-pane -t 3
tmux send-keys -t test "ssh vm3" C-m

tmux select-pane -t 0
#tmux send-keys -t test "cd Projects; echo $PASS | sudo -S ./run-gxen.sh" C-m
tmux send-keys -t test "cd Projects; echo $PASS | su -c ./run-gxen.sh; echo $PASS | su -c ./run-test.sh" C-m
tmux select-pane -t 1
tmux send-keys -t test "cd Projects; echo $PASS | su -c ./run-gxen.sh; echo $PASS | su -c ./run-test.sh" C-m
tmux select-pane -t 2
tmux send-keys -t test "cd Projects; echo $PASS | su -c ./run-gxen.sh; echo $PASS | su -c ./run-test.sh" C-m
tmux select-pane -t 3
tmux send-keys -t test "cd Projects; echo $PASS | su -c ./run-gxen.sh; echo $PASS | su -c ./run-test.sh" C-m

tmux attach -t test

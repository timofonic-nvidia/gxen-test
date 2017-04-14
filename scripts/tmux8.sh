#!/bin/sh

PASS=$1

tmux start-server
tmux new-session -d -s test -n abc
tmux split-window -v
tmux split-window -h
tmux select-pane -t 0
tmux split-window -h

tmux split-window -h
tmux select-pane -t 0
tmux split-window -h
tmux select-pane - t 1
tmux split-window -h
tmux select-pane -t 3
tmux split-window -h

for i in {0..7}
do
  tmux select-pane -t ${i}
  tmux send-keys -t test "ssh vm${i}" C-m
done

for i in {0..7}
do
  tmux select-pane -t ${i}
  tmux send-keys -t test "cd Projects; echo $PASS | su -c ./run-gxen.sh; echo $PASS | su -c ./run-test.sh" C-m
done

tmux attach -t test

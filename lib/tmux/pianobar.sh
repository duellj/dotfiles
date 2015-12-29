#!/bin/bash

if ps ax | grep -v grep | grep pianobar$ > /dev/null
then
  cat /Users/jonduell/.config/pianobar/current-song.txt
else
  echo ''
fi

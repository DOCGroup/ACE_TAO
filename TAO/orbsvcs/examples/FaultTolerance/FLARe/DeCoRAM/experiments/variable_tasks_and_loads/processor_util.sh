#!/bin/bash
EXPERIMENT_NAME=processor_util
DECORAM_BIN=../../bin
DATA_DIR=$1
RESULT_DIR=$DATA_DIR/$EXPERIMENT_NAME

if [ ! -d ./$DATA_DIR ]; then echo Directory $DATA_DIR does not exist && exit; fi
if [ ! -d ./$RESULT_DIR ]; then mkdir -p ./$RESULT_DIR; fi
rm ./$RESULT_DIR/*.dat

for TASK_NUMBER in 10 20 40 80 160; do \
  for ALGORITHM in aftrmff sftrmff uftrmff rftrmff pftrmff; do \
    echo "count processors used by $ALGORITHM for $TASK_NUMBER tasks..." && \
    $DECORAM_BIN/scheck -c -f ./$DATA_DIR/$ALGORITHM-$TASK_NUMBER.txt >> ./$RESULT_DIR/$EXPERIMENT_NAME-$ALGORITHM.dat; 
  done;\
done
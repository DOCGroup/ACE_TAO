#!/bin/bash
EXPERIMENT_NAME=task_properties
DECORAM_BIN=../../bin
DATA_DIR=$1
RESULT_DIR=$DATA_DIR/$EXPERIMENT_NAME

if [ ! -d ./$DATA_DIR ]; then echo Directory $DATA_DIR does not exist && exit; fi
if [ ! -d ./$RESULT_DIR ]; then mkdir -p ./$RESULT_DIR; fi

for TASK_NUMBER in 10 20 40 80 160; do \
  echo "analyse properties of taskset with $TASK_NUMBER tasks..." && \
  echo "$TASK_NUMBER tasks:" >> ./$RESULT_DIR/$EXPERIMENT_NAME.dat
  $DECORAM_BIN/scheck -a -f ./$DATA_DIR/$TASK_NUMBER-tasks.input >> ./$RESULT_DIR/$EXPERIMENT_NAME.dat; 
done
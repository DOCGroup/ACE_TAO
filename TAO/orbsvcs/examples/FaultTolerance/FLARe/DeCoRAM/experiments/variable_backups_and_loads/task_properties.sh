#!/bin/bash
EXPERIMENT_NAME=task_properties
DECORAM_BIN=../../bin
DATA_DIR=$1
RESULT_DIR=$DATA_DIR/$EXPERIMENT_NAME

if [ ! -d ./$DATA_DIR ]; then echo Directory $DATA_DIR does not exist && exit; fi
if [ ! -d ./$RESULT_DIR ]; then mkdir -p ./$RESULT_DIR; fi

echo "analyse properties of taskset..."
$DECORAM_BIN/scheck -a -f ./$DATA_DIR/tasks.input > ./$RESULT_DIR/$EXPERIMENT_NAME.dat
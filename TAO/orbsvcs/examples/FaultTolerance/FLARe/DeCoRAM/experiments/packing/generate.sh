#!/bin/bash
DECORAM_BIN=../../bin
DATA_DIR=$1

#experiment parameters
INTERVAL_NUMBER=1
MAXIMUM_LOAD=0.25
PERIOD_UPPER=1000.0
PERIOD_LOWER=1.0
SS_UPPER=0.01
SS_LOWER=0.005

if [ ! -d ./$DATA_DIR ]; then mkdir ./$DATA_DIR; fi

for TASK_NO in 10 20 40 80 160; do \
  echo "run tgen for $TASK_NO tasks..." && \
  $DECORAM_BIN/tgen \
    -n $TASK_NO \
    -i $INTERVAL_NUMBER \
    -m $MAXIMUM_LOAD \
    -p $PERIOD_UPPER \
    -l $PERIOD_LOWER \
    -s $SS_UPPER \
    -z $SS_LOWER > ./$DATA_DIR/$TASK_NO-tasks.input && \
  for BACKUP_NO in 1 2 3 4 5; do \
    for ALGORITHM in pftrmff aftrmff frftrmff; do \
      echo "run $ALGORITHM for $BACKUP_NO backups..." && \
      $DECORAM_BIN/$ALGORITHM \
        ./$DATA_DIR/$TASK_NO-tasks.input \
        $[ $TASK_NO * $BACKUP_NO ] \
           $BACKUP_NO > ./$DATA_DIR/$ALGORITHM-$BACKUP_NO-backups-$TASK_NO-tasks.txt; \
    done; \
  done; \
done
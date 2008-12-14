#!/bin/bash
DECORAM_BIN=../../bin
DATA_DIR=$1-$2

#experiment parameters
PROCESSOR_NO=10
TASK_NUMBER=160
INTERVAL_NUMBER=1
MAXIMUM_LOAD=0.$2
PERIOD_UPPER=1000.0
PERIOD_LOWER=1.0
SS_UPPER=0.15
SS_LOWER=0.10

if [ ! -d ./$DATA_DIR ]; then mkdir ./$DATA_DIR; fi

echo "run tgen for $TASK_NUMBER tasks..."
$DECORAM_BIN/tgen \
  -n $TASK_NUMBER \
  -i $INTERVAL_NUMBER \
  -m $MAXIMUM_LOAD \
  -p $PERIOD_UPPER \
  -l $PERIOD_LOWER \
  -s $SS_UPPER \
  -z $SS_LOWER > ./$DATA_DIR/tasks.input

for BACKUP_NO in 0 1 2 3 4 5; do \
  for ALGORITHM in bftrmff eftrmff rftrmff sftrmff; do
    echo "run $ALGORITHM for $BACKUP_NO backups..." && \
    $DECORAM_BIN/$ALGORITHM \
      ./$DATA_DIR/tasks.input \
      $TASK_NUMBER \
      $BACKUP_NO > ./$DATA_DIR/$ALGORITHM-$BACKUP_NO-backups.txt; \
  done; \
done
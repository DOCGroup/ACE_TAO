#!/bin/bash
DECORAM_BIN=../../bin
DATA_DIR=$1-$2

#experiment parameters
PROCESSOR_NO=10
BACKUP_NO=3
INTERVAL_NUMBER=1
MAXIMUM_LOAD=0.$2
PERIOD_UPPER=1000.0
PERIOD_LOWER=1.0
SS_UPPER=0.15
SS_LOWER=0.10

if [ ! -d ./$DATA_DIR ]; then mkdir ./$DATA_DIR; fi
rm ./$DATA_DIR/*.but ./$DATA_DIR/*.sut ./$DATA_DIR/*.eut ./$DATA_DIR/*.rut

for TASK_NUMBER in 10 20 40 80 160; do \
  echo "run tgen for $TASK_NUMBER tasks..." && \
  $DECORAM_BIN/tgen \
    -n $TASK_NUMBER \
    -i $INTERVAL_NUMBER \
    -m $MAXIMUM_LOAD \
    -p $PERIOD_UPPER \
    -l $PERIOD_LOWER \
    -s $SS_UPPER \
    -z $SS_LOWER > ./$DATA_DIR/$TASK_NUMBER-tasks.input && \
  for ALGORITHM in bftrmff sftrmff eftrmff rftrmff; do \
    echo "run $ALGORITHM for $TASK_NUMBER tasks..." && \
    $DECORAM_BIN/$ALGORITHM \
      ./$DATA_DIR/$TASK_NUMBER-tasks.input \
      $[ $TASK_NUMBER * $BACKUP_NO ] \
      $BACKUP_NO > ./$DATA_DIR/$ALGORITHM-$TASK_NUMBER.txt; \
  done && \
  echo "deploy only primaries for $TASK_NUMBER tasks..." && \
  $DECORAM_BIN/sftrmff \
      ./$DATA_DIR/$TASK_NUMBER-tasks.input \
      $TASK_NUMBER \
      0 > ./$DATA_DIR/pftrmff-$TASK_NUMBER.txt; \
done
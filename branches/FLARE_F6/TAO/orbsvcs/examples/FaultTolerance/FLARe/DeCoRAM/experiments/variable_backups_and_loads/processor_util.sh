#!/bin/bash
EXPERIMENT_NAME=processor_util
DECORAM_BIN=../../bin
DATA_DIR=$1
RESULT_DIR=$DATA_DIR/$EXPERIMENT_NAME

if [ ! -d ./$DATA_DIR ]; then echo Directory $DATA_DIR does not exist && exit; fi
if [ ! -d ./$RESULT_DIR ]; then mkdir -p ./$RESULT_DIR; fi

for BACKUP_NO in 0 1 2 3 4 5; do \
  for ALGORITHM in aftrmff uftrmff rftrmff sftrmff; do
    echo "count processors used by $ALGORITHM for $BACKUP_NO backups..." && \
    $DECORAM_BIN/scheck -c -f ./$DATA_DIR/$ALGORITHM-$BACKUP_NO-backups.txt >> ./$RESULT_DIR/$EXPERIMENT_NAME-$ALGORITHM.dat; \
  done; \
done
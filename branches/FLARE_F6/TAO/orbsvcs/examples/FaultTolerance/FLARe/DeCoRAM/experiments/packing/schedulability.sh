#!/bin/bash
EXPERIMENT_NAME=schedulability
DECORAM_BIN=../../bin
DATA_DIR=$1
RESULT_DIR=$DATA_DIR/$EXPERIMENT_NAME

if [ ! -d ./$DATA_DIR ]; then echo Directory $DATA_DIR does not exist && exit; fi
if [ ! -d ./$RESULT_DIR ]; then mkdir -p ./$RESULT_DIR; fi

for ALGORITHM in pftrmff aftrmff frftrmff bsftrmbf bsftrmwf; do \
  for TASK_NO in 10 20 40 80 160; do \
    echo "$TASK_NO tasks:" >> ./$RESULT_DIR/$EXPERIMENT_NAME-$ALGORITHM.dat && \
    for BACKUP_NO in 1 2 3 4; do \
      if [ -s ./$DATA_DIR/$ALGORITHM-$BACKUP_NO-backups-$TASK_NO-tasks.txt ]; then \
        echo "check schedulability of $ALGORITHM result for $BACKUP_NO backups and $TASK_NO tasks ..." && \
        echo "$BACKUP_NO backups: `$DECORAM_BIN/scheck -f ./$DATA_DIR/$ALGORITHM-$BACKUP_NO-backups-$TASK_NO-tasks.txt`" >> ./$RESULT_DIR/$EXPERIMENT_NAME-$ALGORITHM.dat; \
      fi;\
    done; \
  done; \
done
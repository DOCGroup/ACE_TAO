#!/bin/bash
EXPERIMENT_NAME=processor_util
DECORAM_BIN=../../bin
DATA_DIR=$1
RESULT_DIR=$DATA_DIR/$EXPERIMENT_NAME
REPEAT=20

if [ ! -d ./$DATA_DIR ]; then echo Directory $DATA_DIR does not exist && exit; fi
if [ ! -d ./$RESULT_DIR ]; then mkdir -p ./$RESULT_DIR; fi

for RUN in `/usr/bin/seq $REPEAT`; do \
  for ALGORITHM in pftrmff aftrmff frftrmff bsftrmbf bsftrmwf bsftrmbfnf bsftrmwfnf; do \
    for TASK_NO in 10 20 40 80 160; do \
      echo "$TASK_NO tasks:" >> ./$RESULT_DIR/$RUN-$EXPERIMENT_NAME-$ALGORITHM.dat && \
      for BACKUP_NO in 1 2 3 4; do \
        if [ -s ./$DATA_DIR/$RUN-$ALGORITHM-$BACKUP_NO-backups-$TASK_NO-tasks.txt ]; then \
          echo "count processors used in run $RUN by $ALGORITHM for $BACKUP_NO backups and $TASK_NO tasks ..." && \
          echo "$BACKUP_NO backups: `$DECORAM_BIN/scheck -c -f ./$DATA_DIR/$RUN-$ALGORITHM-$BACKUP_NO-backups-$TASK_NO-tasks.txt`" >> ./$RESULT_DIR/$RUN-$EXPERIMENT_NAME-$ALGORITHM.dat; \
        fi;\
      done; \
    done; \
  done; \
done
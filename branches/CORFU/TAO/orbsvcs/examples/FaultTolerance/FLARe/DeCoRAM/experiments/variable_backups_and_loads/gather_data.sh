DATA_FILE=variable_backups_and_loads.dat

for ALGORITHM in sftrmff rftrmff eftrmff bftrmff; do \
  echo "Algorithm $ALGORITHM:" >> $DATA_FILE && \
  for LOAD in 10 20 30 40 50; do \
    echo "Results for $LOAD % load:" >> $DATA_FILE && \
    for RUN in 1 2 3 4 5 6 7 8 9 10; do \
      echo "Run #$RUN:" >> $DATA_FILE && \
      cat load-run$RUN-$LOAD/processor_util/processor_util-$ALGORITHM.dat >> $DATA_FILE; \
    done; \
  done; \
done
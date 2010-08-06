for TURN in 1 2 3 4 5 6 7 8 9 10; do \
  ./generate.sh load-run$TURN $1 && \
  ./processor_util.sh load-run$TURN-$1 && \
  ./task_properties.sh load-run$TURN-$1; \
done
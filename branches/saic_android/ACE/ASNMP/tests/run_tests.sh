#!/bin/sh
# This is the UNIX version of the one-button ACE tests.
# Contributed by Michael Rueger <m_rueger@SYSCOMP.DE>
# mrm@cisco.com - Change to for loop  script could be more generic if we put
# this in the bin dir and fed it a file to read from

IFS="|"
tmp=/tmp

# these patterns should not be included in log file
ERROR_MSGS="assertion failed|not supported|No such file or directory|Invalid argument|timeout|Bad file number"

# these patterns must be included in log file
SUCCESS_MSGS="starting|Ending"

run()
{
  echo running $1
  /bin/rm -f core

  ./$1
  status=$?

  if [ $status -ne 0 ]; then
    echo \"$1\" FAILED with exit status $status!!!!
  fi

  if [ -f core ]; then
    echo \"$1\" dumped core!!!!
  fi

  for i in $SUCCESS_MSGS; do
    grep $i log/$1.log >/dev/null
    if [ $? -eq 1 ]; then
      echo Error in log file no line with $i
    fi
  done

  for i in $ERROR_MSGS; do
    grep $i log/$1.log
  done
}

echo "Starting tests..."
FILES=`ls *_Test 2>/dev/null`

if [ -z $FILES ]; then
  echo "ERROR: no test programs generated matching pattern *_Test."
  echo "ERROR: Try compiling the test programs first."
  exit 1
fi

for i in *_Test
do
  run $i
done

echo "Tests complete..."


# EOF

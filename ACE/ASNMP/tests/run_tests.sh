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
    status=1
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

  return $status
}

echo "Starting tests..."
found=0
failed=0
for i in *_Test
do
  if [ ! -x "$i" ]; then
    continue
  fi
  found=1
  if ! run "$i"; then
    failed=1
  fi
done

echo "Tests complete..."
if [ "$found" -eq 0 ]; then
  echo "ERROR: no executable tests found matching *_Test."
  echo "ERROR: Try compiling the test programs first."
  exit 1
fi
exit "$failed"


# EOF

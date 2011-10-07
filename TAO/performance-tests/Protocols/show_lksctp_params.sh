#!/bin/sh
# $Id$

max_len=24
pushd /proc/sys/net/sctp &> /dev/null
echo "SCTP Parameters"
for i in `ls *`; do
	ct=0
	tmp_len=`echo ${i} | wc -m`
	let "tmp_len=tmp_len-1"
	let "spaces=${max_len}-${tmp_len}"
	echo -n " $i:"
	while [ ${ct} -lt ${spaces} ]; do
	  echo -n " "
	  let "ct=${ct}+1"
	done
	cat $i
done
echo

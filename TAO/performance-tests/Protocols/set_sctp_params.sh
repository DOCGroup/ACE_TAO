#!/bin/sh
# $Id$

B_PROC=/proc/sys/net/ipv4

CONF_PATH=/groups/pces/uav_oep/oomworks/openss7-debug/TAO/performance-tests/Protocols
. ${CONF_PATH}/sctp_params.conf

for i in $SETTINGS; do
	eval "echo \"echo \$$i > \$B_PROC/\${PREFIX}${i}\""
	eval "echo \$$i > \$B_PROC/\${PREFIX}${i}"
done

#!/bin/sh
# $Id$

if test -z $ACE_ROOT; then ACE_ROOT=..; fi
if test -z $TAO_ROOT; then TAO_ROOT=${ACE_ROOT}/TAO; fi
if test -z $CIAO_ROOT; then CIAO_ROOT=${TAO_ROOT}/CIAO; fi
if test -z $DANCE_ROOT; then DANCE_ROOT=${TAO_ROOT}/DAnCE; fi
#
wget http://download.theaceorb.nl/teststat/cleanbuild_testfails-YYYY_MM_DD.txt -O cleanbuild.txt
#
grep -h \!FIXED_BUGS_ONLY ${ACE_ROOT}/tests/*.lst ${ACE_ROOT}/bin/*.lst ${TAO_ROOT}/bin/*.lst ${CIAO_ROOT}/bin/*.lst ${DANCE_ROOT}/bin/*.lst | sed -e "s/^\([^\:]*\).*/\1/" | sed -e "s/\(\/run_test.pl\)\?\s*$//" > Ignore.txt
#
cat cleanbuild.txt | grep -v -f Ignore.txt | tee cleanbuildresults.txt


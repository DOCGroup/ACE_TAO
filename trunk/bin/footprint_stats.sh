#!/bin/sh
#
# $Id$
#


if [ $# -lt 2 ]; then
  echo "Usage: $0 [ACE_ROOT] [DEST]"
  exit 0
fi

ACE_ROOT=$1
DEST=$2
DATE=`date +%Y/%m/%d-%H:%M`

BINS="TAO/tests/ORB_init/ORB_init TAO/tests/ORB_destroy/ORB_destroy"

LIBS="ace/libACE.a \
    TAO/tao/libTAO.a \
    TAO/tao/PortableServer/libTAO_PortableServer.a \
    TAO/tao/Strategies/libTAO_Strategies.a \
    TAO/tao/DynamicAny/libTAO_DynamicAny.a \
    TAO/tao/DynamicInterface/libTAO_DynamicInterface.a \
    TAO/tao/IFR_Client/libTAO_IFR_Client.a \
    TAO/tao/BiDir_GIOP/libTAO_BiDirGIOP.a \
    TAO/tao/Domain/libTAO_Domain.a \
    TAO/tao/IORManipulation/libTAO_IORManip.a \
    TAO/tao/IORTable/libTAO_IORTable.a \
    TAO/tao/TypeCodeFactory/libTAO_TypeCodeFactory.a \
    TAO/tao/RTCORBA/libTAO_RTCORBA.a \
    TAO/tao/IORInterceptor/libTAO_IORInterceptor.a \
    TAO/tao/Messaging/libTAO_Messaging.a \
    TAO/tao/ObjRefTemplate/libTAO_ObjRefTemplate.a \
    TAO/tao/ObjRefTemplate/libTAO_Valuetype.a \
    TAO/tao/ObjRefTemplate/libTAO_RTScheduler.a \
    TAO/tao/RTPortableServer/libTAO_RTPortableServer.a"

cd $ACE_ROOT

for i in $BINS; do
  b=`basename $i`
  if [ -x $i ]; then
    (
      echo -n $DATE " ";
      size $i |
      grep -v text |
      awk '{print $4}'
    ) >> $DEST/source/${b}_size.txt
  fi
done

for i in $LIBS; do
  b=`basename $i`;
  if [ -f $i ]; then
    (
      echo -n $DATE " ";
      size $i |
       awk '{s += $4} END {print s}'
    ) >> $DEST/source/${b}_size.txt
  fi
done

cd $DEST/source

for i in $LIBS $BINS; do
 b=`basename $i`
 /usr/bin/tac ${b}_size.txt > $DEST/data/${b}_size.txt
 /usr/bin/tail -5 ${b}_size.txt > $DEST/data/LAST_${b}_size.txt
 $ACE_ROOT/bin/generate_footprint_chart.sh ${b}_size.txt $DEST/images/${b}_size.png $b
done

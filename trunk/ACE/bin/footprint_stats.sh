#!/bin/sh
#
# $Id$
#


if [ $# -lt 1 ]; then
  echo "Usage: $0 [DEST]"
  exit 0
fi

DEST=$1
DATE=`date +%Y/%m/%d-%H:%M`

BINS="$TAO_ROOT/tests/ORB_init/ORB_init $TAO_ROOT/tests/ORB_destroy/ORB_destroy"

LIBS="$ACE_ROOT/ace/libACE.a \
    $TAO_ROOT/tao/libTAO.a \
    $TAO_ROOT/tao/libTAO_PortableServer.a \
    $TAO_ROOT/tao/libTAO_Strategies.a \
    $TAO_ROOT/tao/libTAO_SmartProxies.a \
    $TAO_ROOT/tao/libTAO_DynamicAny.a \
    $TAO_ROOT/tao/libTAO_DynamicInterface.a \
    $TAO_ROOT/tao/libTAO_IFR_Client.a \
    $TAO_ROOT/tao/libTAO_BiDirGIOP.a \
    $TAO_ROOT/tao/libTAO_Domain.a \
    $TAO_ROOT/tao/libTAO_IORManip.a \
    $TAO_ROOT/tao/libTAO_IORTable.a \
    $TAO_ROOT/tao/libTAO_TypeCodeFactory.a \
    $TAO_ROOT/tao/libTAO_RTCORBA.a \
    $TAO_ROOT/tao/libTAO_IORInterceptor.a \
    $TAO_ROOT/tao/libTAO_Messaging.a \
    $TAO_ROOT/tao/libTAO_ObjRefTemplate.a \
    $TAO_ROOT/tao/libTAO_Valuetype.a \
    $TAO_ROOT/tao/libTAO_RTScheduler.a \
    $TAO_ROOT/tao/libTAO_AnyTypeCode.a \
    $TAO_ROOT/tao/libTAO_PI.a \
    $TAO_ROOT/tao/libTAO_PI_Server.a \
    $TAO_ROOT/tao/libTAO_Codeset.a \
    $TAO_ROOT/tao/libTAO_CodecFactory.a \
    $TAO_ROOT/tao/libTAO_RTPortableServer.a"

mkdir -p $DEST/source
mkdir -p $DEST/data
mkdir -p $DEST/images

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

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
    $TAO_ROOT/tao/PortableServer/libTAO_PortableServer.a \
    $TAO_ROOT/tao/Strategies/libTAO_Strategies.a \
    $TAO_ROOT/tao/SmartProxies/libTAO_SmartProxies.a \
    $TAO_ROOT/tao/DynamicAny/libTAO_DynamicAny.a \
    $TAO_ROOT/tao/DynamicInterface/libTAO_DynamicInterface.a \
    $TAO_ROOT/tao/IFR_Client/libTAO_IFR_Client.a \
    $TAO_ROOT/tao/BiDir_GIOP/libTAO_BiDirGIOP.a \
    $TAO_ROOT/tao/IORManipulation/libTAO_IORManip.a \
    $TAO_ROOT/tao/IORTable/libTAO_IORTable.a \
    $TAO_ROOT/tao/TypeCodeFactory/libTAO_TypeCodeFactory.a \
    $TAO_ROOT/tao/RTCORBA/libTAO_RTCORBA.a \
    $TAO_ROOT/tao/IORInterceptor/libTAO_IORInterceptor.a \
    $TAO_ROOT/tao/Messaging/libTAO_Messaging.a \
    $TAO_ROOT/tao/ObjRefTemplate/libTAO_ObjRefTemplate.a \
    $TAO_ROOT/tao/Valuetype/libTAO_Valuetype.a \
    $TAO_ROOT/tao/RTScheduling/libTAO_RTScheduler.a \
    $TAO_ROOT/tao/AnyTypeCode/libTAO_AnyTypeCode.a \
    $TAO_ROOT/tao/PI/libTAO_PI.a \
    $TAO_ROOT/tao/PI_Server/libTAO_PI_Server.a \
    $TAO_ROOT/tao/Codeset/libTAO_Codeset.a \
    $TAO_ROOT/tao/CodecFactory/libTAO_CodecFactory.a \
    $TAO_ROOT/tao/RTPortableServer/libTAO_RTPortableServer.a"

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

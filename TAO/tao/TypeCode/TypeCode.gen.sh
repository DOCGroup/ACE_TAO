#! /bin/sh

for p in `cat TypeCode.lst.txt | sed -e 's/Struct//'`; do
    u=`echo $p | tr [:lower:] [:upper:]`
    z=`echo $p | tr [:upper:] [:lower:]`

    cp Struct_TypeCode.h ${p}_TypeCode.h
    cp Struct_TypeCode.cpp ${p}_TypeCode.cpp

    eval "sed -e 's/Struct/$p/g' -e 's/tk_struct/tk_${z}/g' -e 's/TAO_STRUCT_TYPECODE_H/TAO_${u}_TYPECODE_H/g' ${p}_TypeCode.h > ${p}_TypeCode.h.new"
    mv ${p}_TypeCode.h.new ${p}_TypeCode.h

    eval "sed -e 's/Struct/$p/g' -e 's/tk_struct/tk_${z}/g' ${p}_TypeCode.cpp > ${p}_TypeCode.cpp.new"
    mv ${p}_TypeCode.cpp.new ${p}_TypeCode.cpp

done

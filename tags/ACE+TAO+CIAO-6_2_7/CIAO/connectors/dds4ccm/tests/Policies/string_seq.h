//$Id$

#ifndef STRING_SEQ_H_
#define STRING_SEQ_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/StringSeq.h"

class StringSequence
{
public:
  StringSequence ();

  static bool check (const ::DDS_StringSeq & dds_seq,
                     const ::DDS::StringSeq & seq,
                     const char * test);
};

#endif /* STRING_SEQ_H_ */

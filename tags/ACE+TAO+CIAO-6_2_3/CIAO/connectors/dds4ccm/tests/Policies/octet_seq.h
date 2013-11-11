//$Id$

#ifndef OCTET_SEQ_H_
#define OCTET_SEQ_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/ndds/convertors/OctetSeq.h"

class OctetSequence
{
public:
  OctetSequence ();

  static bool check (const ::DDS_OctetSeq & dds_seq,
                     const ::DDS::OctetSeq & seq,
                     const char * test);
};

#endif /* OCTET_SEQ_H_ */

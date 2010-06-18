//$Id$

#ifndef DURATION_H_
#define DURATION_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ndds/ndds_cpp.h"
#include "dds4ccm/impl/dds/ndds/Duration_t.h"

class Duration
{
public:
  Duration ();

  static bool check (const ::DDS_Duration_t & dds_dur,
                     const ::DDS::Duration_t & dur);
};

#endif /* DURATION_H_ */

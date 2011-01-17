// -*- C++ -*-
// $Id$

#include "dds_rtf2_dcps_instance_handleC.h"

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)

static ::DDS::InstanceHandle_t S_HANDLE = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 16, 0};

namespace DDS
{
  ::DDS::InstanceHandle_t const HANDLE_NIL = S_HANDLE;
}

#endif

// -*- C++ -*-
// $Id$

#include "ndds_dcps_instance_handleC.h"

static ::DDS::InstanceHandle_t S_HANDLE = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 16, 0};

namespace DDS
{
  ::DDS::InstanceHandle_t const HANDLE_NIL = S_HANDLE;
}


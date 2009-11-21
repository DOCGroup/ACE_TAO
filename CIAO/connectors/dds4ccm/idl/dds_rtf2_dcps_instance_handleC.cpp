// -*- C++ -*-
// $Id$

#include "dds_rtf2_dcps_instance_handleC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

static ::DDS::InstanceHandle_t S_HANDLE = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 16, 0};

namespace DDS
{
  ::DDS::InstanceHandle_t const HANDLE_NIL = S_HANDLE;
}

TAO_END_VERSIONED_NAMESPACE_DECL

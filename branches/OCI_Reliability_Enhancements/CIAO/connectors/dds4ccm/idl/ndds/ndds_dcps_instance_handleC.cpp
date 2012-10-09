// -*- C++ -*-
// $Id$

#include "ndds_dcps_instance_handleC.h"

static ::DDS::InstanceHandle_t S_HANDLE = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 16, 0};

namespace DDS
{
  ::DDS::InstanceHandle_t const HANDLE_NIL = S_HANDLE;
}


bool
operator== (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  DDS_InstanceHandle_t i1 = DDS_HANDLE_NIL;
  DDS_InstanceHandle_t i2 = DDS_HANDLE_NIL;
  i1 <<= instancehandle1;
  i2 <<= instancehandle2;
  if (DDS_InstanceHandle_is_nil(&instancehandle1))
    {
      return DDS_InstanceHandle_is_nil (&instancehandle2);
    }
  else if (DDS_InstanceHandle_is_nil(&instancehandle2))
    {
      return DDS_InstanceHandle_is_nil (&instancehandle1);
    }
  else
    {
      return DDS_InstanceHandle_equals (&i1, &i2);
    }
}

bool
operator!= (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  return !operator== (instancehandle1, instancehandle2);
}

void
operator<<= (::DDS::InstanceHandle_t &ddsinstancehandle, const ::DDS_InstanceHandle_t & instancehandle)
{
  if (DDS_InstanceHandle_is_nil (&instancehandle))
    {
      ddsinstancehandle = DDS::HANDLE_NIL;
    }
  else
    {
      ACE_OS::memcpy (ddsinstancehandle.value, instancehandle.keyHash.value, sizeof (instancehandle.keyHash.value));
      ddsinstancehandle.length = instancehandle.keyHash.length;
      ddsinstancehandle.isValid = instancehandle.isValid;
    }
}

void
operator<<= (::DDS_InstanceHandle_t &ddsinstancehandle, const ::DDS::InstanceHandle_t & instancehandle)
{
  ACE_OS::memcpy (ddsinstancehandle.keyHash.value, instancehandle.value, sizeof (instancehandle.value));
  ddsinstancehandle.keyHash.length = instancehandle.length;
  ddsinstancehandle.isValid = instancehandle.isValid;
}

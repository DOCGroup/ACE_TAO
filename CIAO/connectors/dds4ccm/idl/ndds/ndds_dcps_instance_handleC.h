// -*- C++ -*-
// $Id$

#ifndef _TAO_IDL_DDS_RTF2_DCPSC_H_XX
#define _TAO_IDL_DDS_RTF2_DCPSC_H_XX

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include /**/ "ndds_stub_export.h"
#include /**/ "tao/Versioned_Namespace.h"

#include "ndds_dcps_i_handleC.h"

namespace DDS
{
  extern NDDS_STUB_Export ::DDS::InstanceHandle_t const HANDLE_NIL;
}

inline bool
operator== (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  return instancehandle1.length  == instancehandle2.length  &&
         instancehandle1.isValid == instancehandle2.isValid &&
         ACE_OS::memcmp (instancehandle1.value, instancehandle2.value, sizeof (instancehandle1.value)) == 0;
}

inline bool
operator!= (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  return instancehandle1.length  != instancehandle2.length  ||
         instancehandle1.isValid != instancehandle2.isValid ||
         ACE_OS::memcmp (instancehandle1.value, instancehandle2.value, sizeof (instancehandle1.value)) != 0;
}

#include /**/ "ace/post.h"

#endif /* ifndef */



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
#include "ndds/ndds_cpp.h"

namespace DDS
{
  extern NDDS_STUB_Export ::DDS::InstanceHandle_t const HANDLE_NIL;
}

NDDS_STUB_Export bool
operator== (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2);

NDDS_STUB_Export bool
operator!= (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2);

NDDS_STUB_Export void
operator<<= (::DDS::InstanceHandle_t &ddsinstancehandle, const ::DDS_InstanceHandle_t & instancehandle);

NDDS_STUB_Export void
operator<<= (::DDS_InstanceHandle_t &ddsinstancehandle, const ::DDS::InstanceHandle_t & instancehandle);

#include /**/ "ace/post.h"

#endif /* ifndef */



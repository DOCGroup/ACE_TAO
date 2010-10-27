// -*- C++ -*-
//=============================================================================
/**
 *  @file DLL_Resident_ORB_Initializer.cpp
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilsond@ociweb.com>
 */
//=============================================================================

#include "tao/PI/DLL_Resident_ORB_Initializer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

PortableInterceptor::DLL_Resident_ORB_Initializer::DLL_Resident_ORB_Initializer (
  PortableInterceptor::ORBInitializer_ptr initializer,
  const ACE_TCHAR * dll_name)
  : initializer_(PortableInterceptor::ORBInitializer::_duplicate(initializer))
  , dll_(dll_name)
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("construct DLL_Resident_ORB_Initializer for @%@\n"),
        initializer_.operator->()
        ));
    }
}

PortableInterceptor::DLL_Resident_ORB_Initializer::~DLL_Resident_ORB_Initializer ()
{
  if (TAO_debug_level > 8)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("Destruct DLL_Resident_ORB_Initializer for @%@\n"),
        initializer_.operator->()
        ));
    }
  // let the var release the object first
  initializer_ = PortableInterceptor::ORBInitializer::_nil();
  // then the ACE_DLL can release the dll on the way out.
}


void
PortableInterceptor::DLL_Resident_ORB_Initializer::pre_init (
    ::PortableInterceptor::ORBInitInfo_ptr info
  )
{
  initializer_->pre_init (info);
}

void
PortableInterceptor::DLL_Resident_ORB_Initializer::post_init (
    ::PortableInterceptor::ORBInitInfo_ptr info
  )
{
  initializer_->post_init (info);
}


TAO_END_VERSIONED_NAMESPACE_DECL

/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Current_ORBInitializer_Base.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================


#ifndef CURRENT_ORBINITIALIZER_BASE_H
#define CURRENT_ORBINITIALIZER_BASE_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/TransportCurrent/TCC.h"

#include "tao/TransportCurrent/Transport_Current_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {
    class TAO_Transport_Current_Export Current_ORBInitializer_Base
      : public virtual PortableInterceptor::ORBInitializer
      , public virtual TAO_Local_RefCounted_Object
    {
    public:
      Current_ORBInitializer_Base (const ACE_TCHAR* id);
      virtual ~Current_ORBInitializer_Base (void);

      virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr
                             ACE_ENV_ARG_DECL_NOT_USED )
        ACE_THROW_SPEC( (CORBA::SystemException) );

      virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                              ACE_ENV_ARG_DECL_NOT_USED )
        ACE_THROW_SPEC( (CORBA::SystemException) );

    protected:
      virtual TAO::Transport::Current_ptr
      make_current_instance (TAO_ORB_Core* core,
                             size_t tss_slot_id
                             ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

    protected:
      const ACE_TString id_;
    };

  } /* namespace Transport */

} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* CURRENT_ORBINITIALIZER_BASE_H */

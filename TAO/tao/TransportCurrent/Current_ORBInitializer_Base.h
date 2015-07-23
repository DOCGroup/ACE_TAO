/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Current_ORBInitializer_Base.h
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
      , public virtual ::CORBA::LocalObject
    {
    public:
      Current_ORBInitializer_Base (const ACE_TCHAR* id);
      virtual ~Current_ORBInitializer_Base (void);

      virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr);

       virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
    protected:
        virtual TAO::Transport::Current_ptr
        make_current_instance (TAO_ORB_Core* core, size_t tss_slot_id)
          = 0;
    protected:
      const ACE_TString id_;
#if defined (ACE_WIN32_VC14)
      // Workaround for connect issue 1577211
      ACE_UNIMPLEMENTED_FUNC (Current_ORBInitializer_Base (const Current_ORBInitializer_Base &))
      ACE_UNIMPLEMENTED_FUNC (Current_ORBInitializer_Base &operator = (const Current_ORBInitializer_Base &))
#endif
    };

  } /* namespace Transport */

} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* CURRENT_ORBINITIALIZER_BASE_H */

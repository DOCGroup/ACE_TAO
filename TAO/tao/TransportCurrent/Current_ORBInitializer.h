/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Current_ORBInitializer.h
 *
 *  @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================


#ifndef CURRENT_ORBINITIALIZER_H
#define CURRENT_ORBINITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/TransportCurrent/Current_ORBInitializer_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {
    template <typename Impl>
    class Current_ORBInitializer
      : public Current_ORBInitializer_Base
    {
    public:
      Current_ORBInitializer (const ACE_TCHAR* id);

    protected:
      virtual TAO::Transport::Current_ptr
      make_current_instance (TAO_ORB_Core* c, size_t s);
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include "tao/TransportCurrent/Current_ORBInitializer.cpp"

#include /**/ "ace/post.h"

#endif /* CURRENT_ORBINITIALIZER_H */

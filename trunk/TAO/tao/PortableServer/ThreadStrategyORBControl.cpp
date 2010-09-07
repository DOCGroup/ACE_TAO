// -*- C++ -*-
// $Id$

#include "tao/PortableServer/ThreadStrategyORBControl.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    int
    ThreadStrategyORBControl::enter ()
    {
      return 0;
    }

    int
    ThreadStrategyORBControl::exit ()
    {
      return 0;
    }

    ::PortableServer::ThreadPolicyValue
    ThreadStrategyORBControl::type () const
    {
      return ::PortableServer::ORB_CTRL_MODEL;
    }
  }
}



ACE_FACTORY_NAMESPACE_DEFINE (
  ACE_Local_Service,
  ThreadStrategyORBControl,
  TAO::Portable_Server::ThreadStrategyORBControl)

ACE_STATIC_SVC_DEFINE (
  ThreadStrategyORBControl,
  ACE_TEXT ("ThreadStrategyORBControl"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ThreadStrategyORBControl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)
TAO_END_VERSIONED_NAMESPACE_DECL

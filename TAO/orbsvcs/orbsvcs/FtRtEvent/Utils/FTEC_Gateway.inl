// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FTRTEC {
  ACE_INLINE RtecEventChannelAdmin::EventChannel_ptr
    FTEC_Gateway::_this(ACE_ENV_SINGLE_ARG_DECL)
  {
    PortableServer::POA_var poa = _default_POA();
    return activate(poa.in() ACE_ENV_ARG_PARAMETER);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

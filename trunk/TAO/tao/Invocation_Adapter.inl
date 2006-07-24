// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE
  Invocation_Adapter::Invocation_Adapter (
      CORBA::Object *target,
      Argument **args,
      int arg_number,
      const char *operation,
      size_t op_len,
      Collocation_Proxy_Broker *p,
      Invocation_Type type,
      Invocation_Mode mode)
    : target_ (target)
    , args_ (args)
    , number_args_  (arg_number)
    , operation_ (operation)
    , op_len_ (op_len)
    , cpb_ (p)
    , type_ (type)
    , mode_ (mode)
  {
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

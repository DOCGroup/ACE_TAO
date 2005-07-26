// -*- C++ -*-
// $Id$
namespace TAO
{
  ACE_INLINE
  Invocation_Adapter::Invocation_Adapter (
      CORBA::Object *target,
      Argument **args,
      int arg_number,
      const char *operation,
      int op_len,
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

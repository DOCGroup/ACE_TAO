// -*- C++ -*-
// $Id$

#include "tao/Valuetype/AbstractBase_Invocation_Adapter.h"
#include "tao/Valuetype/AbstractBase.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  AbstractBase_Invocation_Adapter::AbstractBase_Invocation_Adapter (
      CORBA::AbstractBase_ptr target,
      Argument **args,
      int arg_number,
      const char *operation,
      size_t op_len,
      Collocation_Proxy_Broker *p,
      Invocation_Type type,
      Invocation_Mode mode)
    : Invocation_Adapter (target->equivalent_objref (),
                          args,
                          arg_number,
                          operation,
                          op_len,
                          p,
                          type,
                          mode)
  {
  }
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

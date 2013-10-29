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
      int collocation_opportunity,
      Invocation_Type type,
      Invocation_Mode mode,
      bool has_in_args)
    : Invocation_Adapter (target->equivalent_objref (),
                          args,
                          arg_number,
                          operation,
                          op_len,
                          collocation_opportunity,
                          type,
                          mode,
                          has_in_args)
  {
  }
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

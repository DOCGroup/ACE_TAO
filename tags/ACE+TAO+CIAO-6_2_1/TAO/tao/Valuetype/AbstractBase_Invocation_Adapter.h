// -*- C++ -*-

//=============================================================================
/**
 *  @file    AbstractBase_Invocation_Adapter.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_ABSTRACTBASE_INVOCATION_ADAPTER_H
#define TAO_ABSTRACTBASE_INVOCATION_ADAPTER_H

#include /**/ "ace/pre.h"
#include "tao/Valuetype/valuetype_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Adapter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class AbstractBase;

  typedef AbstractBase *AbstractBase_ptr;
}
namespace TAO
{
  class Argument;

  class Collocation_Proxy_Broker;
  class Profile_Transport_Resolver;

  /**
   * @class AbstractBase_Invocation_Adapter
   *
   *
   */
  class TAO_Valuetype_Export AbstractBase_Invocation_Adapter
    : public Invocation_Adapter
  {
  public:
    AbstractBase_Invocation_Adapter (
        CORBA::AbstractBase_ptr target,
        Argument **args,
        int arg_number,
        const char *operation,
        size_t op_len,
        int collocation_opportunity,
        TAO::Invocation_Type type = TAO_TWOWAY_INVOCATION,
        TAO::Invocation_Mode mode = TAO_SYNCHRONOUS_INVOCATION);

  private:
    // Don't allow default initializations
    AbstractBase_Invocation_Adapter (void);

    // Disallow copying and assignment.
    AbstractBase_Invocation_Adapter (const AbstractBase_Invocation_Adapter &);
    AbstractBase_Invocation_Adapter & operator= (
        const AbstractBase_Invocation_Adapter &);

  };
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_INVOCATION_ADAPTER_H */

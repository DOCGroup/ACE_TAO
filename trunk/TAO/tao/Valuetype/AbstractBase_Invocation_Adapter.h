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
#include "valuetype_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Adapter.h"

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
        int op_len,
        Collocation_Proxy_Broker *cpb,
        TAO::Invocation_Type type = TAO_TWOWAY_INVOCATION,
        TAO::Invocation_Mode mode = TAO_SYNCHRONOUS_INVOCATION);

  private:
    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (AbstractBase_Invocation_Adapter (void));

    ACE_UNIMPLEMENTED_FUNC (AbstractBase_Invocation_Adapter & operator= (
        const AbstractBase_Invocation_Adapter &));
  };
} // End namespace TAO


#include /**/ "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/

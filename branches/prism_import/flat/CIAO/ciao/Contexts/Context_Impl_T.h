// -*- C++ -*-

//=============================================================================
/**
 *  @file    Context_Impl_T.h
 *
 *  $Id$
 *
 *  This file contains the declaration of a mixin base class for
 *  the generated context class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_CONTEXT_IMPL_T_H
#define CIAO_CONTEXT_IMPL_T_H

#include /**/ "ace/pre.h"

#include "Context_Impl_Base.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class SystemException;
}

namespace SecurityLevel2
{
  class Credentials;
}
TAO_END_VERSIONED_NAMESPACE_DECL

namespace Components
{
  typedef SecurityLevel2::Credentials Principal;
  typedef Principal *Principal_ptr;

  class IllegalState;

  namespace Transaction
  {
    class UserTransaction;
    typedef UserTransaction *UserTransaction_ptr;
  }
}

namespace CIAO
{
  class Container;
  typedef Container *Container_ptr;

  /**
   * @class Context_Impl
   *
   * @brief Mixin base class for generated context.
   *
   * This class implements operations and contains parameterized
   * members common to all generated servants.
   */
  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  class Context_Impl : public virtual BASE_CTX,
                       public virtual Context_Impl_Base,
                       public virtual TAO_Local_RefCounted_Object
  {
  public:
    Context_Impl (Components::CCMHome_ptr the_home,
                  Container_ptr c,
                  SVNT *sv);

    virtual ~Context_Impl (void);

    // Operations from ::Components::SessionContext.

    virtual CORBA::Object_ptr get_CCM_object ();

  protected:
    SVNT *servant_;
    typename COMP::_var_type component_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Context_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Context_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_CONTEXT_IMPL_T_H */

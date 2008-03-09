// -*- C++ -*-

//=============================================================================
/**
 *  @file    Context_Impl_Base.h
 *
 *  $Id$
 *
 *  This file contains the non-template declaration of a base class for
 *  the template mixin for the generated context class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_CONTEXT_IMPL_BASE_H
#define CIAO_CONTEXT_IMPL_BASE_H

#include /**/ "ace/pre.h"

#include "Context_Impl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ccm/CCM_ContainerC.h"

namespace Components
{
  class CCMHome;
  typedef CCMHome *CCMHome_ptr;

  typedef
    TAO_Objref_Var_T<
        CCMHome
      >
    CCMHome_var;
}

namespace CIAO
{
  class Session_Container;

  /**
   * @class Context_Impl_Base
   *
   * @brief Non-template base class for Context_Impl.
   *
   * Holds the non-template parts of its child class
   * Context_Impl.
   */
  class Context_Impl_Export Context_Impl_Base
    : public virtual Components::CCMContext
  {
  public:
    /// @todo This constructor should be declarated private but it seems
    /// the compilers want it, have to sort this out in detail.
    Context_Impl_Base (void);

    Context_Impl_Base (Components::CCMHome_ptr home, Session_Container * c);

    virtual ~Context_Impl_Base (void);

    // Operations from ::Components::CCMContext.

    virtual Components::Principal_ptr get_caller_principal ();

    virtual Components::CCMHome_ptr get_CCM_home ();

    virtual CORBA::Boolean get_rollback_only ();

    virtual Components::Transaction::UserTransaction_ptr get_user_transaction ();

    virtual CORBA::Boolean is_caller_in_role (const char *role);

    virtual void set_rollback_only ();

    virtual CORBA::Object_ptr resolve_service_reference(const char *service_id);

    // CIAO-specific.

    CIAO::Session_Container *_ciao_the_Container (void) const;

    // Accessors for the private member.
    const char *_ciao_instance_id (void) const;
    void _ciao_instance_id (const char *instance_id);

  protected:
    Components::CCMHome_var home_;
    Session_Container *container_;

  private:
    CORBA::String_var ciao_instance_id_;
  };
}

#if defined (__ACE_INLINE__)
# include "Context_Impl_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* CIAO_CONTEXT_IMPL_BASE_H */

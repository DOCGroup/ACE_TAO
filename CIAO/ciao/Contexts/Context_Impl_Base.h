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

#include "ccm/CCM_ContextC.h"

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
  class Container;
  typedef Container *Container_ptr;
  typedef TAO_Objref_Var_T<Container> Container_var;

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
    Context_Impl_Base (Components::CCMHome_ptr home,
                       Container_ptr c,
                       const char *instance_id);

    virtual ~Context_Impl_Base (void);

    // Operations from ::Components::CCMContext.

#if !defined (CCM_LW)
    virtual Components::Principal_ptr get_caller_principal ();
#endif

    virtual Components::CCMHome_ptr get_CCM_home ();

#if !defined (CCM_LW)
    virtual CORBA::Boolean get_rollback_only ();
#endif

#if !defined (CCM_LW)
    virtual Components::Transaction::UserTransaction_ptr
    get_user_transaction ();
#endif

#if !defined (CCM_LW)
    virtual CORBA::Boolean is_caller_in_role (const char *role);
#endif

#if !defined (CCM_LW)
    virtual void set_rollback_only ();
#endif

#if !defined (CCM_LW)
    virtual CORBA::Object_ptr resolve_service_reference(const char *service_id);
#endif

    /// CIAO-specific.
    CIAO::Container_ptr _ciao_the_Container (void) const;

    /// Accessors for the private member.
    const char *_ciao_instance_id (void) const;
    void _ciao_instance_id (const char *instance_id);
    void NO_IMPLEMENT();

  protected:
    Components::CCMHome_var home_;
    Container_var container_;

  private:
    /// Private constructor, not allowed to be called
    Context_Impl_Base (void);
    CORBA::String_var ciao_instance_id_;
  };
}

#if defined (__ACE_INLINE__)
# include "Context_Impl_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* CIAO_CONTEXT_IMPL_BASE_H */

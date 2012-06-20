// -*- C++ -*-

//=============================================================================
/**
 *  @file    Context_Impl_Base_T.h
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

#include "ccm/CCM_ContextC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * @class Context_Impl_Base_T
   *
   * @brief Template base class for Context_Impl.
   *
   * Holds the non-template parts of its child class
   * Context_Impl.
   */
  template <typename CONTAINER_TYPE>
  class Context_Impl_Base_T
    : public virtual Components::CCMContext
  {
  public:
    Context_Impl_Base_T (::Components::CCMHome_ptr home,
                         typename CONTAINER_TYPE::_ptr_type c,
                         const char *instance_id);

    virtual ~Context_Impl_Base_T (void);

    // Operations from ::Components::CCMContext.
#if !defined (CCM_LW)
    virtual ::Components::Principal_ptr get_caller_principal ();
#endif

    virtual ::Components::CCMHome_ptr get_CCM_home ();

#if !defined (CCM_LW)
    virtual ::CORBA::Boolean get_rollback_only ();
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

    virtual CORBA::Object_ptr resolve_service_reference(const char *service_id);

    virtual void
    install_service_component_reference (const char * service_id,
                               CORBA::Object_ptr objref);

    virtual CORBA::Object_ptr
    uninstall_service_component_reference (const char * service_id);


    /// Temporarily added back for a sponsor, this will be removed at some
    /// moment in favor of a resolve_service_reference() call
    const char *_ciao_instance_id (void) const;

  protected:
    typename CONTAINER_TYPE::_var_type container_;
  private:
    ::Components::CCMHome_var home_;
    ::CORBA::String_var ciao_instance_id_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ciao/Contexts/Context_Impl_Base_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ciao/Contexts/Context_Impl_Base_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_CONTEXT_IMPL_BASE_H */

// $Id$

#ifndef STOREREFERENCES_EXEC_H_
#define STOREREFERENCES_EXEC_H_


#include "StoreReferencesEC.h"

#include /**/ "StoreReferences_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_store_references_test_testComp_Impl
{
  class Component_exec_i
    : public virtual testComp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i  (void);
    virtual ~Component_exec_i  (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::store_references_test::CCM_testComp_Context_var context_;
  };

  extern "C" STOREREFERENCES_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void);

  class STOREREFERENCES_EXEC_Export Component_home_exec_i
      : public virtual testCompHome_Exec,
        public virtual ::CORBA::LocalObject
  {
  public:
      Component_home_exec_i (void);
      virtual ~Component_home_exec_i (void);
      virtual ::Components::EnterpriseComponent_ptr create (void);
  };

  extern "C" STOREREFERENCES_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_ComponentHome_Impl (void);

}

#endif /* ifndef */


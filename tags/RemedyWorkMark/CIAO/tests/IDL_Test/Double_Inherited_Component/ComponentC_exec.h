// -*- C++ -*-
// $Id$

#ifndef INHERITED_COMPONENTS_EXEC_H_
#define INHERITED_COMPONENTS_EXEC_H_


#include "Inherited_componentsEC.h"

#include /**/ "ComponentC_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_connector_test_C_Impl
{
  class ComponentC_exec_i
    : public virtual C_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    ComponentC_exec_i  (void);
    virtual ~ComponentC_exec_i  (void);

    virtual void
    topic_name_c (const char * topic_name);
    virtual char *
    topic_name_c (void);
    virtual void
    topic_name_b (const char * topic_name);
    virtual char *
    topic_name_b (void);
    virtual void
    topic_name_a (const char * topic_name);
    virtual char *
    topic_name_a (void);

    virtual ::connector_test::CCM_X_ptr
    get_x_a2 (void) { return 0; }

    virtual ::connector_test::CCM_Y_ptr
    get_y_b2 (void) { return 0; }

    virtual ::connector_test::CCM_Z_ptr
    get_z_c2 (void) { return 0; }

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::connector_test::CCM_C_Context_var context_;
    CORBA::String_var topic_name_c_;
    CORBA::String_var topic_name_b_;
    CORBA::String_var topic_name_a_;
    bool topic_name_c_has_been_set_;
    bool topic_name_b_has_been_set_;
    bool topic_name_a_has_been_set_;
  };

  extern "C" INHERITED_COMPONENTS_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ComponentC_Impl (void);
}

#endif /* ifndef */


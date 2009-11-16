// -*- C++ -*-
// $Id$

#ifndef HELLO_SENDER_EXEC_H_
#define HELLO_SENDER_EXEC_H_


#include "Inherited_componentsEC.h"

#include /**/ "ComponentC_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

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
    topic_name (const char * topic_name);
    virtual char *
    topic_name (void);
    
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
  
  private:
    ::connector_test::CCM_C_Context_var context_;
    CORBA::String_var topic_name_;
    bool topic_name_has_been_set_;
  };
  
  extern "C" INHERITED_COMPONENTS_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Sender_Impl (void);
}

#endif /* ifndef */


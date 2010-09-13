// -*- C++ -*-
// $Id$

#ifndef COMPONENTLINPASSAGE_EXEC_H_
#define COMPONENTLINPASSAGE_EXEC_H_


#include "CommandlinePassageEC.h"

#include /**/ "CommandlinePassage_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_command_line_test_A_Impl
{
  class Component_exec_i
    : public virtual A_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i  (void);
    virtual ~Component_exec_i  (void);

    virtual void
    tc_max (CORBA::Long tc_max);

    virtual CORBA::Long
    tc_max (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::command_line_test::CCM_A_Context_var context_;
    CORBA::Long tc_max_;
  };

  extern "C" COMMANDLINEPASSAGE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void);
}

#endif /* ifndef */


// -*- C++ -*-
// $Id$

#ifndef COMPONENTLINPASSAGE_EXEC_H_
#define COMPONENTLINPASSAGE_EXEC_H_


#include "CPUAffinityEC.h"

#include /**/ "CPUAffinity_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_cpuaffinity_A_Impl
{
  class Component_exec_i
    : public virtual A_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i  (void);
    virtual ~Component_exec_i  (void);

    virtual void
    cpu_affinity (CORBA::Long tc_max);

    virtual CORBA::Long
    cpu_affinity (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
  
  private:
    ::cpuaffinity::CCM_A_Context_var context_;
    CORBA::Long cpu_affinity_;
  };
  
  extern "C" COMMANDLINEPASSAGE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void);
}

#endif /* ifndef */


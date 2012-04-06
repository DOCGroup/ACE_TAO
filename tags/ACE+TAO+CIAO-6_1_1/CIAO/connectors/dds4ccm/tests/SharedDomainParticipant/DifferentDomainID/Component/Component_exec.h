// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_H_
#define CIAO_COMPONENT_EXEC_H_


#include "ComponentEC.h"

#include /**/ "Component_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

class DDSDomainParticipant;

namespace CIAO_SharedDP_SharedDPComponent_Impl
{
  class Component_exec_i
    : public virtual SharedDPComponent_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Component_exec_i (void);
    virtual ~Component_exec_i (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::SharedDP::CCM_SharedDPComponent_Context_var context_;

    DDSDomainParticipant *  dds_dp1_;
    DDSDomainParticipant *  dds_dp2_;
    DDSDomainParticipant *  dds_dp3_;
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void);
}

#endif /* ifndef */


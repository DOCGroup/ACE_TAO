// -*- C++ -*-
// $Id$

#ifndef CIAO_COMPONENT_EXEC_YFBMGA_H_
#define CIAO_COMPONENT_EXEC_YFBMGA_H_

#include /**/ "ace/pre.h"

#include "ComponentEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Component_exec_export.h"
#include "tao/LocalObject.h"

// #include "ccm/Extension/CCM_ServantContainerInterceptorC.h"
#include "ciao/Containers/Extension/ServantContainerInterceptorRegistration_Impl.h"

#include <map>

//============================================================
// ServantContainerInterceptor
//============================================================
class ServantContainerInterceptor
  : public ::Components::ContainerPortableInterceptor::ServantContainerInterceptor
{
public:
  ServantContainerInterceptor (CORBA::UShort priority,
                              const char * name);
  virtual ~ServantContainerInterceptor (void);

  virtual void
  servant_receive_request (
    ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr info,
    ::CORBA::Boolean_out proceed_call);

  virtual void
  servant_send_reply (
    ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr info,
    ::CORBA::Boolean_out proceed_call);

  virtual void
  servant_send_exception (
    ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr info,
    ::CORBA::Boolean_out proceed_call);

  virtual void
  servant_send_other (
    ::Components::ContainerPortableInterceptor::ContainerServantRequestInfo_ptr info);

  virtual char *
  name (void);

  virtual ::CORBA::UShort
  priority (void);

  virtual void
  priority (
    ::CORBA::UShort priority);

  virtual void
  destroy (void);

  virtual void
  set_slot_id (::PortableInterceptor::SlotId slot_id);

private:
  CORBA::UShort priority_;
  const char * name_;
};

namespace CIAO_Registration_Servant_Registration_Impl
{
  class COMPONENT_EXEC_Export Servant_Registration_exec_i
    : public virtual Servant_Registration_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Servant_Registration_exec_i (void);
    virtual ~Servant_Registration_exec_i (void);
    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */
    //@}
    //@{
    /** Operations from Components::ExtensionComponent. */
    virtual void set_extension_context (::Components::ExtensionContext_ptr ctx);
    virtual void ccm_remove (void);
    //@}
  private:
    ::Registration::CCM_Servant_Registration_Context_var ciao_context_;

    int test_registration(
      ::CIAO::ServantContainerInterceptorRegistration_Impl * pi_cr);
    int test_exception(
      ::CIAO::ServantContainerInterceptorRegistration_Impl * pi_cr);
    int test_priority(
      ::CIAO::ServantContainerInterceptorRegistration_Impl * pi_cr);
    int start_test (void);
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Registration_Servant_Registration_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

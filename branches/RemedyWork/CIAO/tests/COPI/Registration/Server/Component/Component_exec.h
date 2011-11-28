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

// #include "ccm/Extension/CCM_ServerContainerInterceptorC.h"
#include "ciao/Containers/Extension/ServerContainerInterceptorRegistration_Impl.h"

#include <map>

//============================================================
// ServerContainerInterceptor
//============================================================
class ServerContainerInterceptor
  : public ::Components::ContainerPortableInterceptor::ServerContainerInterceptor
{
public:
  ServerContainerInterceptor (CORBA::UShort priority,
                              const char * name);
  virtual ~ServerContainerInterceptor (void);

  virtual void
  receive_request_service_contexts (
    ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr info);

  virtual void
  receive_request (
    ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr info);

  virtual void
  send_reply (
    ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr info);

  virtual void
  send_exception (
    ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr info);

  virtual void
  send_other (
    ::Components::ContainerPortableInterceptor::ContainerServerRequestInfo_ptr info);

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

namespace CIAO_Registration_Server_Registration_Impl
{
  class COMPONENT_EXEC_Export Server_Registration_exec_i
    : public virtual Server_Registration_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Server_Registration_exec_i (void);
    virtual ~Server_Registration_exec_i (void);
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
    ::Registration::CCM_Server_Registration_Context_var ciao_context_;

    int test_registration(
      ::CIAO::ServerContainerInterceptorRegistration_Impl * pi_cr);
    int test_exception(
      ::CIAO::ServerContainerInterceptorRegistration_Impl * pi_cr);
    int test_priority(
      ::CIAO::ServerContainerInterceptorRegistration_Impl * pi_cr);
    int start_test (void);
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Registration_Server_Registration_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

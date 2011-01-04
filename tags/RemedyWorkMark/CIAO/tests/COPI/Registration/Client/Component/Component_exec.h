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

// #include "ccm/Extension/CCM_ClientContainerInterceptorC.h"
#include "ciao/Containers/Extension/ClientContainerInterceptorRegistration_Impl.h"

#include <map>

//============================================================
// ClientContainerInterceptor
//============================================================
class ClientContainerInterceptor
  : public ::Components::ContainerPortableInterceptor::ClientContainerInterceptor
{
public:
  ClientContainerInterceptor (CORBA::UShort priority,
                              const char * name);
  virtual ~ClientContainerInterceptor (void);

  virtual void
  send_request (
    ::Components::ContainerPortableInterceptor::ContainerClientRequestInfo_ptr info);

  virtual void
  send_poll (
    ::Components::ContainerPortableInterceptor::ContainerClientRequestInfo_ptr info);

  virtual void
  receive_reply (
    ::Components::ContainerPortableInterceptor::ContainerClientRequestInfo_ptr info);

  virtual void
  receive_exception (
    ::Components::ContainerPortableInterceptor::ContainerClientRequestInfo_ptr info);

  virtual void
  receive_other (
    ::Components::ContainerPortableInterceptor::ContainerClientRequestInfo_ptr info);

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

namespace CIAO_Registration_Client_Registration_Impl
{
  class COMPONENT_EXEC_Export Client_Registration_exec_i
    : public virtual Client_Registration_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Client_Registration_exec_i (void);
    virtual ~Client_Registration_exec_i (void);
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
    ::Registration::CCM_Client_Registration_Context_var ciao_context_;

    int test_registration(
      ::CIAO::ClientContainerInterceptorRegistration_Impl * pi_cr);
    int test_exception(
      ::CIAO::ClientContainerInterceptorRegistration_Impl * pi_cr);
    int test_priority(
      ::CIAO::ClientContainerInterceptorRegistration_Impl * pi_cr);
    int start_test (void);
  };

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Registration_Client_Registration_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

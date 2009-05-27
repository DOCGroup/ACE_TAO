#ifndef CIAO_BACKUP_GIZMO_LOW_EXEC_H
#define CIAO_BACKUP_GIZMO_LOW_EXEC_H

#include /**/ "ace/pre.h"

#include "Backup_Gizmo_Low_svnt.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Backup_Gizmo_Low_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Backup_Gizmo_Low
{
  class BACKUP_GIZMO_LOW_EXEC_Export Backup_Gizmo_Low_exec_i
    : public virtual Backup_Gizmo_Low_Exec,
      public virtual TAO_Local_RefCounted_Object,
      public virtual ACE_Event_Handler
  {
    public:
    Backup_Gizmo_Low_exec_i (void);

    virtual ~Backup_Gizmo_Low_exec_i (void);

    virtual int handle_timeout (const ACE_Time_Value &,
                                const void *ref);


    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ciao_preactivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    configuration_complete (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ccm_activate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ccm_passivate (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    virtual void
    ccm_remove (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));

    protected:
    Backup_Gizmo_Low_Context *context_;
  };

  class BACKUP_GIZMO_LOW_EXEC_Export Backup_Gizmo_Low_Factory_exec_i
    : public virtual Backup_Gizmo_Low_Factory_Exec,
      public virtual TAO_Local_RefCounted_Object
  {
    public:
    Backup_Gizmo_Low_Factory_exec_i (void);
    virtual ~Backup_Gizmo_Low_Factory_exec_i (void);

    // Supported or inherited operations.

    // Home operations.

    // Factory and finder operations.

    // Attribute operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
                      ::CORBA::SystemException,
                      ::Components::CCMException));
  };

  extern "C" BACKUP_GIZMO_LOW_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_SPACE_Backup_Gizmo_Low_Factory_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* CIAO_BACKUP_GIZMO_LOW_EXEC_H */

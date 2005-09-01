//$Id$
//============================================================
/**
 * @file Sender_exec_1.h
 *
 * Header file for the Executor implementation.
 */
//============================================================

#ifndef SENDER_EXEC_1_H
#define SENDER_EXEC_1_H

#include "SwapExecC.h"
#include "SenderEC.h"
#include "Sender_exec_1_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Sender_Impl
{
  class SenderSwap_exec_i;


  class SENDER_EXEC_1_Export Sender_exec_1_i :
      public virtual Sender_Exec,
      public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    Sender_exec_1_i () {};

    /// Secondary construction.
    Sender_exec_1_i (const char* local_message,
                     SenderSwap_exec_i *e)
      : message_ (CORBA::string_dup (local_message))
      , base_exec_ (e)
    {
    }

    /// Default destructor.
    virtual ~Sender_exec_1_i ();

    /// Operation to set the value of the attribute
    virtual void local_message (const char * local_message
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Operation to get the value of the attribute
    virtual char * local_message (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /* Operations for obtaining the interface reference. */
    /* This method will be used in the assembly face so the
     * ObjRef of this read_message facet will be sent to the
     * client side(receptacle).
     */
    // Note: You can specify the return type as ::Hello::CCM_message *
    virtual Hello::CCM_ReadMessage_ptr
    get_push_message (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));


    // Operation inside of the trigger interface.
    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    void swap_exec (SenderSwap_exec_i *p)
    {
      this->base_exec_ = p;
    }

  protected:
    /// Copmponent specific context
    Sender_Exec_Context_var context_;

  private:
    CORBA::String_var message_;

    // my incarnator
    SenderSwap_exec_i *base_exec_;

    friend class Message_Impl_1;
  };

  class Message_Impl_1 : public virtual Hello::CCM_ReadMessage,
                       public virtual TAO_Local_RefCounted_Object
  {
  public:
    Message_Impl_1 (Sender_exec_1_i& component)
        : component_ (component)
    {
    }

    virtual char *
    get_message (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    Sender_exec_1_i& component_;
  };

  extern "C" SENDER_EXEC_1_Export ::Components::EnterpriseComponent_ptr
  createSenderExec_Impl (SenderSwap_exec_i *p);
}

#endif /* SENDER_EXEC_H */

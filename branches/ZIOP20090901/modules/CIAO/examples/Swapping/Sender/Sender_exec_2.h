//$Id$
//============================================================
/**
 * @file Sender_exec_2.h
 *
 * Header file for the Executor implementation.
 */
//============================================================

#ifndef SENDER_EXEC_2_H
#define SENDER_EXEC_2_H

#include "ciao/CIAO_SwapExecC.h"
#include "SenderEC.h"
#include "Sender_exec_2_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Sender_Impl
{
  class SenderSwap_exec_i;

  class SENDER_EXEC_2_Export Sender_exec_2_i :
      public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {

  public:
    /// Default constructor.
    Sender_exec_2_i ()
        : message_(CORBA::string_dup ("Default Message"))
    {
    }

    /// Secondary construction.
    Sender_exec_2_i (const char* local_message,
                     SenderSwap_exec_i *e)
      : message_ (CORBA::string_dup (local_message))
      , base_exec_ (e)
    {
    }

    /// Default destructor.
    virtual ~Sender_exec_2_i ();

    /// Operation to set the value of the attribute
    virtual void local_message (const char * local_message);

    /// Operation to get the value of the attribute
    virtual char * local_message ();

    /* Operations for obtaining the interface reference. */
    /* This method will be used in the assembly face so the
     * ObjRef of this read_message facet will be sent to the
     * client side(receptacle).
     */
    // Note: You can specify the return type as ::Hello::CCM_message *
    virtual Hello::CCM_ReadMessage_ptr
    get_push_message ();


    // Operation inside of the trigger interface.
    virtual void start ();

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx);

    virtual void ciao_preactivate ();

    virtual void ccm_activate ();

    virtual void ciao_postactivate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

    void swap_exec (SenderSwap_exec_i *p)
    {
      this->base_exec_ = p;
    }

  protected:
    /// Copmponent specific context
    Sender_Exec_Context_var context_;

  private:
    CORBA::String_var message_;

    SenderSwap_exec_i *base_exec_;

    friend class Message_Impl_2;
  };

  class Message_Impl_2 : public virtual Hello::CCM_ReadMessage,
                       public virtual ::CORBA::LocalObject
  {
  public:
    Message_Impl_2 (Sender_exec_2_i& component)
        : component_ (component)
    {
    }

    virtual char *
    get_message ();

  private:
    Sender_exec_2_i& component_;
  };

  extern "C" SENDER_EXEC_2_Export ::Components::EnterpriseComponent_ptr
  createSenderExec_Impl (SenderSwap_exec_i *p);
}

#endif /* SENDER_EXEC_H */

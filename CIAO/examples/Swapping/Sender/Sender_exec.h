//$Id$
//============================================================
/**
 * @file Sender_exec.h
 *
 * Header file for the Executor implementation.
 */
//============================================================

#ifndef SENDER_EXEC_H
#define SENDER_EXEC_H

#include "ciao/Servants/Swapping/CIAO_SwapExecC.h"
#include "SenderEC.h"
#include "Sender_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/DLL.h"
#include "ciao/CCM_EventsC.h"

namespace CIDL_Sender_Impl
{
  class SENDER_EXEC_Export Sender_exec_i :
      public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {

  public:
    /// Default constructor.
    Sender_exec_i ()
        : message_(CORBA::string_dup ("Default Message"))
    {
    }

    /// Secondary construction.
    Sender_exec_i (const char* local_message)
      : message_ (CORBA::string_dup (local_message))
    {
    }

    /// Default destructor.
    virtual ~Sender_exec_i ();

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

    virtual void configuration_complete ();

    virtual void ccm_activate ();

    virtual void ccm_passivate ();

    virtual void ccm_remove ();

  protected:
    /// Copmponent specific context
    Sender_Exec_Context_var context_;

  private:
    CORBA::String_var message_;

    friend class Message_Impl;
  };


  //
  //
  //
  class Message_Impl : public virtual Hello::CCM_ReadMessage,
                       public virtual ::CORBA::LocalObject
  {
  public:
    Message_Impl (Sender_exec_i& component)
        : component_ (component)
    {
    }

    virtual char *
    get_message ();

  private:
    Sender_exec_i& component_;
  };

  class SenderSwap_exec_i;


  typedef ::Components::EnterpriseComponent_ptr (*ExecFactory) (SenderSwap_exec_i *);

  /**
   * @class Sender_exec_i
   *
   * Sender executor implementation class.
   */

  class SENDER_EXEC_Export SenderSwap_exec_i :
      public virtual CIAO::Swap_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    SenderSwap_exec_i ();

    ~SenderSwap_exec_i ();

    virtual ::Components::EnterpriseComponent_ptr
    incarnate ();

    virtual ::Components::EnterpriseComponent_ptr
    etherealize ();

    void  consumers (::Components::ConsumerDescriptions *p)
    {
      this->consumers_ = p;
    }

    ::Components::ConsumerDescriptions *consumers (void)
    {
      return this->consumers_._retn ();
    }

  protected:
    int count_;

    ::Components::ConsumerDescriptions_var consumers_;
  };

  class SENDER_EXEC_Export SenderHome_exec_i :
    public virtual SenderHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    SenderHome_exec_i ();

    /// Default dtor.
    virtual ~SenderHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ();


  };

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void);
}

#endif /* SENDER_EXEC_H */



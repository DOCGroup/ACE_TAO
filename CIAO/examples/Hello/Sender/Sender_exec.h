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

#include "SenderEC.h"
#include "Sender_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_Sender_Impl
{
  /**
   * @class Sender_exec_i
   *
   * Sender executor implementation class.
   */

  class SENDER_EXEC_Export Sender_exec_i :
      public virtual Sender_Exec,
      public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    Sender_exec_i ()
      : message_(CORBA::string_dup ("Default Message")),
        color_ (::Hello::empty)
    {
    }

    /// Secondary construction.
    Sender_exec_i (const char* local_message)
      : message_ (CORBA::string_dup (local_message)),
        color_ (::Hello::empty)
    {
    }

    /// Default destructor.
    virtual ~Sender_exec_i ();

    /// Operation to set the value of the attribute "local_message"
    virtual void local_message (const char * local_message)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Operation to get the value of the attribute "local_message"
    virtual char * local_message ()
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Operation to set the value of the attribute "hertz"
    virtual void hertz (CORBA::Long hertz)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Operation to get the value of the attribute "hertz"
    virtual CORBA::Long hertz ()
      ACE_THROW_SPEC ((CORBA::SystemException));

    /* Operations for obtaining the interface reference. */
    /* This method will be used in the assembly face so the
     * ObjRef of this read_message facet will be sent to the
     * client side(receptacle).
     */
    // Note: You can specify the return type as ::Hello::CCM_message *
    virtual Hello::CCM_ReadMessage_ptr
    get_push_message ()
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Hello::COLOR_SELECTION
    color ()
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    color (::Hello::COLOR_SELECTION)
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Operation inside of the trigger interface.
    virtual void start ()
        ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_preactivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_activate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_passivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:
    /// Copmponent specific context
    Sender_Exec_Context_var context_;

  private:
    CORBA::String_var message_;

    CORBA::Long hertz_;

    ::Hello::COLOR_SELECTION color_;

    friend class Message_Impl;
  };


  //
  //
  //
  class Message_Impl : public virtual Hello::CCM_ReadMessage,
                       public virtual TAO_Local_RefCounted_Object
  {
  public:
    Message_Impl (Sender_exec_i& component)
        : component_ (component)
    {
    }

    virtual char *
    get_message ()
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    Sender_exec_i& component_;
  };

  /**
   * @class SenderHome_exec_i
   *
   * Sender home executor implementation class.
   */
  class SENDER_EXEC_Export SenderHome_exec_i :
    public virtual SenderHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    SenderHome_exec_i ();

    /// Default dtor.
    virtual ~SenderHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void);
}

#endif /* SENDER_EXEC_H */



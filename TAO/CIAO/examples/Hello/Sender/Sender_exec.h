//$Id$
//============================================================
/**
 * @file Sender_exec.h
 *
 * Header file for the Executor implementation.
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 */
//============================================================

#ifndef SENDER_EXEC_H
#define SENDER_EXEC_H

#include "SenderEIC.h"

#include "tao/LocalObject.h"

namespace MyImpl
{
  /**
   * @class Sender_exec_i
   *
   * Sender executor implementation class.
   */

  class SENDER_EXEC_Export Sender_exec_i :
      public virtual Hello::CCM_Sender,
      public virtual Component::SessionComponent,
      public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    Sender_exec_i ()
        : message_("Default Message")
    {
    }

    /// Secondary construction.
    Sender_exec_i (const char* local_message)
        : message_ (local_message)
    {
    }

    /// Default destructor.
    virtual ~Sender_exec_i ();

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
    virtual Hello::CCM_message_ptr
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

    virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:
    /// Copmponent specific context
    Hello::CCM_Sender_Context_var context_;

  private:
    CORBA::String_var message_;

    friend class message_impl;
  };


  //
  //
  //
  class message_impl : public virtual Hello::CCM_message,
                       public virtual TAO_Local_RefCounted_Object
  {
  public:
    message_impl (Sender_exec_i& component)
        : component_ (component)
    {
    }

    virtual char *
    get_message (ACE_ENV_SINGLE_ARG_DECL)
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
    public virtual Hello::CCM_SenderHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    SenderHome_exec_i ();

    /// Default dtor.
    virtual ~SenderHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void);

#endif /* SENDER_EXEC_H */

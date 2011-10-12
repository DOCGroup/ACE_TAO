// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_0HSO8P_H_
#define CIAO_RECEIVER_EXEC_0HSO8P_H_

#include /**/ "ace/pre.h"

#include "ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Receiver_exec_export.h"
#include "tao/LocalObject.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Receiver_Impl
{
  // Forward declaration
  class Receiver_exec_i;

  /**
   * HelloGenerator
   */
  class HelloGenerator :
     public ACE_Event_Handler
  {
  public:
    HelloGenerator (Receiver_exec_i &callback);

    //Callback from the reactor.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    Receiver_exec_i &generator_;
  };

  /**
   * Component Executor Implementation Class: Receiver_exec_i
   */

  class Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    /** @name Supported operations and attributes. */
    //@{
    //@}

    /** @name Component attributes and port operations. */
    //@{
   //@}

    /** @name Operations from Components::SessionComponent. */
    //@{
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    /** @name User defined public operations. */
    //@{
    void tick (void);
    //@}

  private:
    ::CCM_Receiver_Context_var ciao_context_;

    /** @name Component attributes. */

    /** @name User defined members. */
    //@{
    //@}

    /** @name User defined private operations. */
    //@{
    HelloGenerator * hello_generator_;
    ACE_Time_Value interval_;
    ACE_Reactor* reactor (void);
    //@}
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Receiver_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

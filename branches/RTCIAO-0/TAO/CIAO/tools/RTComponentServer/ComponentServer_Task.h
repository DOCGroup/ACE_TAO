// -*- C++ -*-

//=============================================================================
/**
 *  @file    ComponentServer_Task.h
 *
 *  $Id$
 *
 *  An ACE_Task subclass that manages the RT-ORB thread for CIAO's RT
 *  ComponentServer.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_COMPONENTSERVER_TASK_H
#define CIAO_COMPONENTSERVER_TASK_H
#include "ace/pre.h"

#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

namespace CIAO
{
  /**
   * @class ComponentServer_task
   *
   * @brief An ACE_Task subclass that manages the RT-ORB therad
   */
  class ComponentServer_Task
    : public virtual ACE_Task_Base
  {
  public:
    // We can add a rt-task configuration facility here.
    typedef struct _options
    {
      // default ctor.
      _options () : use_callback_ (1) {}

     // The name of the file to write stringified IOR to.
      ACE_CString ior_output_filename_;

      // Stringified IOR of a CIAO's callback object.
      ACE_CString callback_ior_;

      // CIAO ComponentServer uses the callback object to pass it's
      // own object reference back to ServerActivator.
      int use_callback_;
    } Options;

    ComponentServer_Task (ACE_Thread_Manager &thread_manager,
                          CORBA::ORB_ptr orb,
                          Options &opts);

    int svc (void);

  protected:
    CORBA::ORB_var orb_;

    // ComponentServer config options.
    Options options_;
  };

}

#if defined (__ACE_INLINE__)
# include "ComponentServer_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_COMPONENTSERVER_TASK_H */

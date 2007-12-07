// -*- C++ -*-

//=============================================================================
/**
 *  @file    Receiver_Task.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 *
 */
//=============================================================================

#ifndef _DDS_TEST_RECEIVER_TASK_H_
#define _DDS_TEST_RECEIVER_TASK_H_

#include "DDSReceiver_svnt.h"
#include <dds/DCPS/Service_Participant.h>

#include <ace/Task.h>
#include <string>
#include <map>

/**
 * @class Receiver_Task
 * @author Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 * @brief This class checks for DDS messages
 */

namespace CIDL_DDSReceiverImpl
{
  class Receiver_Task : public ACE_Task_Base
  {
  public:
    /// constructor
    Receiver_Task ();

    /// initializes the context member
    void set_context (DDSReceiver_Context* context);

    /// exectution function of the thread
    virtual int svc ();
  
    /// indicates, that the execution of the thread should stop
    void stop ();
    
  private:
    /// flag for thread cancellation
    bool stopped_;

    /// domain participant factory
    DDS::DomainParticipantFactory_var dp_factory_;

    /// domain participant
    DDS::DomainParticipant_var participant_;

    /// component context
    DDSReceiver_Context* context_;
  };

}

#endif /* _DDS_TEST_RECEIVER_TASK_H_ */

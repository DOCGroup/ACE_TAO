/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    BPR_Drivers.h
//
// = DESCRIPTION
//    This code builds abstractions to factor out common code from
//    the different possible implementations of the Timer_Queue based
//    bounded packet relay example.
//
// = AUTHORS
//    Chris Gill           <cdgill@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu>
//
//    Based on the Timer Queue Test example written by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#if !defined (_BPR_DRIVERS_H_)
#define _BPR_DRIVERS_H_

#include "ace/Task.h"

// forward declarations
class Input_Device_Wrapper_Base;
class Output_Device_Wrapper_Base;

class Command_Base
{
  // = TITLE
  //    Defines an abstract class that allows us to invoke commands
  //    without knowing anything about the implementation.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a command
  //    independent of the effect of the command, or the objects used
  //    to implement it.
public:
  virtual int execute (void *arg) = 0;
  // Invokes the method <action_> from the object <receiver_>.
};

class Bounded_Packet_Relay_Base
{
  // = TITLE
  //     Base class for the time-bounded packet relay class.
  //
  // = DESCRIPTION
  //     This enum must go here to avoid confusing certain broken C++
  //     compilers.
public:
  // = Enumerates possible status values for a transmission.
  enum Transmission_Status 
  {
    UN_INITIALIZED,
    STARTED,
    COMPLETED,
    TIMED_OUT,
    CANCELLED,
    ERROR_DETECTED
  };
};

class Input_Device_Wrapper_Base : public ACE_Task_Base
{
  // = TITLE
  //    This class defines an abstract base class for an input device
  //    wrapper that hides the details of the specific device and
  //    provides a consistent message passing interface without
  //    knowing anything about the implementation of the input device
  //    or the message receiver.
  //
  //    The abstract base class ctor takes a command template object
  //    that is instantiated with the correct receiver and action
  //    types. This command object is used to send newly created input
  //    messages to the receiver.
  //
  //    The abstract base class is designed to operate in an active
  //    "push" mode, sending input data to the receiver whenever the
  //    data is ready.  The underlying device may be active, notifying
  //    the wrapper when data is ready, or may be passive in which
  //    case the wrapper must rely on a reactive and/or polling
  //    mechanism.
  //
  // = DESCRIPTION
  //    Derived classes are responsible for filling in concrete
  //    definitions for the abstract message creation method and the
  //    svc method.
public:
  // = Initialization and termination methods.
  Input_Device_Wrapper_Base (ACE_Thread_Manager *input_task_mgr);
  // Constructor.

  virtual ~Input_Device_Wrapper_Base ();
  // Destructor.
 
  int set_send_input_msg_cmd (Command_Base *send_input_msg_cmd);
  // Sets send input message command in the input device driver
  // object.

  int set_input_period (u_long input_period);
  // Sets period (in usecs) between when inputs are created.

  int set_send_count (long count);
  // Sets count of messages to send.

  int request_stop (void);
  // Requests that the input device stop sending messages and
  // terminate its thread.  Should return 1 if it will do so, 0 if it
  // has already done so, or -1 if there is a problem doing so.

  virtual int svc (void);
  // This method runs the input device loop in the new thread.

protected:
  virtual ACE_Message_Block *create_input_message (void) = 0;
  // Creates a new message block, carrying data read from the
  // underlying input device.

  virtual int send_input_message (ACE_Message_Block *);
  // Sends a newly created message block, carrying data read from the
  // underlying input device, by passing a pointer to the message
  // block to its command execution.

  Command_Base *send_input_msg_cmd_;
  // Send newly created input message.

  u_long input_period_;
  // Period between when input values are produced (usecs).

  ACE_Reactor reactor_;
  // Reactor used to multiplex input streams, timeouts.

  int is_active_;
  // Flag to indicate whether or not input object is 
  // (and should remain) active.

  long send_count_;
  // Count of messages to send before stopping (-1 indicates the
  // device should not stop).

  long current_count_;
  // Currently remaining count of messages to send before stopping 
  // (-1 indicates the device should not stop).

};

class Output_Device_Wrapper_Base
{
  // = TITLE
  //    This class defines an abstract base class for an output device
  //    wrapper that hides the details of the specific device and
  //    provides a consistent write method interface without knowing
  //    anything about the implementation.
  //
  // = DESCRIPTION
  //    The abstract methods write_output_message () and
  //    modify_device_settings () are defined in derived classes to
  //    write the contents of the passed message out the underlying
  //    output device, and update device settings, respectively.
public:
  virtual int write_output_message (void *) = 0;
  // Writes contents of the passed message block out to the underlying
  // output device.

  virtual int modify_device_settings (void *) = 0;
  // Provides an abstract interface to allow modifying device
  // settings.
};

// include the templates
#include "BPR_Drivers_T.h"

#endif /* _BPR_DRIVERS_H_ */

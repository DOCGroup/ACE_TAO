// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Wait_Strategy.h
//
// = DESCRIPTION
//     Classes to strategize waiting for reply.
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

// @@ Alex: don't forget to protect your files against multiple
// inclusion:

#ifndef TAO_WAIT_STRATEGY_H
#define TAO_WAIT_STRATEGY_H

// @@ Alex: why do you need this #include?
#include "tao/GIOP.h"

class TAO_Request_Mux_Strategy;
class TAO_Transport;

class TAO_Export TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Strategy for waiting for the reply.
  //
  // = DESCRIPTION
  //

public:
  TAO_Wait_Strategy (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Wait_Strategy (void);
  // Destructor.

  virtual int wait (void) = 0;
  // Base class virtual method.

  virtual int handle_input (void) = 0;
  // Handle the input.

  // @@ Alex: this class should *not* depend on the IIOP_Handlers,
  //    can't you use TAO_Transport for this? After all it returns an
  //    Event_Handler if you need one...

  virtual int register_handler (void) = 0;
  // Register the handler with the Reactor if it makes sense for the
  // strategy.

  virtual int resume_handler (ACE_Reactor *reactor) = 0;
  // Depending on the wait strategy resume the connect handler.

protected:
  TAO_Transport *transport_;
  // Transport object.
};

// @@ Alex: we should consider moving these classes to separate files,
//    that can minimize the footprint of systems that use only one of
//    the strategies....

class TAO_Export TAO_Wait_On_Reactor : public TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Wait on the Reactor. Happens in s Single Threaded client
  //    environment.
  //
  // = DESCRIPTION
  //

public:
  TAO_Wait_On_Reactor (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Wait_On_Reactor (void);
  // Destructor.

  virtual int wait (void);
  // Do the event loop of the Reactor.

  virtual int handle_input (void);
  // Handle the input. Delegate this job to Transport object. Before
  // that suspend the handler in the Reactor.

  // @@ Alex: this class should *not* depend on the IIOP_Handlers,
  //    can't you use TAO_Transport for this? After all it returns an
  //    Event_Handler if you need one...

  virtual int register_handler (void);
  // Register the handler with the Reactor.

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the connection handler.

private:
  int reply_received_;
  // This flag indicates if a *complete* reply has been received. Used 
  // to exit the event loop.
};

class TAO_Export TAO_Wait_On_Leader_Follower : public TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Wait according to the Leader-Follower model. Leader does the
  //    event loop of the Reactor and the Followers wait on the
  //    condition variable.
  //
  // = DESCRIPTION
  //

public:
  TAO_Wait_On_Leader_Follower (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Wait_On_Leader_Follower (void);
  // Destructor.

  virtual int wait (void);
  // Wait according to the L-F model.

  virtual int handle_input (void);
  // Handle the input. Delegate this job to Transport object. Before
  // that, suspend the handler in the Reactor.

  // @@ Alex: another use of IIOP_Handler...

  virtual int register_handler (void);
  // Register the handler with the Reactor.

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the connection handler.

  virtual int send_request (TAO_ORB_Core* orb_core,
                            TAO_OutputCDR &stream);
  // Send the request in <stream>.

protected:
  ACE_SYNCH_CONDITION* cond_response_available (void);
  // Return the cond_response_available, initializing it if necessary.

  ACE_thread_t calling_thread_;
  // the thread ID of the thread we were running in.

  ACE_SYNCH_CONDITION* cond_response_available_;
  // wait on reponse if the leader-follower model is active.

  int expecting_response_;
  // State flag which, if non-zero, indicates that we were expecting
  // respose. Otherwise, any input received is unexpected.
  // @@ Do we need this anymore? (Alex).

  int reply_received_;
  // This flag indicates if a *complete* reply was received. Until
  // that happens we block on the leader/follower condition variable
  // or the reactor event loop.
};

class TAO_Export TAO_Wait_On_Read :  public TAO_Wait_Strategy
{
  // = TITLE
  //
  //    Wait on receiving the reply.
  //
  // = DESCRIPTION
  //

public:
  TAO_Wait_On_Read (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Wait_On_Read (void);
  // Destructor.

  virtual int wait (void);
  // Wait on the read operation.

  virtual int handle_input (void);
  // Handle the input. Delegate this job to Transport object.

  // @@ Alex: another use of IIOP_Handler...

  virtual int register_handler (void);
  // No-op. Return 0.

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the connection handler. No-op. Returns 0.
};

#endif /* TAO_WAIT_STRATEGY_H */

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


#include "tao/GIOP.h"

class TAO_Request_Mux_Strategy;
class TAO_Transport;
class TAO_IIOP_Handler_Base;

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

  virtual int register_handler (TAO_IIOP_Handler_Base *handler) = 0;
  // Register the handler with the Reactor if it makes sense for the
  // strategy.

  virtual int resume_handler (ACE_Reactor *reactor) = 0;
  // Depending on the wait strategy resume the connect handler. 
  
protected:
  TAO_Transport *transport_;
  // Transport object.
};

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

  virtual int register_handler (TAO_IIOP_Handler_Base *handler);
  // Register the handler with the Reactor.
  
  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the connection handler. 
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

  virtual int register_handler (TAO_IIOP_Handler_Base *handler);
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

  int input_available_;
  // Flag indicating whether or not input is available.  Only valid
  // when <expecting_response_> is non-zero.
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

  virtual int register_handler (TAO_IIOP_Handler_Base *handler);
  // No-op. Return 0.
  
  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the connection handler. No-op. Returns 0.
};

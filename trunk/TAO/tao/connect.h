// This may look like C, but it's really -*- C++ -*-

#if !defined (TAO_CONNECT_H)
#  define TAO_CONNECT_H

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    connect.h
//
// = AUTHOR
//     Chris Cleeland
// 
// ============================================================================

#  include "ace/Reactor.h"
#  include "ace/Acceptor.h"
#  include "ace/SOCK_Acceptor.h"
#  include "ace/Synch.h"
#  include "ace/Svc_Handler.h"

// Forward Decls
class TAO_OA_Parameters;

// @@ Is this really an *OA* connection handler anymore?  Seems like
// connections are really associated with ORBs in the POA-based
// architecture.
class TAO_OA_Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    Handles requests on a single connection in a server.
  //
{
public:
  TAO_OA_Connection_Handler (ACE_Thread_Manager *t = ACE_Thread_Manager::instance ());
  // Constructor.
  
  virtual int open (void *);
  // Called by the framework when the handler is completely set up.
  // Argument is unused.
  
  virtual int svc (void);
  // Only used when the handler is turned into an active object by
  // calling <activate>.  This serves as the event loop in such cases.

  TAO_OA_Parameters *params (void);
  void params (TAO_OA_Parameters *p);

  // = Template Methods Called by <handle_input>

  virtual int read_message (CDR &msg, CORBA::Environment &env);
  // Extract a message from the stream associated with <peer()> and
  // place it into <msg>.  Return 0 if success, -1 with <errno> and
  // <env> set if problems.

  virtual int handle_message (CDR &msg, int &response_required,
                              CDR &response, CORBA::Environment &env);
  // Handle processing of the request residing in <msg>, setting
  // <response_required> to zero if the request is for a oneway or
  // non-zero if for a two-way and <response> to any necessary
  // response (including errors).  In case of errors, -1 is returned
  // and additional information carried in <env>.

  virtual void send_response (CDR &response);
  // @@ Please document me...

protected:
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

private:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> SUPERCLASS;

  TAO_OA_Parameters *params_;
};

typedef ACE_Strategy_Acceptor<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR> 
	TAO_ACCEPTOR;

#endif /* TAO_CONNECT_H */

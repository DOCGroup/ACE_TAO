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

#if 0
#  include "ace/Reactor.h"
#  include "ace/Acceptor.h"
#  include "ace/SOCK_Acceptor.h"
#  include "ace/Synch.h"
#  include "ace/Svc_Handler.h"
#endif

// Forward Decls
class TAO_OA_Parameters;

class TAO_OA_Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //    Handles requests on a single connection in a server.
  //
  // = NOTES
  //    Should probably be named something like TAO_OA_Connection_Handler.
{
public:
  // @@ Please add comments.
  TAO_OA_Connection_Handler (ACE_Thread_Manager *t = ACE_Thread_Manager::instance ());
  virtual int open (void *);
  virtual int svc (void);

  TAO_OA_Parameters *params (void);
  void params (TAO_OA_Parameters *p);

protected:
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

private:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> SUPERCLASS;

  TAO_OA_Parameters *params_;
};

typedef ACE_Strategy_Acceptor<TAO_OA_Connection_Handler, ACE_SOCK_ACCEPTOR> ROA_Acceptor;

#endif /* TAO_CONNECT_H */

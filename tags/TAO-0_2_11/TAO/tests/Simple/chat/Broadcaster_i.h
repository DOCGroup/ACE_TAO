// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Broadcaster_i.h
//
// = DESCRIPTION
//    Defines the implementation header for the Broadcaster interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#if !defined (BROADCASTER_I_H)
#define BROADCASTER_I_H

#include "BroadcasterS.h"
#include "ReceiverC.h"
#include "tao/TAO.h"
#include "ace/Containers.h"
#include "ace/SString.h"

class Broadcaster_i : public POA_Broadcaster
{
  // = TITLE
  //    The implementation of the Broadcaster class which is the servant
  //    object for the chat server.

public:
  Broadcaster_i (void);
  // Constructor

  ~Broadcaster_i (void);
  // Destructor

  virtual CORBA::Long add (Receiver_ptr receiver,
                           const char * nickname, 
                           CORBA::Environment &_tao_environment);
  // saves receiver references in a list.

  virtual CORBA::Long remove (Receiver_ptr receiver, 
                              CORBA::Environment &_tao_environment);
  // removes receiver references from the list.

  virtual void say (const char * text,
		    CORBA::Environment &_tao_environment);
  // called by Broadcaster clients to send messages.

private:
  TAO_ORB_Manager orb_manager_;
  // the ORB manager.

  struct Receiver_Data
  {
    Receiver_var receiver_;
    ACE_CString nick_;
  };
  // per client info

  typedef ACE_Unbounded_Set<Receiver_Data> RECEIVER_SET;
  typedef ACE_Unbounded_Set_Iterator<Receiver_Data> RECEIVER_SET_ITERATOR;

  RECEIVER_SET receiver_set_;
  // set of registered clients.
};

#endif /* BROADCASTER_I_H */

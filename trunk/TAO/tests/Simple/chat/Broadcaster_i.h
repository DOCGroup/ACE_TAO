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

class Broadcaster_i : public POA_Broadcaster
{
  // = TITLE
  //    The implementation of the Broadcaster class which is the servant
  //    object for the chat server.

 public:
  Broadcaster_i();
  // Constructor

  ~Broadcaster_i();
  // Destructor

  virtual CORBA::Long _cxx_register (Receiver_ptr receiver,
				     const char * nickname, CORBA::Environment &_tao_environment);
  // saves receiver references in a list.

  virtual CORBA::Long un_register (Receiver_ptr receiver, CORBA::Environment &_tao_environment);
  // removes receiver references from the list.

  virtual void say (const char * text,
		    CORBA::Environment &_tao_environment);
  // called by Broadcaster clients to send messages.

 private:
  TAO_ORB_Manager orb_manager_;
  // the ORB manager.

  struct Receiver_Data
  {
    Receiver_ptr receiver_ptr_;
    char* nick_;
  };
  // per client info

  typedef ACE_Unbounded_Set<Receiver_Data*> Receiver_Set;
  typedef ACE_Unbounded_Set_Iterator<Receiver_Data*> Receiver_Set_Iterator;

  Receiver_Set receiver_set_;
  //set of registered clients.
};

#endif /* BROADCASTER_I_H */

// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Broadcaster_i.cpp
//
// = DESCRIPTION
//   Implementation of the Broadcaster_i class. This class is the servant
//   object for the chat server.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Broadcaster_i.h"

Broadcaster_i::Broadcaster_i (void)
{
}

Broadcaster_i::~Broadcaster_i (void)
{
}

void
Broadcaster_i::add (Receiver_ptr receiver,
                    CORBA::Environment &environment)
{
  // store the client information.
  Receiver_var receiver = Receiver::_duplicate (receiver);

  // insert the Receiver reference to the set
  if (receiver_set_.insert (receiver) == -1)
   {
     // Raise exception
     environment.exception (new Broadcaster::CannotAdd (/* reason */));
   }
}

void
Broadcaster_i::remove (Receiver_ptr receiver,
                       CORBA::Environment &environment)
{
  
}

void 
Broadcaster_i::say (const char *text,
		    CORBA::Environment &environment)
{
  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      // Broadcast the message to all registered clients
      for (RECEIVER_SET_ITERATOR iter = this->receiver_set_.begin ();
           iter != this->receiver_set_.done ();
           iter++)
        {
          (*iter)->message (msg, TAO_TRY_ENV);      
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Broadcaster_i::say\t\n");
    }
      
  TAO_ENDTRY;
}

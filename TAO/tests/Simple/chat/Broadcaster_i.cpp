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

Broadcaster_i::Broadcaster_i()
{
}

Broadcaster_i::~Broadcaster_i()
{
  // clean up the receiver_set_
  Receiver_Set_Iterator iter(receiver_set_);

  Receiver_Data **receiver_data_ptr;

  if (iter.first() == 1)
  while (iter.done() == 0)
   {
     iter.next(receiver_data_ptr);

     delete (*receiver_data_ptr)->nick_;

     iter.advance();
   }
}

CORBA::Long Broadcaster_i:: _cxx_register (Receiver_ptr receiver,
				     const char * nickname, CORBA::Environment &_tao_environment)
{
  // check if the set is full.
  if (receiver_set_.is_full() == 1 )
    {
      return -1;
    }

  Receiver_Data *receiver_data_ptr;

  // create a Receiver_Data object.
  ACE_NEW_RETURN(receiver_data_ptr, Receiver_Data(), -1);

  // store the client information.
  receiver_data_ptr->receiver_ptr_ = Receiver::_duplicate(receiver);
  receiver_data_ptr->nick_ = CORBA::string_dup (nickname);

  // insert the Receiver reference to the set
  if (receiver_set_.insert(receiver_data_ptr) == -1)
   {
     return -1;
   }

   return 0;
}

CORBA::Long Broadcaster_i::un_register (Receiver_ptr receiver,
					CORBA::Environment &_tao_environment)
{
  // NOT implemented yet..
  //
  return -1;
}

void Broadcaster_i::say (const char * text,
		    CORBA::Environment &_tao_environment)
{
  // broadcast the message to all registered clients
  Receiver_Set_Iterator iter(receiver_set_);

  Receiver_Data **receiver_data_ptr ;

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      if (iter.first() == 1)
      while (iter.done() == 0)
      {
	iter.next(receiver_data_ptr);

	char *msg = CORBA::string_dup(text);
        //char *nick = (*receiver_data_ptr)->nick_;

	//(*receiver_data_ptr)->receiver_ptr_->message (nick, TAO_TRY_ENV);
	(*receiver_data_ptr)->receiver_ptr_->message (msg, TAO_TRY_ENV);

	iter.advance();
      }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Broadcaster_i::say\t\n");
    }

  TAO_ENDTRY;
}

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

int
Broadcaster_i::Receiver_Data::operator == (const Receiver_Data &receiver_data) const
{
  // The <_is_equivalent> function checks if the _var and _ptr objects
  // are the same.  NOTE: this call might not behave well on other
  // ORBs since <_is_equivalent> isn't guaranteed to differentiate
  // object references.
  return this->receiver_.in()->_is_equivalent (receiver_data.receiver_)
    && this->nickname_ == receiver_data.nickname_;
}

Broadcaster_i::Broadcaster_i (void)
{
}

Broadcaster_i::~Broadcaster_i (void)
{
}

void
Broadcaster_i::add (Receiver_ptr receiver,
		    const char *nickname,
                    CORBA::Environment &environment)
{
  // @@ Please check and make sure that you fully qualify the
  // Receiver_Data class with Broadcaster_i.
  Broadcaster_i::Receiver_Data receiver_data;

  // Store the client information.
  receiver_data.receiver_ = Receiver::_duplicate (receiver);
  receiver_data.nickname_ = nickname;

  // Insert the Receiver reference to the set
  if (receiver_set_.insert (receiver_data) == -1)
    // Raise exception
    environment.exception (new Broadcaster::CannotAdd
			   ("failed to add to the receiver set\n"));
  // Tell everyone which person just joined the chat.
  ACE_CString broadcast_string =
    ACE_CString ("**** ")
    + ACE_CString (nickname)
    + ACE_CString (" has joined the chat ****\n");

  this->broadcast (broadcast_string.fast_rep (),
		   environment);
}

void
Broadcaster_i::remove (Receiver_ptr receiver,
                       CORBA::Environment &environment)
{
  Receiver_Data receiver_data_to_remove;

  for (RECEIVER_SET_ITERATOR iter = this->receiver_set_.begin ();
       iter != this->receiver_set_.end ();
       iter++)
    {
      // @@ Please put the comment about "non-portability" here, as
      // well!
      if ((*iter).receiver_.in ()->_is_equivalent (receiver) == 1)
	{
	  receiver_data_to_remove = *iter;
	  break;
	}
    }

  // Remove the reference from our list.

  if (this->receiver_set_.remove (receiver_data_to_remove) == -1)
    // Raise exception.
    environment.exception(new Broadcaster::CannotRemove
			  ("failed to remove from receiver set\n"));

  // Tell everyone, which person left the chat.
  ACE_CString broadcast_string = "**** "
    + receiver_data_to_remove.nickname_
    + " left the chat"
    + " ****\n";

  this->broadcast (broadcast_string.fast_rep (),
		   environment);
}

void
Broadcaster_i::say (Receiver_ptr receiver,
		    const char *text,
		    CORBA::Environment &TAO_TRY_ENV)
{
  TAO_TRY
    {
      ACE_CString sender_nickname ("Sender Unknown");

      // Find the nickname for this receiver.

      for (RECEIVER_SET_ITERATOR iter = this->receiver_set_.begin ();
           iter != this->receiver_set_.end ();
           iter++)
        {
	  // @@ Please add comment here....
          if ((*iter).receiver_.in ()->_is_equivalent (receiver) == 1)
	    {
	      sender_nickname = (*iter).nickname_;
	    }
        }

      // Broadcast the message to all registered clients
      ACE_CString broadcast_string ("[" + sender_nickname + "] " + text);

      // @@ Please be consistent in your use of env vs. environment.
      // In fact, I recommend you use the TAO macro TAO_TRY_ENV.
      this->broadcast (broadcast_string.fast_rep (),
		       env);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Broadcaster_i::say\t\n");
    }
  TAO_ENDTRY;
}

void
Broadcaster_i::broadcast (const char *text,
			  CORBA::Environment &_tao_environment)
{
  // Broadcast the message to all registered clients.

  for (RECEIVER_SET_ITERATOR iter = this->receiver_set_.begin ();
       iter != this->receiver_set_.end ();
       iter++)
    {
      // @@ You need to put in an exception handling check here for the
      // case where something fails.  I'm not sure what you should do,
      // but you should probably print a message indicating which
      // exception occurred and then keep processing the other
      // receivers.
      (*iter).receiver_->message (text,
				  _tao_environment);
    }
}

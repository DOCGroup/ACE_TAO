// $Id$

#include "RMCast_Reassembly.h"
#include "RMCast_Partial_Message.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "RMCast_Reassembly.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Reassembly, "$Id$")

ACE_RMCast_Reassembly::
ACE_RMCast_Reassembly (void)
  :  ACE_RMCast_Module ()
{
}

ACE_RMCast_Reassembly::~ACE_RMCast_Reassembly (void)
{
  /*!<
     We cleanup the resources in the destructor
     <B color=red>@@ TODO</B> Why not in the close() operation?
  */
  for (Message_Map_Iterator i = this->messages_.begin ();
       i != this->messages_.end ();
       ++i)
    {
      ACE_RMCast_Partial_Message *message = (*i).int_id_;
      if (message != 0)
        delete message;
    }
  this->messages_.unbind_all ();
}

int
ACE_RMCast_Reassembly::data (ACE_RMCast::Data &data)
{
  if (this->next () == 0)
    return 0;

  if (data.payload->length () + data.fragment_offset > data.total_size)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "RMCast_Reassembly::data - invalid size\n"));
      return -1; // Corrupt message?
    }

  ACE_RMCast_Partial_Message *message;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (this->messages_.find (data.sequence_number, message) == -1)
      {
        ACE_NEW_RETURN (message,
                        ACE_RMCast_Partial_Message (data.total_size),
                        -1);

        if (this->messages_.bind (data.sequence_number,
                                  message) == -1)
          return -1; // Internal error?
      }

    // The message was in the collection, but it has been received
    // already, this is a duplicate fragment, just drop it.
    if (message == 0)
      return 0;

    if (message->fragment_received (data.total_size,
                                    data.fragment_offset,
                                    data.payload) == -1)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Error in fragment_received\n"));
        return -1;
      }

    if (!message->is_complete ())
      return 0;

    // Remove the message from the collection, but leave a marker
    // to indicate that it was already received...
    if (this->messages_.rebind (data.sequence_number,
                                (ACE_RMCast_Partial_Message*)0) == -1)
      return -1;
  }

  // Push the message...
  ACE_RMCast::Data downstream_data;
  downstream_data.source = data.source;
  downstream_data.sequence_number = data.sequence_number;
  downstream_data.total_size = message->message_body ()->length ();
  downstream_data.fragment_offset = 0;
  downstream_data.payload = message->message_body ();

  int r = this->next ()->data (downstream_data);

  delete message;

  return r;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >;
template class ACE_Hash_Map_Entry<ACE_UINT32,ACE_RMCast_Partial_Message*>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

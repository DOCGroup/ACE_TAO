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
  (void) this->close_i ();
}

/**
 *
 * We cleanup the resources in the destructor
 */
int
ACE_RMCast_Reassembly::close ()
{
  this->close_i ();
  return this->ACE_RMCast_Module::close ();
}

int
ACE_RMCast_Reassembly::data (ACE_RMCast::Data &data)
{
  if (this->next () == 0)
    return 0;

  // ACE_DEBUG ((LM_DEBUG,
  //             "Reassembly::data - %d,%d,%d\n",
  //             data.sequence_number,
  //             data.total_size,
  //             data.fragment_offset));

  if (data.payload->length () + data.fragment_offset > data.total_size)
    {
      ACE_ERROR ((LM_ERROR,
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

        // ACE_DEBUG ((LM_DEBUG,
        //             "Reassembly::data - new message\n"));
        if (this->messages_.bind (data.sequence_number,
                                  message) == -1)
          return -1; // Internal error?
      }

    if (message->fragment_received (data.total_size,
                                    data.fragment_offset,
                                    data.payload) == -1)
      {
        // ACE_DEBUG ((LM_DEBUG,
        //             "Error in fragment_received\n"));
        return -1;
      }

    if (!message->is_complete ())
      {
        // ACE_DEBUG ((LM_DEBUG,
        //             "Reassembly::data - message still incomplete\n"));
        return 0;
      }

    if (this->messages_.unbind (data.sequence_number) == -1)
      {
        // ACE_DEBUG ((LM_DEBUG,
        //            "Reassembly::data - message now complete\n"));
        return -1;
      }
  }

  // Push the message...
  ACE_RMCast::Data downstream_data;
  downstream_data.source = data.source;
  downstream_data.sequence_number = data.sequence_number;
  downstream_data.total_size = ACE_static_cast(
                                    ACE_UINT32,
                                    message->message_body ()->length ());
  downstream_data.fragment_offset = 0;
  downstream_data.payload = message->message_body ();

  int r = this->next ()->data (downstream_data);

  delete message;

  return r;
}

void
ACE_RMCast_Reassembly::close_i (void)
{
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >;
template class ACE_Hash_Map_Entry<ACE_UINT32,ACE_RMCast_Partial_Message*>;

template class ACE_Less_Than<ACE_UINT32>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Hash<ACE_UINT32>,ACE_Equal_To<ACE_UINT32>,ACE_Null_Mutex >
#pragma instantiate ACE_Hash_Map_Entry<ACE_UINT32,ACE_RMCast_Partial_Message*>

#pragma instantiate ACE_Less_Than<ACE_UINT32>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


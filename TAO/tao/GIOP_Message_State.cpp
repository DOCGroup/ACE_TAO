// -*- C++ -*-

//$Id$

#include "tao/GIOP_Message_State.h"
#include "tao/GIOP_Utils.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_State.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Message_State, "$Id$")

TAO_GIOP_Message_State::TAO_GIOP_Message_State (TAO_ORB_Core* orb_core)
  : byte_order (TAO_ENCAP_BYTE_ORDER),
    more_fragments (0),
    message_type (TAO_GIOP_MESSAGERROR),
    message_size (0),
    request_id (0),
    // Problem similar to GIOP_Message_handler.cpp - Bala
    fragmented_messages (ACE_CDR::DEFAULT_BUFSIZE)
{
  //giop_version.major = TAO_DEF_GIOP_MAJOR;
  //giop_version.minor = TAO_DEF_GIOP_MINOR;
}

TAO_GIOP_Message_State::~TAO_GIOP_Message_State (void)
{
  // @@ Bala: this is not a very useful comment, is it?
  //no-op
}

int
TAO_GIOP_Message_State::is_complete (ACE_Message_Block &current_buf)
{
  if (this->more_fragments)
    {
      if (this->fragmented_messages.length () == 0)
        {
          this->first_fragment_byte_order = this->byte_order;
          this->first_fragment_giop_version = this->giop_version;
          this->first_fragment_message_type = this->message_type;
          // this->fragments_end = this->fragments_begin = current;
          this->fragmented_messages.copy (current_buf.rd_ptr (),
                                          current_buf.length ());

          // Reset the buffer
          current_buf.reset ();

          // Reset our state
          this->reset ();
          return 0;
        }

      return this->append_fragment (current_buf);
    }

  if (this->fragmented_messages.length () != 0)
    {
      // This is the last message, but we must defragment before
      // sending
      if (this->append_fragment (current_buf) == -1)
        return -1;

      // Copy the entire message block into <current_buf>
      current_buf.data_block (this->fragmented_messages.data_block ()->clone ());

      this->fragmented_messages.reset ();

      this->byte_order = this->first_fragment_byte_order;
      this->giop_version = this->first_fragment_giop_version;
      this->message_type = this->first_fragment_message_type;

      // This message has no more fragments, and there where no fragments
      // before it, just return. Notice that current_buf has the
      // *right* contents
    }


  return 1;
}

int
TAO_GIOP_Message_State::append_fragment (ACE_Message_Block& current)
{
  if (this->first_fragment_byte_order != this->byte_order
      || this->first_fragment_giop_version.major != this->giop_version.major
      || this->first_fragment_giop_version.minor != this->giop_version.minor)
    {
      // Yes, print it out in all debug levels!. This is an error by
      // CORBA 2.4 spec
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) incompatible fragments:\n")
                  ACE_TEXT ("   Different GIOP versions or byte order\n")));
      this->reset ();
      return -1;
    }

  size_t req_size =
    this->fragmented_messages.size () + current.length ();

  this->fragmented_messages.size (req_size);

  // Copy the message
  this->fragmented_messages.copy (current.rd_ptr (),
                                  current.length ());

  current.reset ();

  // Reset our state
  this->reset ();

  return 0;
}

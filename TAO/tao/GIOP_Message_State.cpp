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
    current_offset (0),
    cdr (orb_core->create_input_cdr_data_block (ACE_CDR::DEFAULT_BUFSIZE),
         TAO_ENCAP_BYTE_ORDER,
         orb_core),
    fragments_begin (0),
    fragments_end (0)
{
  //giop_version.major = TAO_DEF_GIOP_MAJOR;
  //giop_version.minor = TAO_DEF_GIOP_MINOR;
}

TAO_GIOP_Message_State::~TAO_GIOP_Message_State (void)
{
  //no-op
}

int
TAO_GIOP_Message_State::is_complete ()
{
  if (this->message_size != this->current_offset)
    return 0;

  if (this->more_fragments)
    {
      // This is only one fragment of the complete Request....
      ACE_Message_Block* current =
        this->cdr.steal_contents ();
      if (this->fragments_begin == 0)
        {
          this->first_fragment_byte_order = this->byte_order;
          this->first_fragment_giop_version = this->giop_version;
          this->first_fragment_message_type = this->message_type;
          this->fragments_end = this->fragments_begin = current;
          this->reset ();
          return 0;
        }

      return this->append_fragment (current);
    }

  if (this->fragments_begin != 0)
    {
      // This is the last message, but we must defragment before
      // sending

      ACE_Message_Block* current =
        this->cdr.steal_contents ();
      if (this->append_fragment (current) == -1)
        return -1;

      // Copy the entire chain into the input CDR.....
      this->cdr.reset (this->fragments_begin,
                       this->first_fragment_byte_order);
      ACE_Message_Block::release (this->fragments_begin);
      this->fragments_begin = 0;
      this->fragments_end = 0;
      
      this->byte_order = this->first_fragment_byte_order;
      this->giop_version = this->first_fragment_giop_version;
      this->message_type = this->first_fragment_message_type;

      /*FALLTHROUGH*/
    }
  // else
  // {
  // This message has no more fragments, and there where no fragments
  // before it, just return... notice that this->cdr has the right
  // contents.
  // }

  return 1;
}

int
TAO_GIOP_Message_State::append_fragment (ACE_Message_Block* current)
{
  this->fragments_end->cont (current);
  this->fragments_end = this->fragments_end->cont ();

  if (this->first_fragment_byte_order != this->byte_order
      || this->first_fragment_giop_version.major != this->giop_version.major
      || this->first_fragment_giop_version.minor != this->giop_version.minor)
    {
      // Yes, print it out in all debug levels!
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) incompatible fragments:\n"
                  "   Different GIOP versions or byte order\n"));
      this->reset ();
      return -1;
    }
  this->reset ();
  return 0;
}

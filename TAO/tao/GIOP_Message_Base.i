// -*- C++ -*-
//$Id$

//
// GIOP_Message_Base
//
ACE_INLINE const size_t
TAO_GIOP_Message_Base::header_len (void)
{
  return TAO_GIOP_HEADER_LEN;
}

ACE_INLINE const size_t
TAO_GIOP_Message_Base::message_size_offset (void)
{
  return TAO_GIOP_MESSAGE_SIZE_OFFSET;
}

ACE_INLINE const size_t
TAO_GIOP_Message_Base::major_version_offset (void)
{
  return TAO_GIOP_VERSION_MAJOR_OFFSET;
}

ACE_INLINE const size_t
TAO_GIOP_Message_Base::minor_version_offset (void)
{
  return TAO_GIOP_VERSION_MINOR_OFFSET;
}

ACE_INLINE const size_t
TAO_GIOP_Message_Base::flags_offset (void)
{
  return TAO_GIOP_MESSAGE_FLAGS_OFFSET;
}

ACE_INLINE const size_t
TAO_GIOP_Message_Base::message_type_offset (void)
{
  return TAO_GIOP_MESSAGE_TYPE_OFFSET;
}


ACE_INLINE int
TAO_GIOP_Message_Base::parse_magic_bytes (
  TAO_GIOP_Message_State  *state) 
{
  // Grab the read pointer
  char *buf = state->cdr.rd_ptr ();
  
  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      // For the present...
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) bad header, magic word [%c%c%c%c]\n"), 
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }

  if (this->validate_version (state) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%N|%l|%p|%t) Error in validating")
                      ASYS_TEXT ("revision \n")));
          return -1;
        }
    }
  
  return 0;
}

ACE_INLINE int
TAO_GIOP_Message_Base::parse_header (TAO_GIOP_Message_State *state)
{
  char *buf = state->cdr.rd_ptr ();

  // Let us be specific that it is for 1.0
  if (this->minor_version () == 0 &&
      this->major_version () == 1)
    {
      state->byte_order = buf[this->flags_offset ()];
      if (TAO_debug_level > 2
          && state->byte_order != 0 && state->byte_order != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO (%P|%t) invalid byte order <%d>")
                      ASYS_TEXT (" for version <1.0>\n"),
                      state->byte_order));
          return -1;
        }
    }
  else
    {
      state->byte_order     =
        (CORBA::Octet) (buf[this->flags_offset ()]& 0x01);
      state->more_fragments =
        (CORBA::Octet) (buf[this->flags_offset ()]& 0x02);

      if (TAO_debug_level > 2
          && (buf[this->flags_offset ()] & ~0x3) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO (%P|%t) invalid flags for <%d>")
                      ASYS_TEXT (" for version <%d %d> \n"),
                      buf[this->flags_offset ()],
                      this->major_version (),
                      this->minor_version ()));
          return -1;
        }
    }

  // Get the message type
  state->message_type = buf[this->message_type_offset ()];

  // Reset our input CDR stream
  state->cdr.reset_byte_order (state->byte_order);


  state->cdr.skip_bytes (this->message_size_offset ());
  state->cdr.read_ulong (state->message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n"), 
                  this->major_version (),
                  this->minor_version (),
                  state->byte_order,
                  state->message_type,
                  state->message_size));
    }

  return 1;
}

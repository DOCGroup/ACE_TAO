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

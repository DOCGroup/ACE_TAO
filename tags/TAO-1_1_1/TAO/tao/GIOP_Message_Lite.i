//$Id$


ACE_INLINE int
TAO_GIOP_Message_Lite::parse_header (TAO_GIOP_Message_State *state)
{
  // Get the read pointer
  char *buf = state->cdr.rd_ptr ();

  // @@ Bala: i added the following comment, does it make sense?
  // In GIOPLite the version, byte order info, etc. are hardcoded, and
  // not transmitted over the wire.
  state->byte_order = TAO_ENCAP_BYTE_ORDER;
  state->giop_version.major = TAO_DEF_GIOP_MAJOR;
  state->giop_version.minor = TAO_DEF_GIOP_MINOR;

  // Get the message type.
  state->message_type = buf[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];

  state->cdr.reset_byte_order (state->byte_order);
  // The first bytes are the length of the message.
  state->cdr.read_ulong (state->message_size);

  return 0;
}



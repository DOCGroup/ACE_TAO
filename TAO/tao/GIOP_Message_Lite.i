//$Id$



ACE_INLINE int
TAO_GIOP_Message_Lite::parse_header (TAO_GIOP_Message_State *state)
{
  // Get the read pointer
  char *buf = state->cdr.rd_ptr ();
  
  state->byte_order = TAO_ENCAP_BYTE_ORDER;
  state->giop_version.major = TAO_DEF_GIOP_MAJOR;
  state->giop_version.minor = TAO_DEF_GIOP_MINOR;
  state->message_type = buf[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];
  
  state->cdr.reset_byte_order (state->byte_order);
  state->cdr.read_ulong (state->message_size);

  return 0;
}

ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_Lite::
  make_reply (CORBA::ULong request_id,
              TAO_OutputCDR &output)
{
  // Write the GIOP header first
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_REPLY,
                               output);
  
  // create and write a dummy context
  // We dont really need this. But for some reason the reply is not
  // parsable on the receiving side?
  // @@Bala, look in to this
  IOP::ServiceContextList resp_ctx;
  resp_ctx.length (0);
  output << resp_ctx;

  // Write the request ID
  output.write_ulong (request_id);
  
  return 0;
}

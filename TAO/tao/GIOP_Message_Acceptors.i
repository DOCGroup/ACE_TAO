// -*- C++ -*-
//$Id$

ACE_INLINE
TAO_GIOP_Message_Acceptors::
  TAO_GIOP_Message_Acceptors (TAO_ORB_Core *orb_core)
    : output_ (this->repbuf_,
               sizeof this->repbuf_,
               TAO_ENCAP_BYTE_ORDER,
               orb_core->output_cdr_buffer_allocator (),
               orb_core->output_cdr_dblock_allocator (),
               orb_core->orb_params ()->cdr_memcpy_tradeoff (),
               orb_core->to_iso8859 (),
               orb_core->to_unicode ())
      
{
#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (this->repbuf_,
                         '\0',
                         sizeof this->repbuf_);
#endif /* ACE_HAS_PURIFY */
}


ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_Acceptors::
  make_reply (CORBA::ULong request_id,
              TAO_OutputCDR &output)
{
  // Write the GIOP header first
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_REPLY,
                               output);

  // Write the reply header
  this->accept_state_->write_reply_header (output,
                                           request_id);
  
  return 0;
                               
}

ACE_INLINE int
TAO_GIOP_Message_Acceptors::
  make_send_locate_reply (TAO_Transport *transport,
                          TAO_GIOP_Locate_Request_Header &request,
                          TAO_GIOP_Locate_Status_Msg &status_info)
{
  // Note here we are making the Locate reply header which is *QUITE*
  // different from the reply header made by the make_reply () call.. 
  // Make the GIOP message header
  this->write_protocol_header (TAO_PLUGGABLE_MESSAGE_LOCATEREPLY,
                               this->output_);             

  // This writes the header & body
  this->accept_state_->write_locate_reply_mesg (this->output_,
                                                request.request_id (),
                                                status_info);

  // Send the message
  int result = this->send_message (transport,
                                   this->output_);
                   
  // Print out message if there is an error
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ASYS_TEXT ("TAO_GIOP::process_server_message")));
        }
    }   
  
  return result;
}


ACE_INLINE void
TAO_GIOP_Message_Acceptors::
set_state (CORBA::Octet def_major,
           CORBA::Octet def_minor)
{
  // @@Bala Need to find a better way
  switch (def_major)
    {
    case 1:
      switch (def_minor)
        {
        case 0:
          this->accept_state_ = &this->implementations_.version_10;
          break;
        case 1:
          this->accept_state_ = &this->implementations_.version_11;
          break;
        default:
          break;
        }
      break;
    default:
      break;
    }
}




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


ACE_INLINE
TAO_GIOP_Message_Acceptors::
  ~TAO_GIOP_Message_Acceptors (void)
{
  this->output_.reset ();
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




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

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Acceptors::major_version (void)
{
  return this->accept_state_->major_version ();
}

ACE_INLINE CORBA::Octet
TAO_GIOP_Message_Acceptors::minor_version (void)
{
  return this->accept_state_->minor_version ();
}

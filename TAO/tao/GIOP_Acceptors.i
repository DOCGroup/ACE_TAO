//$Id$
ACE_INLINE 
TAO_GIOP_Acceptor_1_1::TAO_GIOP_Acceptor_1_1 (void)
  : our_major_version_ (1),
    our_minor_version_ (1)
{
  // no-op
}

ACE_INLINE const CORBA::Octet&
TAO_GIOP_Acceptor_1_1::major_version (void)
{
  
  return this->our_major_version_;
}

ACE_INLINE const CORBA::Octet&
TAO_GIOP_Acceptor_1_1::minor_version (void)
{
  return this->our_minor_version_;
}

ACE_INLINE size_t 
TAO_GIOP_Acceptor_1_1::header_length (void)
{
  return TAO_GIOP_HEADER_LEN;
}

ACE_INLINE size_t 
TAO_GIOP_Acceptor_1_1::offset_length (void)
{
  return TAO_GIOP_MESSAGE_SIZE_OFFSET;
}

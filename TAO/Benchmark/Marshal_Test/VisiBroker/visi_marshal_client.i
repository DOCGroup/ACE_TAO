
ACE_INLINE
Visi_Marshal_Client_Proxy::Visi_Marshal_Client_Proxy (void)
  : orb_ (CORBA_ORB::_nil ()),
    ref_ (0)
{
}

ACE_INLINE
Visi_Marshal_Client_Proxy::~Visi_Marshal_Client_Proxy (void)
{
  // releasing order is important

  // release the reference
  CORBA::release (ref_);

  // release the ORB
  CORBA::release (orb_);
}



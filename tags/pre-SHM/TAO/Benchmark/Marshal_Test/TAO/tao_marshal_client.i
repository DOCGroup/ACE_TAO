// $Id$


ACE_INLINE
Tao_Marshal_Client_Proxy::Tao_Marshal_Client_Proxy (void)
  : orb_ (0),
    ref_ (0)
{
}

ACE_INLINE
Tao_Marshal_Client_Proxy::~Tao_Marshal_Client_Proxy (void)
{
  // releasing order is important

  // release the reference
  CORBA_release (ref_);

  // release the ORB
  CORBA_release (orb_);
}



// $Id$

// constructor
ACE_INLINE
Visi_Marshal_Server_Proxy::Visi_Marshal_Server_Proxy (void)
  : orb_ (0),
    boa_ (0),
    ssi_ (0),
    dsi_ (0)
{
}

// destructor
ACE_INLINE
Visi_Marshal_Server_Proxy::~Visi_Marshal_Server_Proxy (void)
{
  // releasing order is important

  // release the impl
  delete ssi_;
  delete dsi_;

  // release the BOA
  CORBA::release (boa_);

  // release the ORB
  CORBA::release (orb_);
}

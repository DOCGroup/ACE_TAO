// $Id$

// constructor
ACE_INLINE
Marshal_SSI_Impl::Marshal_SSI_Impl (const char *obj_name)
  : MarshalBOAImpl (obj_name)
{
}

// destructor
ACE_INLINE
Marshal_SSI_Impl::~Marshal_SSI_Impl (void)
{
}

// constructor
ACE_INLINE
Marshal_DSI_Impl::Marshal_DSI_Impl (CORBA::ORB_ptr orb, const char *obj_name)
  : orb_ (orb)
{
}

// destructor
ACE_INLINE
Marshal_DSI_Impl::~Marshal_DSI_Impl (void)
{
  orb_ = CORBA::ORB::_nil ();
}


// constructor
ACE_INLINE
Marshal_SSI_Impl::Marshal_SSI_Impl (const char *obj_name)
  : _skel_Marshal (obj_name)
{
}

// destructor
ACE_INLINE
Marshal_SSI_Impl::~Marshal_SSI_Impl (void)
{
}

// destructor
ACE_INLINE
Marshal_DSI_Impl::~Marshal_DSI_Impl (void)
{
  orb_ = 0;//CORBA::ORB::_nil ();
}



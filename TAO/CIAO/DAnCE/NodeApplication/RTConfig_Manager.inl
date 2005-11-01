// $Id$

ACE_INLINE
CIAO::RTResource_Config_Manager::RTResource_Config_Manager (RTCORBA::RTORB_ptr rtorb)
  : rtorb_ (RTCORBA::RTORB::_duplicate (rtorb))
{
}

ACE_INLINE
CIAO::RTResource_Config_Manager::~RTResource_Config_Manager ()
{
  this->fini ();
}


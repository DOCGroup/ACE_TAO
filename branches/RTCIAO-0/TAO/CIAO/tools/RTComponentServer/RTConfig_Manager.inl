// $Id$

ACE_INLINE
CIAO::RTResource_Config_Manager::RTResource_Config_Manager (RTCORBA::RTORB_ptr orb)
  : rtorb_ (RTCORBA::RTORB::_duplicate (orb))
{
}

ACE_INLINE
CIAO::RTResource_Config_Manager::~RTResource_Config_Manager ()
{
}

ACE_INLINE
CIAO::RTPolicy_Set_Manager::RTPolicy_Set_Manager
(CIAO::RTResource_Config_Manager &resource_config,
 CORBA::ORB_ptr orb,
 RTCORBA::RTORB_ptr rtorb)
  : resource_manager_ (resource_config),
    orb_ (CORBA::ORB::_duplicate (orb)),
    rtorb_ (RTCORBA::RTORB::_duplicate (rtorb))

{
}

CIAO::RTPolicy_Set_Manager::~RTPolicy_Set_Manager ()
{
}

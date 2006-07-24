/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::RTServer::RTNodeApplication_Impl::RTNodeApplication_Impl (CORBA::ORB_ptr o,
                                                                RTCORBA::RTORB_ptr rto,
                                                                PortableServer::POA_ptr p)
  : NodeApplication_Impl (o, p),
    rtorb_ (RTCORBA::RTORB::_duplicate (rto))
{
}



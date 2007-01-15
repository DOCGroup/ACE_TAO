// $Id$

ACE_INLINE
Callback_i::Callback_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

ACE_INLINE
Middle_i::Middle_i (CORBA::ORB_ptr orb,
                    Simple_Server_ptr server)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server::_duplicate (server))
{
}

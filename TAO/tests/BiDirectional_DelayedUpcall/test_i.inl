ACE_INLINE
Callback_i::Callback_i (CORBA::ORB_ptr orb, Simple_Server_ptr srv)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     server_ (Simple_Server::_duplicate (srv))
{
}

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb,
                                  int no_iterations)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     callback_ (0),
     no_iterations_ (no_iterations)
{
}

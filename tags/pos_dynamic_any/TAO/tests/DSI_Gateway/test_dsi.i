// $Id$

ACE_INLINE
DSI_Simple_Server::DSI_Simple_Server (CORBA::ORB_ptr orb,
                                      Simple_Server_ptr target,
                                      PortableServer::POA_ptr poa)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     target_ (Simple_Server::_duplicate (target)),
     poa_ (PortableServer::POA::_duplicate (poa))
{
}

// $Id$

ACE_INLINE
TAO_Client_Priority_Policy::TAO_Client_Priority_Policy (
    PortableServer::POA_ptr poa,
    const TAO::PrioritySpecification &priority_spec)
  :  poa_ (PortableServer::POA::_duplicate (poa)),
     priority_spec_ (priority_spec)
{
}

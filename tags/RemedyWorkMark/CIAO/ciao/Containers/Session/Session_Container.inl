// $Id$   -*- C++ -*-

ACE_INLINE CORBA::Object_ptr
CIAO::Session_Container_i::get_objref (PortableServer::Servant p)
{
  return this->the_POA ()->servant_to_reference (p);
}

ACE_INLINE CIAO::Servant_Activator_ptr
CIAO::Session_Container_i::ports_servant_activator (void)
{
  return Servant_Activator::_duplicate(this->sa_.in ());
}

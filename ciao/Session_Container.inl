// $Id$   -*- C++ -*-

ACE_INLINE CORBA::Object_ptr
CIAO::Session_Container::get_objref (PortableServer::Servant p)
{
  return this->the_POA ()->servant_to_reference (p);
}

ACE_INLINE CIAO::Servant_Activator *
CIAO::Session_Container::ports_servant_activator (void) const
{
  return this->sa_;
}

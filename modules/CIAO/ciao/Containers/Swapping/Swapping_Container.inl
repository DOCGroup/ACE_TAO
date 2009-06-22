// $Id$

ACE_INLINE CORBA::Object_ptr
CIAO::Swapping_Container::get_objref (PortableServer::Servant p)
{
  return this->the_POA ()->servant_to_reference (p);
}

// $Id$

ACE_INLINE CORBA::Object_ptr
CIAO::Swapping_Container::get_objref (PortableServer::Servant p
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->the_home_servant_POA ()->servant_to_reference (p
                                                 ACE_ENV_ARG_PARAMETER);
}

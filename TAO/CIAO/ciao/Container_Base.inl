// $Id$   -*- C++ -*-

ACE_INLINE CIAO::ContainerEventService_ptr
CIAO::Container::get_container_events_ref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->events_ref_.in ()))
    {
      this->events_ref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return this->events_ref_._retn ();
}


ACE_INLINE CORBA::Object_ptr
CIAO::Session_Container::get_objref (PortableServer::Servant p
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->_ciao_the_POA ()->servant_to_reference (p
                                                       ACE_ENV_ARG_PARAMETER);
}

// $Id$

::Components::CCMHome_ptr
CIAO::HomeRegistrar_Impl::find_home_by_component_type (const char * comp_repid
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::HomeNotFound))
{
  return this->find_home_impl (comp_repid
                               ACE_ENV_ARG_PARAMETER);
}

::Components::CCMHome_ptr
CIAO::HomeRegistrar_Impl::find_home_by_home_type (const char * home_repid
                                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::HomeNotFound))
{
  return this->find_home_impl (home_repid
                               ACE_ENV_ARG_PARAMETER);
}

::Components::CCMHome_ptr
CIAO::HomeRegistrar_Impl::find_home_by_name (const char * home_name
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::HomeNotFound))
{
  return this->find_home_impl (home_name
                               ACE_ENV_ARG_PARAMETER);
}

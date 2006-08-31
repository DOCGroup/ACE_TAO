// $Id$

//
// CUTS_CCM_Factory_T
//
template <typename HOME_EXEC, typename EXEC>
CUTS_CCM_Factory_T <HOME_EXEC, EXEC>::CUTS_CCM_Factory_T (void)
{

}

//
// ~CUTS_CCM_Factory_T
//
template <typename HOME_EXEC, typename EXEC>
CUTS_CCM_Factory_T <HOME_EXEC, EXEC>::~CUTS_CCM_Factory_T (void)
{

}

//
// create
//
template <typename HOME_EXEC, typename EXEC>
::Components::EnterpriseComponent_ptr
CUTS_CCM_Factory_T <HOME_EXEC, EXEC>::create (
  ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((::CORBA::SystemException,
                   ::Components::CCMException))
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_THROW_EX (retval,
                    EXEC,
                    ::CORBA::NO_MEMORY ());

  ACE_CHECK_RETURN (::Components::EnterpriseComponent::_nil ());
  return retval;
}

// StandardConfigurator.cpp,v 1.0 2004/14/04 08:47:46 white Exp

#include "StandardConfigurator_Impl.h"
#include "Servant_Impl_Base.h"

ACE_RCSID (ciao,
           StandardConfigurator_Impl,
           "$Id$")


namespace CIAO
{
  StandardConfigurator_Impl::StandardConfigurator_Impl (
      Servant_Impl_Base* servant)
    : component_ (servant)
  {
  }

  StandardConfigurator_Impl::~StandardConfigurator_Impl ()
  {
  }

  void
  StandardConfigurator_Impl::set_configuration (
      const Components::ConfigValues &descr
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    this->component_->set_attributes (descr
                                      ACE_ENV_ARG_PARAMETER);
  }

  void
  StandardConfigurator_Impl::configure (
    Components::CCMObject_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::WrongComponentType))
  {

  }
}

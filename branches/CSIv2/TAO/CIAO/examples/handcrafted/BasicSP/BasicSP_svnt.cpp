// $Id$

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.cpp       GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

#include "BasicSP_svnt.h"

ACE_RCSID (examples,
           BasicSP_svnt,
           "$Id$")

CIAO_GLUE_BasicSP::ReadData_Servant::ReadData_Servant (
    BasicSP::CCM_ReadData_ptr executor,
    Components::CCMContext_ptr c)
  : executor_ (BasicSP::CCM_ReadData::_duplicate (executor)),
    ctx_ (Components::CCMContext::_duplicate (c))
{
}



CIAO_GLUE_BasicSP::ReadData_Servant::~ReadData_Servant (void)
{
}

// This is only a guideline...  we always relay the operation to underlying
// executor.

char *
CIAO_GLUE_BasicSP::ReadData_Servant::get_data (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->get_data (ACE_ENV_SINGLE_ARG_PARAMETER);
}


// get_component implementation.
// get_component is a standard CORBA::Object operations that returns
// the component reference that an object interface associates to.
CORBA::Object_ptr
CIAO_GLUE_BasicSP::ReadData_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Components::SessionContext_var sc =
    Components::SessionContext::_narrow (this->ctx_.in ());

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->ctx_.in ());

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

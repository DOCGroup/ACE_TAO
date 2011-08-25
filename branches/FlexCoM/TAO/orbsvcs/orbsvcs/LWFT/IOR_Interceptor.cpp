// -*- C++ -*-
// $Id$

#include "IOR_Interceptor.h"
#include "ObjectReferenceFactory.h"
#include "tao/ORB_Constants.h"


IOR_Interceptor::IOR_Interceptor (void)
{
}

char *
IOR_Interceptor::name (void)
{
  return CORBA::string_dup ("IOR_Interceptor");
}

void
IOR_Interceptor::destroy (void)
{
}

void
IOR_Interceptor::establish_components (
    PortableInterceptor::IORInfo_ptr /* info */)
{
}

void
IOR_Interceptor::components_established (
    PortableInterceptor::IORInfo_ptr info)
{
  PortableInterceptor::ObjectReferenceFactory_var old_orf =
    info->current_factory ();

  PortableInterceptor::ObjectReferenceFactory * tmp;
  ACE_NEW_THROW_EX (tmp,
                    ObjectReferenceFactory (old_orf.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ObjectReferenceFactory_var orf = tmp;

  info->current_factory (orf.in ());
}

void
IOR_Interceptor::adapter_manager_state_changed (
    const char *,
    PortableInterceptor::AdapterState)
{
}

void
IOR_Interceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
{
}

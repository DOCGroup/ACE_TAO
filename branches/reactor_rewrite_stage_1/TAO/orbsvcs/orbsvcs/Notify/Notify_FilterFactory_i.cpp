/* -*- C++ -*- $Id$ */

#include "Notify_FilterFactory_i.h"
#include "Notify_Filter_i.h"

ACE_RCSID(Notify, Notify_FilterFactory_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_FilterFactory_i::TAO_Notify_FilterFactory_i (void)
{
}

// Implementation skeleton destructor
TAO_Notify_FilterFactory_i::~TAO_Notify_FilterFactory_i (void)
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_FilterFactory_i::get_ref (TAO_ENV_SINGLE_ARG_DECL)
{
  CosNotifyFilter::FilterFactory_var filterfactory;

  filterfactory = _this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyFilter::FilterFactory::_nil ());

  return filterfactory._retn ();
}

CosNotifyFilter::Filter_ptr
TAO_Notify_FilterFactory_i::create_filter (
    const char *constraint_grammar
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidGrammar
  ))
{
  // @@: change to "ExTCL" later.
  if (ACE_OS::strcmp (constraint_grammar, "TCL") != 0)
    ACE_THROW_RETURN (CosNotifyFilter::InvalidGrammar (), 0);

  TAO_Notify_Filter_i* filter;

  ACE_NEW_THROW_EX (filter,
                    TAO_Notify_Filter_i,
                    CORBA::NO_MEMORY ());

  return filter->get_ref (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_FilterFactory_i::create_mapping_filter (
                                                   const char * /*constraint_grammar*/,
                                                   const CORBA::Any & /*default_value*/
                                                   TAO_ENV_ARG_DECL_NOT_USED //TAO_ENV_SINGLE_ARG_PARAMETER
                                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::InvalidGrammar
                   ))
{
  return 0;
}

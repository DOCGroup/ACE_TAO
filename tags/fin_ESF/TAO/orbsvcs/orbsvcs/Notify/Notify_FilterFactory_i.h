/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_FILTER_FACTORY_I_H
#define NOTIFY_FILTER_FACTORY_I_H

#include "orbsvcs/orbsvcs/CosNotifyFilterS.h"

class TAO_ORBSVCS_Export TAO_Notify_FilterFactory_i :
public POA_CosNotifyFilter::FilterFactory
{
  // = TITLE
  //
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_FilterFactory_i (void);
  // Constructor

  virtual ~TAO_Notify_FilterFactory_i (void);
  // Destructor

  CosNotifyFilter::FilterFactory_ptr
  get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Activate with the default POA

  virtual CosNotifyFilter::Filter_ptr create_filter (
    const char * constraint_grammar,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidGrammar
  ));

  virtual CosNotifyFilter::MappingFilter_ptr create_mapping_filter (
    const char * constraint_grammar,
    const CORBA::Any & default_value,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidGrammar
  ));

};

#endif /* NOTIFY_FILTER_FACTORY_I_H */

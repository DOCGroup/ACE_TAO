// -*- C++ -*-

//=============================================================================
/**
 *  @file   EventChannelFactory_i.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef EVENTCHANNELFACTORY_I_H
#define EVENTCHANNELFACTORY_I_H

#include "orbsvcs/FT_CORBAS.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EventChannelFactory_i : public POA_PortableGroup::GenericFactory {
public:
  EventChannelFactory_i(const char* conf_filename, CORBA::ORB_ptr);

  virtual CORBA::Object_ptr create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id
    );

  virtual void delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id
    );

private:
  CORBA::Object_ptr create_process (
    char * process,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id);

  const char* conf_file;
  int id;
  CORBA::ORB_ptr orb;
  typedef ACE_Hash_Map_Manager<int, CORBA::Object_var, ACE_Null_Mutex> Objects;
  Objects objects;
};
#endif


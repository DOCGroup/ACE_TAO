// -*- C++ -*-

//=============================================================================
/**
 *  @file   EventChannelFactory_i.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef EVENTCHANNELFACTORY_I_H
#define EVENTCHANNELFACTORY_I_H

#include "orbsvcs/FT_CORBAS.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager_T.h"

#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EventChannelFactory_i : public POA_PortableGroup::GenericFactory
{
public:
  EventChannelFactory_i (std::string const &conf_filename, CORBA::ORB_ptr);

  CORBA::Object_ptr create_object (
    char const *type_id,
    PortableGroup::Criteria const &the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id) override;

  void delete_object (
    PortableGroup::GenericFactory::FactoryCreationId const &factory_creation_id) override;

private:
  EventChannelFactory_i (EventChannelFactory_i const &) = delete;
  EventChannelFactory_i &operator= (EventChannelFactory_i const &) = delete;
  EventChannelFactory_i (EventChannelFactory_i &&) = delete;
  EventChannelFactory_i &operator= (EventChannelFactory_i &&) = delete;

  CORBA::Object_ptr create_process (
    std::string const &process,
    PortableGroup::Criteria const &the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id);

  std::string conf_file;
  int id {};
  CORBA::ORB_ptr orb;
  using Objects = ACE_Hash_Map_Manager<int, CORBA::Object_var, ACE_Null_Mutex>;
  Objects objects;
};
#endif


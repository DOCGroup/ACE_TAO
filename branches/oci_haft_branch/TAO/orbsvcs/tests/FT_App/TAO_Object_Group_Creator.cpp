/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    TAO_Object_Group_Creator.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Utility to Create Object Group
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "TAO_Object_Group_Creator.h"
#include <iostream>
#include <fstream>
#include <orbsvcs/PortableGroup/PG_Properties_Encoder.h>
#include <orbsvcs/FT_FaultDetectorFactoryC.h>
#include <orbsvcs/PortableGroup/PG_Operators.h>

#include <ace/Get_Opt.h>


TAO::Object_Group_Creator::Object_Group_Creator ()
  : registry_(0)
  , replication_manager_(0)
  , have_replication_manager_(0)
  , detector_infos_ (0)
{
}

TAO::Object_Group_Creator::~Object_Group_Creator ()
{
}

int TAO::Object_Group_Creator::set_factory_registry ( const char * factory_ior)
{
  int result = 1;

  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var registry_obj = this->orb_->string_to_object (factory_ior  ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    this->registry_ = PortableGroup::FactoryRegistry::_narrow(registry_obj  ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (! CORBA::is_nil (registry_))
    {
      result = 0; // success
    }
  }
  ACE_CATCHANY
  {
    result = 1;
  }
  ACE_ENDTRY;
  return result;
}

int TAO::Object_Group_Creator::set_factory_registry (PortableGroup::FactoryRegistry_ptr factory)
{
  this->registry_ = factory;
  return 0;
}


int TAO::Object_Group_Creator::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 1;
  this->orb_ = orb;

  if (CORBA::is_nil (this->registry_))
  {
    ///////////////////////////////
    // Find the ReplicationManager
    ACE_TRY_NEW_ENV
    {
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!CORBA::is_nil (this->replication_manager_))
      {
        this->have_replication_manager_ = 1;
        // empty criteria
        ::PortableGroup::Criteria criteria;
        this->registry_ = this->replication_manager_->get_factory_registry(criteria  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (!CORBA::is_nil (this->registry_))
        {
          this->detector_infos_ = this->registry_->list_factories_by_role (FT::FAULT_DETECTOR_ROLE_NAME, this->detector_type_id_.out()
              ACE_ENV_ARG_PARAMETER)
          ACE_TRY_CHECK;
          CORBA::ULong count = this->detector_infos_->length();
          ACE_DEBUG ((LM_DEBUG,
            "%T %n (%P|%t)Object_Group_Creator: found %u factories for FaultDetectors\n",
             ACE_static_cast (unsigned, count)
             ));
          result = 0;
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            "%T %n (%P|%t) Object_Group_Creator: ReplicationManager failed to return FactoryRegistry.\n"
            ));
          ACE_THROW (CORBA::NO_IMPLEMENT());
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          "%T %n (%P|%t) Object_Group_Creator: resolve_initial_references for ReplicationManager failed.\n"
          ));
        ACE_THROW (CORBA::OBJECT_NOT_EXIST());
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Creator: Exception resolving ReplicationManager,\n");

      result = 1;
    }
    ACE_ENDTRY;
  }

  return result;
}

int TAO::Object_Group_Creator::unregister_role(const char * role ACE_ENV_ARG_DECL)
{
  int result = 0;
  ACE_ERROR ((LM_INFO,
    "%T %n (%P|%t) Object_Group_Creator: Unregistering all factories for %s\n",
    role
    ));
  this->registry_->unregister_factory_by_role (role ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return result;
}



int TAO::Object_Group_Creator::create_detector_for_replica (
  CORBA::Object_ptr replica,
  const char * role,
  const char * type_id,
  PortableGroup::ObjectGroupId group_id,
  const PortableGroup::Location & location
  ACE_ENV_ARG_DECL)
{
  int result = 0;

  CORBA::ULong detector_count = this->detector_infos_->length();
  for (CORBA::ULong n_detector = 0; result == 0 && n_detector < detector_count; ++n_detector)
  {
    PortableGroup::FactoryInfo & info = (*this->detector_infos_)[n_detector];
    if ( info.the_location == location || n_detector + 1 == detector_count)
    {
      TAO_PG::Properties_Encoder encoder;

      PortableGroup::Value value;

/*
      //////////////////
      // FaultDetectorFactory gets picky about FaultNotifier's object type.
      // coddle it.
      ::FT::FaultNotifier_var notifier = ::FT::FaultNotifier::_narrow(this_obj);
      value <<= notifier;
      encoder.add(::FT::FT_NOTIFIER, value);
*/

      FT::PullMonitorable_ptr monitorable = FT::PullMonitorable::_narrow (replica);
      value <<= monitorable;
      encoder.add (::FT::FT_MONITORABLE, value);

      FT::FTDomainId domain_id = 0;
      value <<= domain_id;
      encoder.add (::FT::FT_DOMAIN_ID, value);

      value <<= location;
      encoder.add (::FT::FT_LOCATION, value);

      value <<= type_id;
      encoder.add (::FT::FT_TYPE_ID, value);

      value <<= group_id;
      encoder.add (::FT::FT_GROUP_ID, value);

      value <<= CORBA::string_dup (role);
      encoder.add (PortableGroup::role_criterion, value);

      // allocate and populate the criteria
      FT::Criteria_var criteria;
      ACE_NEW_NORETURN (criteria,
        FT::Criteria);
      if (criteria.ptr() == 0)
      {
        ACE_ERROR((LM_ERROR,
          "%T %n (%P|%t)Object_Group_Creater: Error cannot allocate criteria.\n"
          ));
          result = -1;
      }
      else
      {
        encoder.encode(criteria);
        FT::GenericFactory::FactoryCreationId_var factory_creation_id;

        info.the_factory->create_object (
          type_id,
          criteria.in(),
          factory_creation_id
          ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        result = 1;
      }
    }
  }
  return result;
}

CORBA::Object_ptr TAO::Object_Group_Creator::create_group(
    const char * role,
    int write_iors
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException ))
{
  int result = 0;
  CORBA::Object_var group;

  PortableGroup::ObjectGroupId group_id = 0;
  CORBA::String_var type_id;
  ::PortableGroup::FactoryInfos_var infos = this->registry_->list_factories_by_role (role, type_id
      ACE_ENV_ARG_PARAMETER)
  ACE_CHECK_RETURN (CORBA::Object::_nil());

  CORBA::ULong count = infos->length();
  ACE_ERROR ((LM_INFO,
    "%T %n (%P|%t): Object_Group_Creator: found %u factories for %s : %s\n",
    ACE_static_cast (unsigned, count),
    role,
    ACE_static_cast(const char *, type_id)
    ));

  if (count > 0)
  {
    ///////////////////////////
    // Begin with an empty IOGR
    ::PortableGroup::GenericFactory::FactoryCreationId_var creation_id;
    if (this->have_replication_manager_)
    {
      PortableGroup::Criteria criteria(1);
      criteria.length(1);
      criteria[0].nam.length(1);
      criteria[0].nam[0].id = PortableGroup::PG_MEMBERSHIP_STYLE;
      criteria[0].val <<= PortableGroup::MEMB_APP_CTRL;

      group = this->replication_manager_->create_object(
        type_id.in(),
        criteria,
        creation_id
        ACE_ENV_ARG_PARAMETER
        );
      ACE_CHECK_RETURN (CORBA::Object::_nil());

      //@@ this is a bit of a hack
      creation_id >>= group_id;

    }

    const PortableGroup::Location * first_location = 0;

    for (CORBA::ULong nFact = 0; nFact < count; ++nFact)
    {
      ::PortableGroup::FactoryInfo & info = infos[nFact];

      const char * loc_name = info.the_location[0].id;
      ACE_ERROR ((LM_INFO,
        "%T %n (%P|%t) Object_Group_Creator: Creating %s@%s\n",
        role,
        loc_name
        ));

      PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;
      CORBA::Object_var created_obj = info.the_factory->create_object (
        type_id.in(),
        info.the_criteria,
        factory_creation_id
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil());
      if ( !CORBA::is_nil(created_obj) )
      {
        if (nFact == 0)
        {
          first_location = & info.the_location;
        }
        create_detector_for_replica (
          created_obj,
          role,
          type_id.in(),
          group_id,
          info.the_location
          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CORBA::Object::_nil());

        const char * replica_ior = orb_->object_to_string (created_obj ACE_ENV_ARG_PARAMETER );
        ACE_CHECK_RETURN (CORBA::Object::_nil());


        if (write_iors)
        {
          ////////////////////////////////////
          // Somewhat of a hack
          // guess at role of factory creation id
          CORBA::ULong ulong_id = 0;
          CORBA::Long long_id = 0;
          if (factory_creation_id >>= ulong_id)
          {
            // ok
          }
          else if (factory_creation_id >>= long_id)
          {
            ulong_id = ACE_static_cast(CORBA::ULong, long_id);
          }
          else
          {
            ACE_ERROR ((LM_ERROR,
              "%T %n (%P|%t) Object_Group_Creator: Can't decypher factory creation id.\n"
              ));
            // Guessed wrong.  Just use default value
          }

          char replica_ior_filename[200]; // "${role}_$(location)_${factory_id}.ior"

          ACE_OS::snprintf(replica_ior_filename, sizeof(replica_ior_filename)-1, "%s_%s_%lu.ior",
            role,
            loc_name,
            ACE_static_cast(unsigned long, ulong_id));
          replica_ior_filename[sizeof(replica_ior_filename)-1] = '\0';

          ACE_ERROR ((LM_INFO,
            "%T %n (%P|%t) Object_Group_Creator: Writing ior for created object to %s\n",
            replica_ior_filename
            ));

          if (write_ior_file(replica_ior_filename, replica_ior) != 0)
          {
            ACE_ERROR ((LM_ERROR,
              "%T %n (%P|%t) Object_Group_Creator: Error writing ior [%s] to %s\n",
               replica_ior,
               replica_ior_filename
               ));
          }
        }

        group = this->replication_manager_->add_member (group,
                          info.the_location,
                          created_obj
                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CORBA::Object::_nil());
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
          "%T %n (%P|%t) Object_Group_Creator: create_object returned nil????\n"
          ));
      }
    }

    if (first_location != 0)
    {
      group = this->replication_manager_->set_primary_member (group.in(), * first_location ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil());
    }
    ACE_ERROR ((LM_INFO,
      "%T %n (%P|%t) Object_Group_Creator:  Successfully created group of %s\n",
      role
      ));

    const char * group_iogr = orb_->object_to_string (group ACE_ENV_ARG_PARAMETER );
    ACE_CHECK_RETURN (CORBA::Object::_nil());
  }

  return group._retn();
}


int TAO::Object_Group_Creator::fini ()
{
  return 0;
}

int TAO::Object_Group_Creator::write_ior_file(const char * outputFile, const char * ior)
{
  int result = -1;
  FILE* out = ACE_OS::fopen (outputFile, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", ior);
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "%T %n (%P|%t) Object_Group_Creator: Open failed for %s\n", outputFile
    ));
  }
  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
//  template ACE_Vector<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
//# pragma instantiate ACE_Vector<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

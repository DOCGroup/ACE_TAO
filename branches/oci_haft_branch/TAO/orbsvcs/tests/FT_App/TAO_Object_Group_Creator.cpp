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

#include <ace/Get_Opt.h>


TAO::Object_Group_Creator::Object_Group_Creator ()
  : registry_(0)
  , replication_manager_(0)
  , have_replication_manager_(0)
  , iogr_seq_(0)
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
          result = 0; // success
        }
        else
        {
          std::cerr << "Creator: ReplicationManager failed to return FactoryRegistry." << std::endl;
        }
      }
      else
      {
        std::cerr << "Creator: Can't resolve ReplicationManager, and no factory registry option was given." << std::endl;
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Creator: Exception resolving ReplicationManager, and no -f option was given.\n");

      result = 1;
    }
    ACE_ENDTRY;
  }

  return result;
}

int TAO::Object_Group_Creator::unregister_role(const char * role ACE_ENV_ARG_DECL)
{
  int result = 0;
  std::cout << std::endl << "Creator: Unregistering all factories for " << role << std::endl;
  this->registry_->unregister_factory_by_role (role ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return result;
}

int TAO::Object_Group_Creator::create_group(const char * role, int write_iors, int write_iogr ACE_ENV_ARG_DECL)
{
  int result = 0;

  CORBA::String_var type_id;
  ::PortableGroup::FactoryInfos_var infos = this->registry_->list_factories_by_role (role, type_id
      ACE_ENV_ARG_PARAMETER)
  ACE_CHECK;

  CORBA::ULong count = infos->length();
  std::cout << "Creator: found " << count << " factories for "
            << role << "(" << ACE_static_cast(const char *, type_id) << ")"<< std::endl;

  if (count > 0)
  {
    ///////////////////////////
    // Begin with an empty IOGR
    ::PortableGroup::GenericFactory::FactoryCreationId_var creation_id;
    CORBA::Object_var group;
    if (write_iogr && this->have_replication_manager_)
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
      ACE_CHECK;

    }

    for (CORBA::ULong nFact = 0; nFact < count; ++nFact)
    {

#define DUMP_EM_ALL
#ifdef DUMP_EM_ALL
      if (write_iogr && this->have_replication_manager_)
      {
        const char * group_ior = orb_->object_to_string (group ACE_ENV_ARG_PARAMETER );
        char group_ior_filename[200]; // "${role}_$(location)_${factory_id}.ior"

        ACE_OS::snprintf(group_ior_filename, sizeof(group_ior_filename)-1, "%s_%lu_%lu.group",
          role,
          this->iogr_seq_,
          ACE_static_cast(unsigned long, nFact));

        std::cout << "Creator: Writing ior before factory " << nFact << " to " << group_ior_filename << std::endl;

        if (write_ior_file(group_ior_filename, group_ior) != 0)
        {
          std::cerr << "Creator: Error writing ior [" << group_ior << "] to " << group_ior_filename << std::endl;
        }
      }
#endif //DUMP_EM_ALL

      ::PortableGroup::FactoryInfo info = infos[nFact];
  /*  struct FactoryInfo {
        GenericFactory the_factory;
        Location the_location;
        Criteria the_criteria;
      };
  */
      const char * loc_name = info.the_location[0].id;
      std::cout << "Creator: Creating " << role << " at " << loc_name << std::endl;

      PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;
      CORBA::Object_var created_obj = info.the_factory->create_object (
        type_id.in(),
        info.the_criteria,
        factory_creation_id
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if ( !CORBA::is_nil(created_obj) )
      {
        const char * replica_ior = orb_->object_to_string (created_obj ACE_ENV_ARG_PARAMETER );
        ACE_CHECK;


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
            std::cerr << "Can't decypher factory creation id." << std::endl;
            // Guessed wrong.  Just use default value
          }

          char replica_ior_filename[200]; // "${role}_$(location)_${factory_id}.ior"

          ACE_OS::snprintf(replica_ior_filename, sizeof(replica_ior_filename)-1, "%s_%s_%lu.ior",
            role,
            loc_name,
            ACE_static_cast(unsigned long, ulong_id));
          replica_ior_filename[sizeof(replica_ior_filename)-1] = '\0';

          std::cout << "Creator: Writing ior for created object to " << replica_ior_filename << std::endl;

          if (write_ior_file(replica_ior_filename, replica_ior) != 0)
          {
            std::cerr << "Creator: Error writing ior [" << replica_ior << "] to " << replica_ior_filename << std::endl;
          }
        }

        if (write_iogr && this->have_replication_manager_)
        {
          group = this->replication_manager_->add_member (group,
                            info.the_location,
                            created_obj
                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
#if 0 // DEBUG_DISABLE
          if (nFact == 0)
          {
            group = this->replication_manager_->set_primary_member(group, info.the_location
                            ACE_ENV_ARG_PARAMETER);
          }
#endif// DEBUG_DISABLE
        }
      }
      else
      {
        std::cerr << "Creator: create_object returned nil????" << std::endl;
      }
    }

    std::cout << "Creator:  Successfully created group of " << role << std::endl;

    if(write_iogr && have_replication_manager_)
    {
      const char * group_iogr = orb_->object_to_string (group ACE_ENV_ARG_PARAMETER );
      ACE_CHECK;
      char group_ior_filename[200];

      ACE_OS::snprintf(group_ior_filename, sizeof(group_ior_filename)-1, "%s_%lu.iogr",
        role,
        this->iogr_seq_);
      group_ior_filename[sizeof(group_ior_filename)-1] = '\0';

      std::cout << "Creator: Writing ior for created object group to " << group_ior_filename << std::endl;

      if (write_ior_file(group_ior_filename, group_iogr) != 0)
      {
        std::cerr << "Creator: Error writing ior [" << group_iogr << "] to " << group_ior_filename << std::endl;
      }
      this->iogr_seq_ += 1;
    }

  }

  return result;
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
      "Open failed for %s\n", outputFile
    ));
  }
  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
//  template ACE_Vector<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
//# pragma instantiate ACE_Vector<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

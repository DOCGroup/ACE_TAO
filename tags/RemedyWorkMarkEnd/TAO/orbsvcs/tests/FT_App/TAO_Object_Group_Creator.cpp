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
#include <ace/OS_NS_stdio.h>
#include <orbsvcs/PortableGroup/PG_Properties_Encoder.h>
#include <orbsvcs/FT_FaultDetectorFactoryC.h>
#include <orbsvcs/PortableGroup/PG_Operators.h>

#include <ace/Get_Opt.h>

TAO::Object_Group_Creator::Object_Group_Creator ()
  : orb_(CORBA::ORB::_nil ())
  , registry_ (PortableGroup::FactoryRegistry::_nil ())
  , replication_manager_ (FT::ReplicationManager::_nil ())
  , detector_infos_ (0)
  , have_replication_manager_ (0)
{
}

TAO::Object_Group_Creator::~Object_Group_Creator ()
{
}

int TAO::Object_Group_Creator::set_factory_registry (PortableGroup::FactoryRegistry_ptr factory)
{
  this->registry_ = PortableGroup::FactoryRegistry::_duplicate (factory);
  return CORBA::is_nil (this->registry_.in ()) ? -1 : 0 ;
}


int TAO::Object_Group_Creator::init (CORBA::ORB_ptr orb)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  if (CORBA::is_nil (this->registry_.in ()))
  {
    ///////////////////////////////
    // Find the ReplicationManager
    try
    {
      CORBA::Object_var rm_obj
        = orb->resolve_initial_references (
          "ReplicationManager");

      this->replication_manager_
        = ::FT::ReplicationManager::_narrow (
          rm_obj.in ());
      if (!CORBA::is_nil (this->replication_manager_.in ()))
      {
        this->have_replication_manager_ = 1;
        // empty criteria
        ::PortableGroup::Criteria criteria;
        this->registry_
          = this->replication_manager_->get_factory_registry (
            criteria);
        if (!CORBA::is_nil (this->registry_.in ()))
        {
          this->detector_infos_
            = this->registry_->list_factories_by_role (
              FT::FAULT_DETECTOR_ROLE_NAME,
              this->detector_type_id_.out ());
          CORBA::ULong count = this->detector_infos_->length ();
          ACE_DEBUG ( (LM_DEBUG,
            "%T %n (%P|%t)Object_Group_Creator: found %u factories for FaultDetectors\n",
             static_cast<unsigned> (count)
             ));
        }
        else
        {
          ACE_ERROR ( (LM_ERROR,
            "%T %n (%P|%t) Object_Group_Creator: ReplicationManager failed to return FactoryRegistry.\n"
            ));
          throw CORBA::NO_IMPLEMENT ();
        }
      }
      else
      {
        ACE_ERROR ( (LM_ERROR,
          "%T %n (%P|%t) Object_Group_Creator: resolve_initial_references for ReplicationManager failed.\n"
          ));
        throw CORBA::OBJECT_NOT_EXIST ();
      }
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Creator: Exception resolving ReplicationManager,\n");

      result = 1;
    }
  }

  return result;
}

int TAO::Object_Group_Creator::unregister_role (const char * role)
{
  int result = 0;
  ACE_ERROR ( (LM_INFO,
    "%T %n (%P|%t) Object_Group_Creator: Unregistering all factories for %s\n",
    role
    ));
  this->registry_->unregister_factory_by_role (role);
  return result;
}



int TAO::Object_Group_Creator::create_detector_for_replica (
  CORBA::Object_ptr replica,
  const char * role,
  const char * type_id,
  PortableGroup::ObjectGroupId group_id,
  const PortableGroup::Location & location)
{
  int result = 0;

  if (this->have_replication_manager_)
  {
    CORBA::ULong detector_count = this->detector_infos_->length ();
    for (CORBA::ULong n_detector = 0; result == 0 && n_detector < detector_count; ++n_detector)
    {
      PortableGroup::FactoryInfo & info = (*this->detector_infos_)[n_detector];
      if ( info.the_location == location || n_detector + 1 == detector_count)
      {
        TAO_PG::Properties_Encoder encoder;

        PortableGroup::Value value;

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
        PortableGroup::Criteria_var criteria;
        ACE_NEW_NORETURN (criteria,
          PortableGroup::Criteria);
        if (criteria.ptr () == 0)
        {
          ACE_ERROR ( (LM_ERROR,
            "%T %n (%P|%t)Object_Group_Creater: Error cannot allocate criteria.\n"
            ));
            result = -1;
        }
        else
        {
          encoder.encode (criteria);
          PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;

          info.the_factory->create_object (
            type_id,
            criteria.in (),
            factory_creation_id);
          result = 1;
        }
      }
    }
  }
  return result;
}


CORBA::Object_ptr TAO::Object_Group_Creator::create_infrastructure_managed_group (
    const char * type_id)
{
  CORBA::Object_var group = CORBA::Object::_nil ();

  if (this->have_replication_manager_)
  {
    // set typeid properties
    PortableGroup::Properties properties (1);
    properties.length (3);

    properties[0].nam.length (1);
    properties[0].nam[0].id = PortableGroup::PG_MEMBERSHIP_STYLE;
    properties[0].val <<= PortableGroup::MEMB_INF_CTRL;

    PortableGroup::InitialNumberMembersValue inm(2);
    properties[1].nam.length (1);
    properties[1].nam[0].id = PortableGroup::PG_INITIAL_NUMBER_MEMBERS;
    properties[1].val <<= inm;

    PortableGroup::MinimumNumberMembersValue mnm(1);
    properties[2].nam.length (1);
    properties[2].nam[0].id = PortableGroup::PG_MINIMUM_NUMBER_MEMBERS;
    properties[2].val <<= mnm;

    this->replication_manager_->set_type_properties (
      type_id,
      properties);

    ::PortableGroup::GenericFactory::FactoryCreationId_var creation_id;
    PortableGroup::Criteria criteria (1);
    criteria.length (1);
    criteria[0].nam.length (1);
    criteria[0].nam[0].id = PortableGroup::PG_MEMBERSHIP_STYLE;
    criteria[0].val <<= PortableGroup::MEMB_APP_CTRL;

    group = this->replication_manager_->create_object (
      type_id,
      criteria,
      creation_id
      );
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT("%T %n (%P|%t): Object_Group_Creator: infrastructure managed group requires Replication Manager\n")
      ));
  }
  return group._retn ();
}

CORBA::Object_ptr TAO::Object_Group_Creator::create_group (
    const char * role,
    int write_iors)
{
  CORBA::Object_var group = CORBA::Object::_nil ();

  PortableGroup::ObjectGroupId group_id = 0;
  CORBA::String_var type_id;
  ::PortableGroup::FactoryInfos_var infos = this->registry_->list_factories_by_role (role, type_id);

  CORBA::ULong count = infos->length ();
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("%T %n (%P|%t): Object_Group_Creator: ")
              ACE_TEXT ("found %u factories for %s : %s\n"),
              static_cast<unsigned> (count),
              role,
              type_id.in ()));

  if (count > 0)
  {
    ///////////////////////////
    // Begin with an empty IOGR
    ::PortableGroup::GenericFactory::FactoryCreationId_var creation_id;
    if (this->have_replication_manager_)
    {

      //////////////////////////////////////////////////////
      // note infrastructure controlled because we want the
      // ReplicationManager to manage the object after it's created.
      // Initial number members = 0 because we do not want the Replication
      // Manager to populate the group.
      // Minimum number of members = 0 because we do not want the
      // Replication Manager to replace failed members.
      // Semi-active, because that's what we do.
      PortableGroup::Criteria criteria (3);
      criteria.length (4);
      criteria[0].nam.length (1);
      criteria[0].nam[0].id = PortableGroup::PG_MEMBERSHIP_STYLE;
      criteria[0].val <<= PortableGroup::MEMB_INF_CTRL;

      criteria[1].nam.length (1);
      criteria[1].nam[0].id = PortableGroup::PG_INITIAL_NUMBER_MEMBERS;
      criteria[1].val <<= PortableGroup::InitialNumberMembersValue (0);

      criteria[2].nam.length (1);
      criteria[2].nam[0].id = PortableGroup::PG_MINIMUM_NUMBER_MEMBERS;
      criteria[2].val <<= PortableGroup::MinimumNumberMembersValue (0);

      criteria[3].nam.length (1);
      criteria[3].nam[0].id = FT::FT_REPLICATION_STYLE;
      criteria[3].val <<= FT::SEMI_ACTIVE;

      group = this->replication_manager_->create_object (
        type_id.in (),
        criteria,
        creation_id
        );

      //@@ this is a bit of a hack
      creation_id >>= group_id;

    }

    const PortableGroup::Location * first_location = 0;

    for (CORBA::ULong nFact = 0; nFact < count; ++nFact)
    {
      ::PortableGroup::FactoryInfo & info = infos[nFact];

      const char * loc_name = info.the_location[0].id;
      ACE_ERROR ( (LM_INFO,
        "%T %n (%P|%t) Object_Group_Creator: Creating %s@%s\n",
        role,
        loc_name
        ));

      PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;
      CORBA::Object_var created_obj = info.the_factory->create_object (
        type_id.in (),
        info.the_criteria,
        factory_creation_id.out ());
      if ( !CORBA::is_nil (created_obj.in ()) )
      {
// that which was first shall now be last        if (nFact == 0)
        {
          first_location = & info.the_location;
        }

        // try to create a detector, but don't worry if it doesn't happen
        (void) create_detector_for_replica (
          created_obj.in (),
          role,
          type_id.in (),
          group_id,
          info.the_location);

        const char * replica_ior = orb_->object_to_string (created_obj.in () );


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
            ulong_id = static_cast<CORBA::ULong> (long_id);
          }
          else
          {
            ACE_ERROR ( (LM_ERROR,
              "%T %n (%P|%t) Object_Group_Creator: Can't decypher factory creation id.\n"
              ));
            // Guessed wrong.  Just use default value
          }

          char replica_ior_filename[200]; // "${role}_$ (location)_${factory_id}.ior"

          ACE_OS::snprintf (replica_ior_filename, sizeof (replica_ior_filename), "%s_%s_%lu.ior",
            role,
            loc_name,
            static_cast<unsigned long> (ulong_id));

          ACE_ERROR ( (LM_INFO,
            "%T %n (%P|%t) Object_Group_Creator: Writing ior for created object to %s\n",
            replica_ior_filename
            ));

          if (write_ior_file (replica_ior_filename, replica_ior) != 0)
          {
            ACE_ERROR ( (LM_ERROR,
              "%T %n (%P|%t) Object_Group_Creator: Error writing ior [%s] to %s\n",
               replica_ior,
               replica_ior_filename
               ));
          }
        }

        if (this->have_replication_manager_)
        {
          group = this->replication_manager_->add_member (
            group.in (),
            info.the_location,
            created_obj.in ());
        }
      }
      else
      {
        ACE_ERROR ( (LM_ERROR,
          "%T %n (%P|%t) Object_Group_Creator: create_object returned nil????\n"
          ));
      }
    }

    if (first_location != 0 && this->have_replication_manager_)
    {
      group = this->replication_manager_->set_primary_member (group.in (), * first_location);
    }
    ACE_ERROR ( (LM_INFO,
      "%T %n (%P|%t) Object_Group_Creator:  Successfully created group of %s\n",
      role
      ));
  }

  return group._retn ();
}


int TAO::Object_Group_Creator::fini ()
{
  return 0;
}

int TAO::Object_Group_Creator::write_ior_file (const char * outputFile, const char * ior)
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
    ACE_ERROR ( (LM_ERROR,
      "%T %n (%P|%t) Object_Group_Creator: Open failed for %s\n", outputFile
    ));
  }
  return result;
}

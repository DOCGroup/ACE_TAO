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
  : registry_filename_(0)
  , registry_(0)
  , replication_manager_(0)
  , have_replication_manager_(0)
  , iogr_seq_(0)
{
}

TAO::Object_Group_Creator::~Object_Group_Creator ()
{
}

int
TAO::Object_Group_Creator::parse_args (int argc, char *argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, "t:f:k:");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 't':
      {
        this->create_types_.push_back (get_opts.opt_arg ());
        break;
      }
      case 'k':
      {
        this->kill_types_.push_back (get_opts.opt_arg ());
        break;
      }
      case 'f':
      {
        this->registry_filename_ = get_opts.opt_arg ();
        break;
      }

      default:
      {
        std::cerr << "Creator: Unknown argument -" << (char) c << std::endl;
        usage(std::cerr);
        result = 1;
        break;
      }
      case '?':
      {
        usage(std::cerr);
        result = 1;
        break;
      }
    }
  }

  if ( this->create_types_.size() == 0 && this->kill_types_.size())
  {
    std::cerr << "Creator: neither create (-t) nor kill (-k) specified.  Nothing to do." << std::endl;
    usage (std::cerr);
    result = -1;
  }

  return result;
}

void TAO::Object_Group_Creator::usage(ostream & out)const
{
  out << "usage"
      << " -t <type_id for objects to be created>"
      << " -f <factory ior file>"
      << std::endl;
}



int TAO::Object_Group_Creator::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 1;
  this->orb_ = orb;

  // if a factory IOR was specified on command line
  if ( this->registry_filename_ != 0)
  {
    //////////////////////////////////////
    // Try using the -f argument as an IOR
    ACE_TRY_NEW_ENV
    {
      CORBA::Object_var registry_obj = this->orb_->string_to_object (this->registry_filename_  ACE_ENV_ARG_PARAMETER); 
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
  }
  else  // no -f option.  Try RIR(RM)
  {
    ///////////////////////////////
    // Find the ReplicationManager
    ACE_TRY_NEW_ENV
    {
std::cout << "RIR(ReplicationManager)" << std::endl;
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!CORBA::is_nil (replication_manager_))
      {
std::cout << "Found a _real_ ReplicationManager.  Ask it for a factory registry." << std::cout;
        have_replication_manager_ = 1;
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

std::cout << "did we get a FactoryRegistry instead?" << std::endl;
        registry_ =  ::PortableGroup::FactoryRegistry::_narrow(rm_obj.in()  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        if (!CORBA::is_nil(registry_))
        {
std::cout << "Found a FactoryRegistry DBA ReplicationManager" << std::endl;
          result = 0; // success
        }
        else
        {
          std::cerr << "Creator: Can't resolve ReplicationManager, and no -f option was given." << std::endl;
        }
      }
    }
    ACE_CATCHANY
    {
      std::cerr << "Creator: Exception resolving ReplicationManager, and no -f option was given." << std::endl;
      result = 1;
    }
    ACE_ENDTRY;
  }

  return result;
}

int TAO::Object_Group_Creator::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;
  size_t typeCount = this->create_types_.size();
  for ( size_t nType = 0; result == 0 && nType < typeCount; ++nType)
  {
    const char * type = this->create_types_[nType].c_str();
    result = create_group (type);
  }

  typeCount = this->kill_types_.size();
  for ( nType = 0; result == 0 && nType < typeCount; ++nType)
  {
    const char * type = this->kill_types_[nType].c_str();
    result = kill_type (type);
  }

  return result;
}

int TAO::Object_Group_Creator::kill_type(const char * type ACE_ENV_ARG_DECL)
{
  int result = 0;
  std::cout << std::endl << "Creator: Unregistering all factories for " << type << std::endl;
  this->registry_->unregister_factory_by_type (type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return result;
}

int TAO::Object_Group_Creator::create_group(const char * type ACE_ENV_ARG_DECL)
{
  int result = 0;

  std::cout << std::endl << "Creator: Creating group of " << type << std::endl;

  ::PortableGroup::FactoryInfos_var infos = this->registry_->list_factories_by_type (type
      ACE_ENV_ARG_PARAMETER)
  ACE_CHECK;

  CORBA::ULong count = infos->length();
  std::cout << "Creator: found " << count << " factories for " << type << std::endl;

  if (count > 0)
  {
    ///////////////////////////
    // Begin with an empty IOGR
    ::PortableGroup::GenericFactory::FactoryCreationId_var creation_id;
    CORBA::Object_var iogr;
    if (this->have_replication_manager_)
    {
      PortableGroup::Criteria criteria;
      iogr = this->replication_manager_->create_object(
        type,
        criteria,
        creation_id
        ACE_ENV_ARG_PARAMETER
        );
      ACE_CHECK;

    }

    for (CORBA::ULong nFact = 0; nFact < count; ++nFact)
    {
      ::PortableGroup::FactoryInfo info = infos[nFact];
  /*  struct FactoryInfo {
        GenericFactory the_factory;
        Location the_location;
        Criteria the_criteria;
      };
  */
      const char * loc_name = info.the_location[0].id;
      std::cout << "Creator: Creating " << type << " at " << loc_name << std::endl;

      PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;
      CORBA::Object_var created_obj = info.the_factory->create_object (
        type,
        info.the_criteria,
        factory_creation_id
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if ( !CORBA::is_nil(created_obj) )
      {
        const char * replica_ior = orb_->object_to_string (created_obj ACE_ENV_ARG_PARAMETER );
        ACE_CHECK;

        ////////////////////////////////////
        // Somewhat of a hack
        // guess at type of factory creation id
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

        char replica_ior_filename[200]; // "${type}_$(location)_${factory_id}.ior"

        ACE_OS::snprintf(replica_ior_filename, sizeof(replica_ior_filename)-1, "%s_%s_%lu.ior",
          type,
          loc_name,
          ACE_static_cast(unsigned long, ulong_id));
        replica_ior_filename[sizeof(replica_ior_filename)-1] = '\0';

        std::cout << "Creator: Writing ior for created object to " << replica_ior_filename << std::endl;

        if (write_ior_file(replica_ior_filename, replica_ior) != 0)
        {
          std::cerr << "Creator: Error writing ior [" << replica_ior << "] to " << replica_ior_filename << std::endl;
        }

        if (this->have_replication_manager_)
        {
          iogr = this->replication_manager_->add_member (iogr,
                            info.the_location,
                            created_obj
                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          if (nFact == 0)
          {
            iogr = this->replication_manager_->set_primary_member(iogr, info.the_location
                            ACE_ENV_ARG_PARAMETER);
          }
        }
      }
      else
      {
        std::cerr << "Creator: create_object returned nil????" << std::endl;
      }
    }

    std::cout << "Creator:  Successfully created group of " << type << std::endl;

    if( have_replication_manager_)
    {
      const char * replica_iogr = orb_->object_to_string (iogr ACE_ENV_ARG_PARAMETER );
      ACE_CHECK;
      char replica_iogr_filename[200];

      ACE_OS::snprintf(replica_iogr_filename, sizeof(replica_iogr_filename)-1, "%s_%lu.iogr",
        type,
        this->iogr_seq_);
      replica_iogr_filename[sizeof(replica_iogr_filename)-1] = '\0';

      std::cout << "Creator: Writing ior for created object to " << replica_iogr_filename << std::endl;

      if (write_ior_file(replica_iogr_filename, replica_iogr) != 0)
      {
        std::cerr << "Creator: Error writing ior [" << replica_iogr << "] to " << replica_iogr_filename << std::endl;
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

int
main (int argc, char *argv[])
{
  int result = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    TAO::Object_Group_Creator app;
    result = app.parse_args(argc, argv);
    if (result == 0)
    {
      result = app.init (orb ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result == 0)
      {
        result = app.run (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      if (result == 0)
      {
        result = app.fini();
      }
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "Object_Group_Creator::main\t\n");
    result = -1;
  }
  ACE_ENDTRY;
  return result;
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Vector<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

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
#include <orbsvcs/FT_ReplicationManagerC.h>

#include <ace/Get_Opt.h>

TAO::Object_Group_Creator::Object_Group_Creator ()
  : registry_filename_(0)
  , registry_(0)
  , iogr_group_id_(0)
{
}

TAO::Object_Group_Creator::~Object_Group_Creator ()
{
}

int
TAO::Object_Group_Creator::parse_args (int argc, char *argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, "t:f:");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 't':
      {
        this->types_.push_back (get_opts.opt_arg ());
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

  if ( this->types_.size() == 0)
  {
    std::cerr << "Creator: Missing required parameter: -t <type> " << std::endl;
    usage (std::cerr);
    result = -1;
  }

  if ( this->registry_filename_ == 0)
  {
    std::cerr << "Creator: Missing required parameter: -f <FactoryRegistry.ior> " << std::endl;
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
  int result = 0;
  this->orb_ = orb;

////////////////STASH0

  // Find the ReplicationManager.  If found, assume it's our factory
  CORBA::Object_var obj = orb->resolve_initial_references("ReplicationManager");
  PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());  

  /////////////////////////////
  // resolve reference to factory
  CORBA::String_var registry_ior;
  if (read_ior_file(this->registry_filename_, registry_ior))
  {
    CORBA::Object_var registry_obj = this->orb_->string_to_object(registry_ior);
    this->registry_ = PortableGroup::FactoryRegistry::_narrow(registry_obj);
    if (CORBA::is_nil(this->registry_))
    {
      std::cerr << "Creator: Can't resolve FactoryRegistry IOR " << this->registry_filename_ << std::endl;
      result = -1;
    }
  }
  else
  {
    std::cerr << "Creator: Can't read FactoryRegistry IOR " << this->registry_filename_ << std::endl;
    result = -1;
  }

  return result;
}

int TAO::Object_Group_Creator::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;
  size_t typeCount = this->types_.size();
  for ( size_t nType = 0; result == 0 && nType < typeCount; ++nType)
  {
    const char * type = this->types_[nType].c_str();
    result = create_group (type);
  }
  return result;
}

int TAO::Object_Group_Creator::create_group(const char * type)
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
////////////////STASH1
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
      CORBA::Object_var obj = info.the_factory->create_object (
        type,
        info.the_criteria,
        factory_creation_id
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if ( !CORBA::is_nil(obj) )
      {
        const char * replica_ior = orb_->object_to_string (obj ACE_ENV_ARG_PARAMETER );
        ACE_CHECK;

////////////////STASH2

        ////////////////////////////////////
        // Somewhat of a hack
        // guess at factory creation id type
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
      }
      else
      {
        std::cerr << "Creator: create_object returned nil????" << std::endl;
      }
    }

    std::cout << "Creator:  Successfully created group of " << type << std::endl;
////////////////STASH3
  }

  return result;
}


int TAO::Object_Group_Creator::fini ()
{
  return 0;
}



int TAO::Object_Group_Creator::read_ior_file(const char * fileName, CORBA::String_var & ior)
{
  int result = 0;
  FILE *in = ACE_OS::fopen (fileName, "r");
  if (in != 0)
  {
    ACE_OS::fseek(in, 0, SEEK_END);
    size_t fileSize = ACE_OS::ftell(in);
    ACE_OS::fseek(in, 0, SEEK_SET);
    char * buffer;
    ACE_NEW_NORETURN (buffer,
      char[fileSize+1]);
    if (buffer != 0)
    {
      if( fileSize == ACE_OS::fread(buffer, 1, fileSize, in))
      {
        buffer[fileSize] = '\0';
        ior = CORBA::string_dup(buffer);
        ACE_TRY_CHECK;
        result = 1; // success
      }
      delete[] buffer;
    }
  }
  else
  {
    result = 1;
  }
  return result;
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
    std::cout << "***END TRY SCOPE***" << std::endl;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                         "Object_Group_Creator::main\t\n");
    result = -1;
  }
  ACE_ENDTRY;
  std::cout << "***EXIT***" << std::endl;

  return result;
}


#ifdef UNUSED_CODE_STASH
////////////////STASH0
  // Get an object reference for the ORBs IORManipulation object!
  CORBA::Object_var IORM =
    this->orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                      0
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->iorm_ = TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

////////////////STASH1
    // Create IOR list for use with merge_iors.
    TAO_IOP::TAO_IOR_Manipulation::IORList iors (count + 1);

    // create a property set
    TAO_PG::Properties_Encoder encoder;
    PortableGroup::Value value;

    value <<= 99;
    encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

    // allocate and populate the criteria
    FT::Properties_var props_in;
    ACE_NEW_NORETURN (props_in, FT::Properties);
    if (props_in.ptr() == 0)
    {
      ACE_ERROR((LM_ERROR, "Error cannot allocate properties.\n" ));
    }
    else
    {
      encoder.encode(props_in);
    }

    iors.length (count + 1);
    iors [0] = this->object_group_manager_.create_object_group(
                       this->iogr_group_id_,
                       "domain",
                       type,
                       props_in);

////////////////STASH2
        iors [nFact + 1] = obj;

////////////////STASH3
    CORBA::Object_var object_group = this->iorm_->merge_iors (iors ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    const char * iogr = orb_->object_to_string (object_group ACE_ENV_ARG_PARAMETER );
    ACE_CHECK;


    char iogr_filename[200]; // "${type}_${iogr_id}.ior"

    ACE_OS::snprintf(iogr_filename, sizeof(iogr_filename)-1, "%s_%lu.ior",
      type,
      ACE_static_cast(unsigned long, iogr_group_id_));
    iogr_filename[sizeof(iogr_filename)-1] = '\0';

    std::cout << "Creator: Writing iogr for created object to " << iogr_filename << std::endl;

    if (write_ior_file(iogr_filename, iogr) != 0)
    {
      std::cerr << "Creator: Error writing iogr [" << iogr << "] to " << iogr_filename << std::endl;
    }


    this->iogr_group_id_ += 1;

#endif // UNUSED_CODE_STASH



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Vector<ACE_CString>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector<ACE_CString>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    StubConfig.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Simple configuration manager for a fault tolerant application.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#include <iostream>
#include <fstream>

#include <ace/Get_Opt.h>
#include <orbsvcs/PortableGroupC.h>

class StubConfig
{
 public:
  ///////////////////////////
  // construction/destruction
  StubConfig ();

  ~StubConfig ();

  /////////////////
  // initialization
  int parse_args (int argc, char *argv[]);

  int init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL);

  ////////////
  // execution
  int run (ACE_ENV_SINGLE_ARG_DECL);

  ////////////
  // shut down
  int fini ();

  /////////////////
  // implementation
private:
  void usage (ostream & out)const;
  int read_ior_file(const char * fileName, CORBA::String_var & ior);
  int write_ior_file(const char * outputFile, const char * ior);

  ////////////////////
  // forbidden methods
 private:
    StubConfig (const StubConfig & rhs);
    StubConfig & operator = (const StubConfig & rhs);

  ////////////////
  // Data members
 private:
  CORBA::ORB_var orb_;
  const char * registry_filename_;
  PortableGroup::FactoryRegistry_var registry_;
  const char * type_id_;
};


StubConfig::StubConfig ()
  : registry_filename_(0)
  , registry_(0)
  , type_id_(0)
{
}

StubConfig::~StubConfig ()
{
}

int
StubConfig::parse_args (int argc, char *argv[])
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
        this->type_id_ = get_opts.opt_arg ();
        break;
      }
      case 'f':
      {
        this->registry_filename_ = get_opts.opt_arg ();
        break;
      }

      default:
      {
        std::cerr << "Config: Unknown argument -" << (char) c << std::endl;
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

  if ( this->type_id_ == 0)
  {
    std::cerr << "Config: Missing required parameter: -t <type_id> " << std::endl;
    usage (std::cerr);
    result = -1;
  }

  if ( this->registry_filename_ == 0)
  {
    std::cerr << "Config: Missing required parameter: -f <FactoryRegistry.ior> " << std::endl;
    usage (std::cerr);
    result = -1;
  }

  return result;
}

void StubConfig::usage(ostream & out)const
{
  out << "usage"
      << " -t <type_id for objects to be created>"
      << " -f <factory ior file>"
      << std::endl;
}



int StubConfig::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 0;
  this->orb_ = orb;

  /////////////////////////////
  // resolve reference to factory
  CORBA::String_var registry_ior;
  if (read_ior_file(this->registry_filename_, registry_ior))
  {
    CORBA::Object_var registry_obj = this->orb_->string_to_object(registry_ior);
    this->registry_ = PortableGroup::FactoryRegistry::_narrow(registry_obj);
    if (CORBA::is_nil(this->registry_))
    {
      std::cerr << "Config: Can't resolve FactoryRegistry IOR " << this->registry_filename_ << std::endl;
      result = -1;
    }
  }
  else
  {
    std::cerr << "Config: Can't read FactoryRegistry IOR " << this->registry_filename_ << std::endl;
    result = -1;
  }

  return result;
}

int StubConfig::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;


  ::PortableGroup::FactoryInfos_var infos = this->registry_->list_factories_by_type (this->type_id_
      ACE_ENV_ARG_PARAMETER)
  ACE_CHECK;

  CORBA::ULong count = infos->length();
  std::cout << "Config: found " << count << " factories for " << this->type_id_ << std::endl;

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
      std::cout << "Config: Creating " << type_id_ << " at " << loc_name << std::endl;

      PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;
      CORBA::Object_var obj = info.the_factory->create_object (
        this->type_id_,
        info.the_criteria,
        factory_creation_id
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if ( !CORBA::is_nil(obj) )
      {
        const char * replica_ior = orb_->object_to_string (obj ACE_ENV_ARG_PARAMETER );
        ACE_CHECK;

        ACE_CString replica_ior_filename(loc_name); // "$(location1)_$type_id.ior"
        replica_ior_filename += "_";
        replica_ior_filename += this->type_id_;
        replica_ior_filename += ".ior";
        std::cout << "Config: Writing ior for created object to " << replica_ior_filename.c_str() << std::endl;

        if (write_ior_file(replica_ior_filename.c_str(), replica_ior) != 0)
        {
          std::cerr << "Config: Error writing ior [" << replica_ior << "] to " << replica_ior_filename.c_str() << std::endl;
        }
      }
      else
      {
        std::cerr << "Config: create_object returned nil????" << std::endl;
      }
  }
  return result;
}


int StubConfig::fini ()
{
  return 0;
}



int StubConfig::read_ior_file(const char * fileName, CORBA::String_var & ior)
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

int StubConfig::write_ior_file(const char * outputFile, const char * ior)
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
    StubConfig app;
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
                         "StubConfig::main\t\n");
    result = -1;
  }
  ACE_ENDTRY;
  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

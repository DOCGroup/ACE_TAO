/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    TAO_Object_Group_Creator.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Main wrapped around TAO_Object_Group_Creator
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "FT_Creator.h"
#include <iostream>
#include <fstream>
#include <orbsvcs/PortableGroup/PG_Properties_Encoder.h>

#include <ace/Get_Opt.h>


FTAPP::FT_Creator::FT_Creator ()
  : registry_ior_ (0)
  , registry_ (0)
  , replication_manager_ (0)
  , have_replication_manager_ (0)
  , write_iors_ (0)
  , write_iogr_ (0)
  , iogr_seq_ (0)
  , ns_register_ (1)
  , prefix_ ("")
{
}

FTAPP::FT_Creator::~FT_Creator ()
{
}

int
FTAPP::FT_Creator::parse_args (int argc, char *argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, "r:ignf:u:p:");
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'r':
      {
        this->create_roles_.push_back (get_opts.opt_arg ());
        break;
      }
      case 'u':
      {
        this->unregister_roles_.push_back (get_opts.opt_arg ());
        break;
      }
      case 'f':
      {
        this->registry_ior_ = get_opts.opt_arg ();
        break;
      }

      case 'g':
      {
        this->write_iogr_ = !this->write_iogr_;
        break;
      }

      case 'i':
      {
        this->write_iors_ = ! this->write_iors_;
        break;
      }

      case 'n':
      {
        this->ns_register_ = !this->ns_register_;
        break;
      }

      case 'p':
      {
        this->prefix_ = get_opts.opt_arg();
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

  if ( this->create_roles_.size() == 0 && this->unregister_roles_.size() == 0)
  {
    std::cerr << "Creator: neither create (-t) nor kill (-u) specified.  Nothing to do." << std::endl;
    usage (std::cerr);
    result = -1;
  }

  return result;
}

void FTAPP::FT_Creator::usage(ostream & out)const
{
  out << "usage\n"
      << " -r <role for objects to be created>\n"
      << " -f <factory registry ior file> (if not specified, ReplicationManager is used.)\n"
      << " -u <role to be unregistered (for testing factory registry)>\n"
      << " -i (toggle write ior for each object (default false))\n"
      << " -p <prefix for registration & file names>\n"
      << " -g (toggle write iogr to file (default false))\n"
      << " -n (toggle register iogr with name service (default true))\n"
      ;
}



int FTAPP::FT_Creator::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 1;
  this->orb_ = orb;

  // if a factory IOR was specified on command line
  if ( this->registry_ior_ != 0)
  {
    result = this->creator_.set_factory_registry(this->registry_ior_  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  result = this->creator_.init (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->ns_register_)
  {
    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    if (CORBA::is_nil(naming_obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }
    this->naming_context_=
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  return result;
}

int FTAPP::FT_Creator::run (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;
  size_t typeCount = this->create_roles_.size();
  for ( size_t nType = 0; result == 0 && nType < typeCount; ++nType)
  {
    const char * role = this->create_roles_[nType].c_str();
    std::cout << std::endl << "Creator: Creating group of " << role << std::endl;
    PortableGroup::ObjectGroup_var group = this->creator_.create_group (
      role,
      this->write_iors_
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (1);

    if (this->write_iogr_)
    {
      CORBA::String_var iogr = this->orb_->object_to_string (group ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (1);

      char iogr_filename[1000];
      ACE_OS::snprintf (iogr_filename, sizeof(iogr_filename)-1, "%s%s_%d.iogr",
        this->prefix_,
        role,
        this->iogr_seq_);
      FILE * iogr_file = fopen (iogr_filename, "w");
      if (iogr_file != 0)
      {
        char const * siogr = ACE_static_cast (const char *, iogr);
        fwrite (siogr, 1, strlen(siogr), iogr_file);
        fclose (iogr_file);
      }
      else
      {
        std::cerr << "Can't open iogr output file " << iogr_filename << std::endl;
        result = 1;
      }
    }

    if(this->ns_register_)
    {
      char iogr_name[1000];
      ACE_OS::snprintf (iogr_name, sizeof(iogr_name)-1, "%s_%s_%d",
        this->prefix_,
        role,
        this->iogr_seq_);

      CosNaming::Name this_name (1);
      this_name.length (1);
      this_name[0].id = CORBA::string_dup (iogr_name);

      this->naming_context_->rebind (this_name, group.in()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (1);
    }

    iogr_seq_ += 1;

  }

  typeCount = this->unregister_roles_.size();
  for ( nType = 0; result == 0 && nType < typeCount; ++nType)
  {
    const char * role = this->unregister_roles_[nType].c_str();
    result = this->creator_.unregister_role (role);
  }

  return result;
}

int FTAPP::FT_Creator::fini ()
{
  return this->creator_.fini();
}

int
main (int argc, char *argv[])
{
  int result = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    FTAPP::FT_Creator app;
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
                         "FT_Creator::main\t\n");
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

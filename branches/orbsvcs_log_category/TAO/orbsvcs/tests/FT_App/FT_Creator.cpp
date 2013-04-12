/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_Creator.cpp
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
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include <orbsvcs/PortableGroup/PG_Properties_Encoder.h>

#include <ace/Get_Opt.h>
#include <ace/OS_NS_stdio.h>

FTAPP::FT_Creator::FT_Creator ()
  : creator_ ()
  , orb_ (CORBA::ORB::_nil ())
  , registry_ior_(0)
  , replication_manager_ (::FT::ReplicationManager::_nil ())
  , have_replication_manager_ (0)
  , write_iors_ (0)
  , write_iogr_ (0)
  , ns_register_ (1)
  , iogr_seq_ (0)
  , prefix_ ("")
{
}

FTAPP::FT_Creator::~FT_Creator ()
{
}

int
FTAPP::FT_Creator::parse_args (int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("r:ignf:u:p:"));
  int c;

  while (result == 0 && (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'r':
      {
        this->create_roles_.push_back (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      }
      case 'u':
      {
        this->unregister_roles_.push_back (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
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
        this->prefix_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg());
        break;
      }

      default:
      {
        ACE_OS::fprintf (stderr, "Creator: Unknown argument -%c\n", (char) c);
        usage(stderr);
        result = 1;
        break;
      }
      case '?':
      {
        usage(stderr);
        result = 1;
        break;
      }
    }
  }

  if ( this->create_roles_.size() == 0 && this->unregister_roles_.size() == 0)
  {
    //FUZZ: disable check_for_lack_ACE_OS
    ACE_OS::fprintf (stderr, "Creator: neither create (-t) nor kill (-u) specified.  Nothing to do.\n");
    //FUZZ: enable check_for_lack_ACE_OS
    usage (stderr);
    result = -1;
  }

  return result;
}

void FTAPP::FT_Creator::usage(FILE* out)const
{
  ACE_OS::fprintf (out, "usage\n"
                        " -r <role for objects to be created>\n"
                        " -f <factory registry ior file> (if not specified, ReplicationManager is used.)\n"
                        " -u <role to be unregistered (for testing factory registry)>\n"
                        " -i (toggle write ior for each object (default false))\n"
                        " -p <prefix for registration & file names>\n"
                        " -g (toggle write iogr to file (default false))\n"
                        " -n (toggle register iogr with name service (default true))\n");
}



int FTAPP::FT_Creator::init (CORBA::ORB_ptr orb)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  // if a factory IOR was specified on command line
  if ( this->registry_ior_ != 0)
  {
    CORBA::Object_var registry_obj
      = this->orb_->string_to_object (this->registry_ior_);
    PortableGroup::FactoryRegistry_var registry
      = PortableGroup::FactoryRegistry::_narrow(registry_obj.in ());
    if (! CORBA::is_nil (registry.in ()))
    {
      result = this->creator_.set_factory_registry(registry.in());
    }
  }

  if (result == 0)
  {
    result = this->creator_.init (orb);
  }


  if (result == 0 && this->ns_register_)
  {
    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService");

    if (CORBA::is_nil(naming_obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }
    this->naming_context_=
      CosNaming::NamingContext::_narrow (naming_obj.in ());
  }

  return result;
}

int FTAPP::FT_Creator::run (void)
{
  int result = 0;
  size_t typeCount = this->create_roles_.size();
  size_t nType = 0;
  for ( nType = 0; result == 0 && nType < typeCount; ++nType)
  {
    const char * role = this->create_roles_[nType].c_str();
    ACE_OS::fprintf (stdout, "\nCreator: Creating group of %s\n", role);
    PortableGroup::ObjectGroup_var group = this->creator_.create_group (
      role,
      this->write_iors_);

    if (this->write_iogr_)
    {
      CORBA::String_var iogr = this->orb_->object_to_string (group.in ());

      char iogr_filename[1000];
      ACE_OS::snprintf (iogr_filename, sizeof(iogr_filename),
        "%s%s_" ACE_SIZE_T_FORMAT_SPECIFIER_ASCII ".iogr",
        this->prefix_.c_str (),
        role,
        this->iogr_seq_);
      FILE * iogr_file = ACE_OS::fopen (iogr_filename, "w");
      if (iogr_file != 0)
      {
        char const * siogr = static_cast<const char *> (iogr.in ());
        ACE_OS::fwrite (siogr, 1, ACE_OS::strlen(siogr), iogr_file);
        ACE_OS::fclose (iogr_file);
      }
      else
      {
        ACE_OS::fprintf (stderr, "Can't open iogr output file %s\n", iogr_filename);
        result = 1;
      }
    }

    if(this->ns_register_)
    {
      char iogr_name[1000];
      ACE_OS::snprintf (iogr_name, sizeof(iogr_name),
        "%s_%s_" ACE_SIZE_T_FORMAT_SPECIFIER_ASCII,
        this->prefix_.c_str (),
        role,
        this->iogr_seq_);

      CosNaming::Name this_name (1);
      this_name.length (1);
      this_name[0].id = CORBA::string_dup (iogr_name);

      this->naming_context_->rebind (this_name, group.in());
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    FTAPP::FT_Creator app;
    result = app.parse_args(argc, argv);
    if (result == 0)
    {
      result = app.init (orb.in ());
      if (result == 0)
      {
        result = app.run ();
      }
      if (result == 0)
      {
        result = app.fini();
      }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("FT_Creator::main\t\n");
    result = -1;
  }
  return result;
}

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
  : registry_ior_(0)
  , registry_(0)
  , replication_manager_(0)
  , have_replication_manager_(0)
  , make_iogr_(0)
  , write_iors_(1)
  , iogr_seq_(0)
{
}

FTAPP::FT_Creator::~FT_Creator ()
{
}

int
FTAPP::FT_Creator::parse_args (int argc, char *argv[])
{
  int result = 0;

  ACE_Get_Opt get_opts (argc, argv, "r:f:u:gi");
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
        make_iogr_ = !make_iogr_;
        break;
      }

      case 'i':
      {
        write_iors_ = ! write_iors_;
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

  if ( this->create_roles_.size() == 0 && this->unregister_roles_.size())
  {
    std::cerr << "Creator: neither create (-t) nor kill (-k) specified.  Nothing to do." << std::endl;
    usage (std::cerr);
    result = -1;
  }

  return result;
}

void FTAPP::FT_Creator::usage(ostream & out)const
{
  out << "usage"
      << " -r <role for objects to be created>"
      << " -f <factory registry ior file> (if not specified, ReplicationManager is used.)"
      << " -u <role to be unregistered (for testing factory registry)>"
      << " -g (toggle write iogr for each group (default is not to write iogrs))"
      << " -i (toggle write ior for each object (default is to write iors))"
      << std::endl;
}



int FTAPP::FT_Creator::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 1;
  this->orb_ = orb;

  // if a factory IOR was specified on command line
  if ( this->registry_ior_ != 0)
  {
    result = this->creator_.set_factory_registry(this->registry_ior_  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  result = this->creator_.init (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

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
    result = this->creator_.create_group (role, this->write_iors_, this->make_iogr_);
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

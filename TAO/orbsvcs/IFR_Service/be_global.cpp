
//=============================================================================
/**
 *  @file    be_global.cpp
 *
 *  Stores global data specific to the compiler back end.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Log_Macros.h"
#include "be_global.h"
#include "ast_generator.h"
#include "global_extern.h"
#include "idl_defines.h"

TAO_IFR_BE_Export BE_GlobalData *be_global = 0;

BE_GlobalData::BE_GlobalData ()
  : removing_ (false),
    filename_ (0),
    enable_locking_ (false),
    do_included_files_ (true),
    allow_duplicate_typedefs_ (false)
{
  // At this point, the FE has been initialized.  We can
  // now instruct it that we want to preserve c++ keywords.
  idl_global->preserve_cpp_keywords (true);
}

BE_GlobalData::~BE_GlobalData ()
{
}

bool
BE_GlobalData::removing () const
{
  return this->removing_;
}

void
BE_GlobalData::removing (bool value)
{
  this->removing_ = value;
}

CORBA::ORB_ptr
BE_GlobalData::orb () const
{
  return this->orb_.in ();
}

void
BE_GlobalData::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = orb;
}

CORBA::Repository_ptr
BE_GlobalData::repository () const
{
  return this->repository_.in ();
}

void
BE_GlobalData::repository (CORBA::Repository_ptr repo)
{
  this->repository_ = repo;
}

ACE_Unbounded_Stack<CORBA::Container_ptr> &
BE_GlobalData::ifr_scopes ()
{
  return this->ifr_scopes_;
}

void
BE_GlobalData::destroy ()
{
}

const char *
BE_GlobalData::filename () const
{
  return this->filename_;
}

void
BE_GlobalData::filename (char *fname)
{
  this->filename_ = fname;
}

bool
BE_GlobalData::enable_locking () const
{
  return this->enable_locking_;
}

void
BE_GlobalData::enable_locking (bool value)
{
  this->enable_locking_ = value;
}

bool
BE_GlobalData::do_included_files () const
{
  return this->do_included_files_;
}

void
BE_GlobalData::do_included_files (bool val)
{
  this->do_included_files_ = val;
}

bool
BE_GlobalData::allow_duplicate_typedefs () const
{
  return this->allow_duplicate_typedefs_;
}

void
BE_GlobalData::allow_duplicate_typedefs (bool val)
{
  this->allow_duplicate_typedefs_ = val;
}

ACE_CString
BE_GlobalData::orb_args () const
{
  return this->orb_args_;
}

void
BE_GlobalData::orb_args (const ACE_CString& args)
{
  this->orb_args_ = args;
}

ACE_CString
BE_GlobalData::spawn_options ()
{
  return this->orb_args_ + idl_global->idl_flags ();
}

#define UNKNOWN_OPTION \
  ORBSVCS_ERROR (( \
      LM_ERROR, \
      ACE_TEXT ("IDL: I don't understand the '%s' option\n"), \
      av[i])); \
  idl_global->parse_args_exit (1);

void
BE_GlobalData::parse_args (long &i, char **av)
{
  switch (av[i][1])
    {
      case 'L':
        if (av[i][2] == '\0')
          {
            be_global->enable_locking (true);
          }
        else
          {
            UNKNOWN_OPTION;
          }
        break;

      case 'r':
        if (av[i][2] == '\0')
          {
            be_global->removing (true);
          }
        else
          {
            UNKNOWN_OPTION;
          }
        break;

      case 'S':
        // Suppress ...
        if (av[i][2] == 'i' && av[i][3] == '\0')
          {
            // ... processing of included IDL files.
            be_global->do_included_files (0);
          }
        else
          {
            UNKNOWN_OPTION;
          }
        break;

      case 'T':
        if (av[i][2] == '\0')
          {
            be_global->allow_duplicate_typedefs (true);
          }
        else
          {
            UNKNOWN_OPTION;
          }
        break;

      default:
        UNKNOWN_OPTION;
    }
}


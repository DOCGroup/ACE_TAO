// $Id$

// ============================================================================
//
// = FILENAME
//    QuoterFactoryFinder.cpp
//
// = DESCRIPTION
//    A Factory Finder for the Quoter example. This example conforms
//    to the CosLifeCycle Factory Finder notion.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "QuoterFactoryFinder.h"


static const char usage [] = 
"[-? |\n[-O[RBport] ORB port number]]";

QuoterFactoryFinder_Server::QuoterFactoryFinder_Server () 
{
}

QuoterFactoryFinder_Server::~QuoterFactoryFinder_Server () 
{
}

QuoterFactoryFinder_Server::init (int argc, char *argv[], CORBA::Environment& env)
{
  if (this->orb_manager_.init (argc,
                               argv,
                               env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "init"),
                      -1);

  // Copy them, because parse_args expects them there.
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();

  QuoterFactoryFinder_i quoterFactoryFinder_Impl;
  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (&quoterFactoryFinder_Impl,
                                 env);

  quoterFactoryFinder_Impl_ptr_ = &quoterFactoryFinder_Impl;

/*  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  if (this->ior_output_file_)
  {
    ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
    ACE_OS::fclose (this->ior_output_file_);
  }
   
  if (this->use_naming_service_)
    return this->init_naming_service (env);

  */
  return 0;
}

int
QuoterFactoryFinder_Server::run (CORBA::Environment& env)
{
  if (orb_manager_.orb()->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"),
                      -1);
  return 0;
}


// Function get_options.

unsigned int
QuoterFactoryFinder_Server::parse_args ()
{
  // We need the 'O' in get_opt () because we also want to have ORB
  // parameters, they all start with 'O'.
  ACE_Get_Opt get_opt (this->argc_, this->argv_, "O?");
  int opt;

  while ((opt = get_opt ()) != EOF)
    switch (opt) 
      {
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    "Usage: %s %s\n",
                    this->argv_[0], usage));
        ACE_OS::exit (0);
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: unknown arg, -%c\n"
                           "Usage: %s %s\n",
                           this->argv_[0], char (opt),
                           this->argv_[0],
                           usage),
                          1);
      }

  if (this->argc_ != get_opt.optind)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: too many arguments\n"
                       "Usage: %s %s\n",
                       this->argv_[0],
                       this->argv_[0],
                       usage),
                      1);
  return 0;
}

// function main

int
main (int argc, char *argv [])
{
  QuoterFactoryFinder_Server quoterFactoryFinder_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Quoter:QuoterFactoryFinder \n \n"));
  TAO_TRY
    {
      if (quoterFactoryFinder_server.init (argc,argv,TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          quoterFactoryFinder_server.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

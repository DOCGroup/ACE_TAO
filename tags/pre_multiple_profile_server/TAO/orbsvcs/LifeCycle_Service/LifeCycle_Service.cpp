// $Id$

// ============================================================================
//
// = FILENAME
//    Life_Cycle_Service.cpp
//
// = DESCRIPTION
//    The server for the LifeCycleService of the quoter example.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "tao/corba.h"
#include "LifeCycle_Service.h"

ACE_RCSID(LifeCycle_Service, LifeCycle_Service, "$Id$")

static const char usage [] = "[-? |\n[-O[RBport] ORB port number]]";

Life_Cycle_Service_Server::Life_Cycle_Service_Server (void)
{
}

Life_Cycle_Service_Server::~Life_Cycle_Service_Server (void)
{
  ACE_TRY_NEW_ENV
    {
      // Unbind the Factory Finder.
      CosNaming::Name generic_Factory_Name (2);
      generic_Factory_Name.length (2);
      generic_Factory_Name[0].id = CORBA::string_dup ("LifeCycle_Service");
      this->namingContext_var_->unbind (generic_Factory_Name, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "User Exception");
    }
  ACE_ENDTRY;
}

int
Life_Cycle_Service_Server::init (int argc,
                                     char *argv[],
                                     CORBA::Environment& ACE_TRY_ENV)
{
  if (this->orb_manager_.init (argc,
                               argv,
                               ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init"),
                      -1);
  // @@ Oh well.  This should actually come before "if".
  ACE_CHECK_RETURN (-1);

  // Copy them, because parse_args expects them there.
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();


  ACE_NEW_RETURN (this->life_Cycle_Service_i_ptr_,
                  Life_Cycle_Service_i(),
                  -1);

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->life_Cycle_Service_i_ptr_,
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Failure while activating the  Factory Finder object

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  // Register the LifeCycle Service with the Naming Service.
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Trying to get a reference to the Naming Service.\n"));

      // Get the Naming Service object reference.
      CORBA::Object_var namingObj_var =
        orb_manager_.orb()->resolve_initial_references ("NameService", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingObj_var.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      // Narrow the object reference to a Naming Context.
      namingContext_var_ = CosNaming::NamingContext::_narrow (namingObj_var.in (),
							      ACE_TRY_ENV);

      ACE_TRY_CHECK;

      if (CORBA::is_nil (namingContext_var_.in ()))
        ACE_ERROR ((LM_ERROR,
                   " (%P|%t) Unable get the Naming Service.\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "Have a proper reference to the Naming Service.\n"));

      CosNaming::Name life_Cycle_Service_Name (1);
      life_Cycle_Service_Name.length (1);
      life_Cycle_Service_Name[0].id = CORBA::string_dup ("Life_Cycle_Service");

      CORBA::Object_ptr tmp = this->life_Cycle_Service_i_ptr_->_this(ACE_TRY_ENV);
      ACE_TRY_CHECK;

      namingContext_var_->bind (life_Cycle_Service_Name,
                                tmp,
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Bound the LifeCycle Service to the Naming Context.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Life_Cycle_Service_Server::init");
    }
  ACE_ENDTRY;

  return 0;
}


int
Life_Cycle_Service_Server::run (CORBA::Environment &)
{
  
  if (orb_manager_.orb()->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"),
                      -1);
  return 0;
}


// Function get_options.

u_int
Life_Cycle_Service_Server::parse_args (void)
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
  return 0;
}

// function main

int
main (int argc, char *argv [])
{
  Life_Cycle_Service_Server life_Cycle_Service_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_LifeCycleService: Life_Cycle_Service_Server \n\n"));
  ACE_TRY_NEW_ENV
    {
      int check = life_Cycle_Service_Server.init (argc,
                                                  argv,
                                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (check)
        return 1;
      else
        {
          life_Cycle_Service_Server.run (ACE_TRY_ENV);
          ACE_TRY_CHECK
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "main");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}



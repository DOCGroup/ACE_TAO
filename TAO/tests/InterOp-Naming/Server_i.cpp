//$Id$

#include "Server_i.h"
#include "tao/debug.h"
#include "tao/IORTable/IORTable.h"

// Constructor.

Server_i::Server_i (void)
  : ior_output_file_ (0),
    ins_ (0)
{
  // no-op.
}

// Destructor.

Server_i::~Server_i (void)
{
}

// Parse the command-line arguments and set options.

int
Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:ni:");
  int c = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;

      case 'i': // For Testing the InterOperable Naming Service.
        this->ins_ = CORBA::string_dup (get_opts.opt_arg ());
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           " [-n] "
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Add the ObjectID:IOR mapping to the IOR table of
// the ORB.

int
Server_i::add_IOR_to_table (CORBA::String_var ior)
{
  ACE_TRY_NEW_ENV
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Adding (KEY:IOR) %s:%s\n",
                    this->ins_,
                    ior.in ()));

      CORBA::Object_var table_object =
        this->orb_manager_.orb ()->resolve_initial_references (
            "IORTable"
            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      adapter->bind (this->ins_, ior.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Exception caugh in add_IOR_to_table");
    }
  ACE_ENDTRY;

  return 0;
}

// Initialize the server.
int
Server_i::init (int argc,
                char *argv[]
                TAO_ENV_ARG_DECL)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa"
                                         TAO_ENV_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);

  ACE_CHECK_RETURN (-1);

  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  ACE_TRY
    {
      CORBA::String_var str  =
        this->orb_manager_.activate_under_child_poa ("INS_servant",
                                                     &this->servant_
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%s>\n",
                  str.in ()));

      if (this->ins_)
        if (this->add_IOR_to_table (str) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "test_for_ins (): failed\n"),
                            -1);

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s",
                           str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "\tException in activation of POA");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Server_i::run (TAO_ENV_SINGLE_ARG_DECL)
{
  // Run the main event loop for the ORB.
  if (this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);

  return 0;
}

// $Id$

#ifndef SIMPLE_UTIL_C
#define SIMPLE_UTIL_C

#include "Simple_util.h"
#include "tao/IORTable/IORTable.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_fcntl.h"

// Constructor.

template <class Servant>
Server<Servant>::Server (void)
  : ior_output_file_ (0),
    naming_ (0),
    ins_ (0)
{
  // no-op.
}

// Destructor.

template <class Servant>
Server<Servant>::~Server (void)
{
}

// Parse the command-line arguments and set options.

template <class Servant> int
Server<Servant>::parse_args (void)
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
                             ACE_TEXT_CHAR_TO_TCHAR (get_opts.opt_arg ())), -1);
        break;

      case 'n': //Use naming service
        this->naming_ = 1;
        break;
      case 'i': // For Testing the InterOperable Naming Service.
        this->ins_ = CORBA::string_dup (get_opts.opt_arg ());
        break;
      case 'h':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d (debug)]"
                           " [-o] <ior_output_file>"
                           " [-n (use naming service)]"
                           " [-i] <InterOperable Naming Service simple object key>"
                           " [-h (help)]"
                           "\n",
                           ACE_TEXT_CHAR_TO_TCHAR (this->argv_ [0])),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Add the ObjectID:IOR mapping to the IOR table of
// the ORB. Ignore this method if you are not testing for
// the InterOperable Naming Service.

template <class Servant> int
Server<Servant>::test_for_ins (CORBA::String_var ior)
{
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Adding (KEY:IOR) %s:%s\n",
                ACE_TEXT_CHAR_TO_TCHAR (this->ins_),
                ACE_TEXT_CHAR_TO_TCHAR (ior.in ())));


  try
    {
      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }

      adapter->bind (this->ins_, ior.in ());
    }
  catch (const CORBA::Exception&)
    {
    }

  return 0;
}

// Initialize the server.
template <class Servant> int
Server<Servant>::init (const char *servant_name,
                       int argc,
                       char *argv[])
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);


  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  if (this->naming_ == 1)
    {

      // Save name in case we use TAO Naming Service.
      name = servant_name;

      // Call naming service
      if (this->register_name () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\n Naming Service\n"),-1);

      return 0;
    }

  // Activate the servant in its own child POA.

  // Make sure that you check for failures here via the ACE_TRY
  // macros?!
  try
    {
      CORBA::String_var str  =
        this->orb_manager_.activate_under_child_poa (servant_name,
                                                     &this->servant_);

      ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%s>\n",
                  ACE_TEXT_CHAR_TO_TCHAR (str.in ())));

      if (this->ins_)
        if (this->test_for_ins (str) != 0)
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "\tException in activation of POA");
      return -1;
    }

  return 0;
}

template <class Servant>int
Server<Servant>::run (void)
{
  // Run the main event loop for the ORB.
  if (this->orb_manager_.run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);

  this->orb_manager_.fini ();

  return 0;
}

template <class Servant> int
Server<Servant>::register_name (void)
{
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  if (this->naming_client_.init (orb.in ()) == -1)
    return -1;

  // create the name for the naming service
  CosNaming::Name bindName;
  bindName.length (1);
  bindName[0].id = CORBA::string_dup (name);


  // (re)Bind the object.
  try
    {
      CORBA::Object_var object = servant_._this ();

      this->orb_manager_.activate_poa_manager ();

      naming_client_->rebind (bindName,
                              object.in());

      // Test for INS.
      if (this->ins_)
        {
          CORBA::String_var ior =
            orb->object_to_string (object.in ());
          if (this->test_for_ins (ior.in ()) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "test_for_ins (): failed\n"),
                              -1);
        }
    }
  catch (const CosNaming::NamingContext::AlreadyBound&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to bind %s \n",
                         ACE_TEXT_CHAR_TO_TCHAR (name)),
                        -1);
    }

  return 0;
}

// Constructor.

template <class INTERFACE_OBJECT, class Var>
Client<INTERFACE_OBJECT, Var>::Client (void)
  : ior_ (0),
    shutdown_ (0)
{
  //no-op
}

// Reads the Server ior from a file

template <class INTERFACE_OBJECT, class Var> int
Client<INTERFACE_OBJECT, Var>::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       ACE_TEXT_CHAR_TO_TCHAR (filename)),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);

  this->ior_ = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

template <class INTERFACE_OBJECT, class Var> int
Client<INTERFACE_OBJECT, Var>::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "df:nk:x");
  int c = 0;
  int result = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'n': // Use naming service
        this->naming_ = 1;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             ACE_TEXT_CHAR_TO_TCHAR (get_opts.opt_arg ())),
                            -1);
        break;
      case 'x': // read the flag for shutting down
        this->shutdown_ = 1;
        break;
      case 'h':  // display help for use of the client.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d (debug)]"
                           " [-k] <ior>"
                           " [-f] <ior_output_file>"
                           " [-n (use naming service)]"
                           " [-x (shutdown server)]"
                           " [-h (help)]"
                           "\n",
                           ACE_TEXT_CHAR_TO_TCHAR (this->argv_ [0])),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

template <class INTERFACE_OBJECT, class Var>
Client<INTERFACE_OBJECT, Var>::~Client (void)
{
  ACE_OS::free (this->ior_);

  this->orb_->destroy ();
}

template <class INTERFACE_OBJECT, class Var> int
Client<INTERFACE_OBJECT, Var>::init (const char *name,
                                 int argc,
                                 char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if(this->ior_ != 0)
        {
          CORBA::Object_var server_object =
            this->orb_->string_to_object (this->ior_);


          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%s>\n",
                               ACE_TEXT_CHAR_TO_TCHAR (this->ior_)),
                              -1);
          this->server_ = INTERFACE_OBJECT::_narrow (server_object.in ());
        }
      else if (this->naming_ == 1)
        {
          // No IOR specified. Use the Naming Service
          ACE_DEBUG((LM_DEBUG,
                     "Using the Naming Service \n"));
          this->name_ = const_cast<char *> (name);
          int retv = this->obtain_initial_references ();
          if (retv ==-1)
            return -1;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "no ior or naming options  specified\n"),
                          -1);


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client_i::init");
      return -1;
    }


  return 0;
}


template <class INTERFACE_OBJECT, class Var> int
Client<INTERFACE_OBJECT, Var>::obtain_initial_references (void)
{

  try
    {
      // Initialize the naming services.
      if (naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);


      CosNaming::Name server_name (1);
      server_name.length (1);
      server_name[0].id =
      CORBA::string_dup (this->name_);
      CORBA::Object_var obj =
        naming_client_->resolve (server_name);

      this->server_ = INTERFACE_OBJECT::_narrow (obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
       ex._tao_print_exception (
         "Client::obtain_initial_references");
      return -1;
    }

  return 0;
}

template <class INTERFACE_OBJECT, class Var> int
Client<INTERFACE_OBJECT, Var>::shutdown (void)
{
  // Returns the shutdwon flag
  return shutdown_;
}

template <class INTERFACE_OBJECT, class Var> void
Client<INTERFACE_OBJECT, Var>::shutdown (int flag)
{
  // Fills the flag
  shutdown_ = flag;
}

#endif /* SIMPLE_UTIL_C */

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
  : servant_ (0)
  , ior_output_file_ (0)
  , naming_ (0)
  , ins_ (0)
{
  Servant *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Servant (),
                    CORBA::NO_MEMORY ());
  this->servant_ = tmp;
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
  ACE_Get_Opt get_opts (this->argc_, this->argv_,
                        ACE_TEXT ("do:ni:"), 1, 0,
                        ACE_Get_Opt::REQUIRE_ORDER);
  int c = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      case 'n': //Use naming service
        this->naming_ = 1;
        break;
      case 'i': // For Testing the InterOperable Naming Service.
        this->ins_ = get_opts.opt_arg ();
        break;
      case 'h':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s")
                           ACE_TEXT (" [-d (debug)]")
                           ACE_TEXT (" [-o] <ior_output_file>")
                           ACE_TEXT (" [-n (use naming service)]")
                           ACE_TEXT (" [-i] <InterOperable Naming Service simple object key>")
                           ACE_TEXT (" [-h (help)]")
                           ACE_TEXT ("\n"),
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Add the ObjectID:IOR mapping to the IOR table of
// the ORB. Ignore this method if you are not testing for
// the InterOperable Naming Service.
template <class Servant> int
Server<Servant>::test_for_ins (const char *ior)
{
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Adding (KEY:IOR) %s:%C\n"),
                this->ins_,
                ior));

  try
    {
      CORBA::Object_var table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil IORTable\n")),
                            -1);
        }

      adapter->bind (ACE_TEXT_ALWAYS_CHAR (this->ins_), ior);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR: test_for_ins failed\n");
      return -1;
    }

  return 0;
}

// Initialize the server.
template <class Servant> int
Server<Servant>::init (const char *servant_name,
                       int argc,
                       ACE_TCHAR *argv[])
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("init_child_poa")),
                      -1);

  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_->orb (orb.in ());

  if (this->naming_ == 1)
    {
      // Call naming service
      if (this->register_name (servant_name) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("\n Naming Service\n")),
                          -1);

      return 0;
    }

  // Activate the servant in its own child POA.

  // Make sure that you check for failures here via the try?!
  try
    {
      CORBA::String_var str  =
        this->orb_manager_.activate_under_child_poa (servant_name,
                                                     this->servant_.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("The IOR is: <%C>\n"),
                  str.in ()));

      if (this->ins_ && this->test_for_ins (str.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("test_for_ins (): failed\n")),
                          -1);

      if (this->ior_output_file_)
        {
          FILE *fh = ACE_OS::fopen (this->ior_output_file_, "w");
          if (fh == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Unable to open %s for writing (%p)\n"),
                               this->ior_output_file_,
                               ACE_TEXT ("fopen")),
                              -1);
          ACE_OS::fprintf (fh, "%s", str.in ());
          ACE_OS::fclose (fh);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\tException in activation of POA");
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
                       ACE_TEXT ("Server_i::run")),
                      -1);

  return 0;
}

template <class Servant> int
Server<Servant>::register_name (const char *name)
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
      CORBA::Object_var object = servant_->_this ();

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
                               ACE_TEXT ("test_for_ins (): failed\n")),
                              -1);
        }
    }
  catch (const CosNaming::NamingContext::AlreadyBound&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to bind %C\n"),
                         name),
                        -1);
    }

  return 0;
}

// Constructor.
template <class ServerInterface>
Client<ServerInterface>::Client (void)
  : ior_ ("")
  , do_shutdown_ (0)
{
  //no-op
}

// Reads the Server ior from a file
template <class ServerInterface> int
Client<ServerInterface>::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to open %s for writing (%p)\n"),
                       filename,
                       ACE_TEXT ("open")),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle, true);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to read ior (%p)\n"),
                       ACE_TEXT ("read")),
                      -1);

  this->ior_ = data;
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.
template <class ServerInterface> int
Client<ServerInterface>::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_,
                        ACE_TEXT ("df:nk:x"), 1, 0,
                        ACE_Get_Opt::REQUIRE_ORDER);
  int c = 0;
  int result = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'k':  // ior provide on command line
        this->ior_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;
      case 'n': // Use naming service
        this->naming_ = 1;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Unable to read ior from %s (%p)\n"),
                             get_opts.opt_arg (),
                             ACE_TEXT ("read_ior")),
                            -1);
        break;
      case 'x': // read the flag for shutting down
        this->do_shutdown_ = 1;
        break;
      case 'h':  // display help for use of the client.
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s")
                           ACE_TEXT (" [-d (debug)]")
                           ACE_TEXT (" [-k] <ior>")
                           ACE_TEXT (" [-f] <ior_output_file>")
                           ACE_TEXT (" [-n (use naming service)]")
                           ACE_TEXT (" [-x (shutdown server)]")
                           ACE_TEXT (" [-h (help)]")
                           ACE_TEXT ("\n"),
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

template <class ServerInterface>
Client<ServerInterface>::~Client (void)
{
  this->orb_->destroy ();
}

template <class ServerInterface> int
Client<ServerInterface>::init (const char *name,
                               int argc,
                               ACE_TCHAR **argv)
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

      if (this->ior_.length () != 0)
        {
          CORBA::Object_var server_object =
            this->orb_->string_to_object (this->ior_.c_str ());

          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("invalid ior <%C>\n"),
                               this->ior_.c_str ()),
                              -1);
          this->server_ = ServerInterface::_narrow (server_object.in ());
          if (CORBA::is_nil (this->server_.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Nil Server\n")),
                                -1);
            }
        }
      else if (this->naming_ == 1)
        {
          // No IOR specified. Use the Naming Service
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("Using the Naming Service\n")));
          int retv = this->obtain_initial_references (name);
          if (retv ==-1)
            return -1;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("no ior or naming options  specified\n")),
                          -1);


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client_i::init");
      return -1;
    }

  return 0;
}

template <class ServerInterface> int
Client<ServerInterface>::obtain_initial_references (const char *name)
{
  try
    {
      // Initialize the naming services.
      if (naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("[CLIENT] Process/Thread Id : (%P/%t) Unable to initialize ")
                           ACE_TEXT ("the TAO_Naming_Client.\n")),
                          -1);

      CosNaming::Name server_name (1);
      server_name.length (1);
      server_name[0].id =
        CORBA::string_dup (name);
      CORBA::Object_var obj =
        naming_client_->resolve (server_name);

      this->server_ = ServerInterface::_narrow (obj.in ());
      if (CORBA::is_nil (this->server_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil Server\n")),
                            -1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client::obtain_initial_references");
      return -1;
    }

  return 0;
}

template <class ServerInterface> int
Client<ServerInterface>::do_shutdown (void)
{
  // Returns the shutdwon flag
  return this->do_shutdown_;
}

template <class ServerInterface> void
Client<ServerInterface>::do_shutdown (int flag)
{
  // Fills the flag
  this->do_shutdown_ = flag;
}

#endif /* SIMPLE_UTIL_C */

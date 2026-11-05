#ifndef SIMPLE_UTIL_C
#define SIMPLE_UTIL_C
# include "Simple_util.h"
# include "tao/debug.h"
# include "ace/OS_NS_fcntl.h"
# include "ace/OS_NS_unistd.h"
# include "ace/OS_NS_stdio.h"

namespace Persistent_Grid_Detail
{
  class Handle_Guard
  {
  public:
    explicit Handle_Guard (ACE_HANDLE handle)
      : handle_ (handle)
    {
    }

    ~Handle_Guard ()
    {
      if (this->handle_ != ACE_INVALID_HANDLE)
        ACE_OS::close (this->handle_);
    }

    Handle_Guard (Handle_Guard const&) = delete;
    Handle_Guard (Handle_Guard&&) = delete;
    Handle_Guard& operator= (Handle_Guard const&) = delete;
    Handle_Guard& operator= (Handle_Guard&&) = delete;

  private:
    ACE_HANDLE handle_;
  };

  struct Read_Buffer_Deleter
  {
    ACE_Allocator *allocator;

    void operator() (char *buffer) const
    {
      this->allocator->free (buffer);
    }
  };
}

// Parse the command-line arguments and set options.

template <class Servant> int
Server<Servant>::parse_args ()
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT("do:m:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_.reset (ACE_OS::fopen (get_opts.opt_arg (), "w"));
        if (!this->ior_output_file_)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;

      case 'm':
        this->mem_pool_name_ = get_opts.opt_arg ();
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Initialize the server.
template <class Servant> int
Server<Servant>::init (char const *servant_name,
                       int argc,
                       ACE_TCHAR *argv[])
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

  int const retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->servant_.orb (orb.in ());

  // Stash the memory pool name for reference
  this->servant_.pool_name (mem_pool_name_);

  // Activate the servant in its own child POA.

  // Make sure that you check for failures here via the ACE_TRY
  // macros?!
  try
    {
      CORBA::String_var const str  =
        this->orb_manager_.activate_under_child_poa (servant_name,
                                                     &this->servant_);

      ACE_DEBUG ((LM_DEBUG,
                  "The IOR is: <%C>\n",
                  str.in ()));

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_.get (),
                           "%s",
                           str.in ());
          this->ior_output_file_.reset ();
        }

    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("\tException in activation of POA");
      return -1;
    }

  return 0;
}

template <class Servant> int
Server<Servant>::run ()
{
    // Run the main event loop for the ORB.
  int const ret = this->orb_manager_.run ();

  if (ret == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Server_i::run"),
                      -1);

  return 0;
}

/////////////////////////////////////////////////////////////////
//      Client code Starts here
////////////////////////////////////////////////////////////////

// Reads the Server ior from a file

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::read_ior (ACE_TCHAR const *filename)
{
  // Open the file for reading.
  ACE_HANDLE const f_handle = ACE_OS::open (filename, O_RDONLY);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading: %p\n",
                       filename),
                      -1);

  Persistent_Grid_Detail::Handle_Guard const f_handle_guard (f_handle);
  ACE_Read_Buffer ior_buffer (f_handle);
  std::unique_ptr<char, Persistent_Grid_Detail::Read_Buffer_Deleter> const data (
    ior_buffer.read (), Persistent_Grid_Detail::Read_Buffer_Deleter {ior_buffer.alloc ()});

  if (!data)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);

  this->ior_ = data.get ();

  return 0;
}

// Parses the command line arguments and returns an error status.

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("df:k:x"));
  int c {};
  int result {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'k':  // ior provide on command line
        {
          char const * const ior = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
          if (!ior)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to convert IOR argument to a narrow string\n"),
                              -1);
          this->ior_ = ior;
        }
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;
      case 'x': // read the flag for shutting down
        this->shutdown_ = true;
        break;
      }

  // Indicates successful parsing of command line.
  return 0;
}

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::init (char const * /*name*/,
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

      if (!this->ior_.empty ())
        {
          CORBA::Object_var const server_object =
            this->orb_->string_to_object (this->ior_.c_str ());

          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%C>\n",
                               this->ior_.c_str ()),
                              -1);
          this->server_ = InterfaceObj::_narrow (server_object.in ());
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "no ior or naming options  specified\n"),
                          -1);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Client_i::init");
      return -1;
    }

  return 0;
}

template <class InterfaceObj, class Var> int
Client<InterfaceObj, Var>::shutdown () const
{
  // Returns the shutdown flag.
  return shutdown_;
}

template <class InterfaceObj, class Var> void
Client<InterfaceObj, Var>::shutdown (int flag)
{
  // Sets the shutdown flag.
  shutdown_ = flag != 0;
}

#endif

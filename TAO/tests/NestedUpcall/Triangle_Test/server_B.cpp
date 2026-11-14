//=============================================================================
/**
 *  @file    server_B.cpp
 *
 *  This class implements a simple server for the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================
#include "server_B.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"

int
Object_B_Server::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT ("do:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;
      case 'o': // output the IOR to a file.
        this->ior_output_file_.reset (ACE_OS::fopen (get_opts.opt_arg (), "w"));
        if (!this->ior_output_file_)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Object_B_Server::init (int argc,
                       ACE_TCHAR** argv)
{
  // Call the init of TAO_ORB_Manager to create a child POA
  // under the root POA.
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa");

  this->argc_ = argc;
  this->argv_ = argv;

  if (this->parse_args () != 0)
    return -1;

  CORBA::String_var const str =
    this->orb_manager_.activate_under_child_poa ("object_B",
                                                 &this->object_B_i_);

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_.get (),
                       "%s",
                       str.in ());
      this->ior_output_file_.reset ();
    }

  return 0;
}

int
Object_B_Server::run ()
{
  int const result = this->orb_manager_.run ();

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Object_B_Server::run"),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Object_B_Server object_B_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls.Triangle_Test: Object B Server \n\n"));

  try
    {
      int const retval = object_B_Server.init (argc, argv);

      if (retval == -1)
        return 1;
      return object_B_Server.run () == 0 ? 0 : 1;
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}

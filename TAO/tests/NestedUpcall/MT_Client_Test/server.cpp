
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  This class implements a simple server for the
 *  Nested Upcalls - MT_Client test.
 *
 *  @author Michael Kircher
 */
//=============================================================================

#include "server.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Event_Handler.h"
#include "ace/Sig_Handler.h"

class TestShutdown : public ACE_Event_Handler
{
public:
  explicit TestShutdown (CORBA::ORB_ptr orb)
    : orb_(CORBA::ORB::_duplicate (orb))
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.register_handler (SIGTERM, this);
    this->shutdown_.register_handler (SIGINT, this);
#elif defined(ACE_WIN32)
    this->shutdown_.register_handler (SIGINT, this);
#endif
  }

  ~TestShutdown () override
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.remove_handler (SIGTERM);
    this->shutdown_.remove_handler (SIGINT);
#elif defined(ACE_WIN32)
    this->shutdown_.remove_handler (SIGINT);
#endif
  }

  int handle_signal (int, siginfo_t*, ucontext_t*) override
  {
    this->orb_->shutdown ();
    return 0;
  }

private:
  CORBA::ORB_var orb_;

  ACE_Sig_Handler shutdown_;

  TestShutdown (TestShutdown const &) = delete;
  TestShutdown (TestShutdown &&) = delete;
  TestShutdown &operator= (TestShutdown const &) = delete;
  TestShutdown &operator= (TestShutdown &&) = delete;
};

int
MT_Object_Server::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("do:m"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o': // output the IOR to a file.
        this->ior_output_file_.reset (ACE_OS::fopen (get_opts.opt_arg (), "w"));
        if (!this->ior_output_file_)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing\n",
                             get_opts.opt_arg ()), -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           this->argv_[0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
MT_Object_Server::init (int argc, ACE_TCHAR** argv)
{
  // Call the init of TAO_ORB_Manager to create a child POA
  // under the root POA.
  if (this->orb_manager_.init_child_poa (argc, argv, "child_poa") == -1)
    return -1;

  this->argc_ = argc;
  this->argv_ = argv;

  if (this->parse_args () == -1)
    return -1;

  CORBA::String_var str;
  str = this->orb_manager_.activate_under_child_poa ("MT_Object",
                                                     &this->mT_Object_i_);

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_.get (), "%s", str.in ());
      this->ior_output_file_.reset ();
    }
  return 0;
}

int
MT_Object_Server::run ()
{
  CORBA::ORB_var orb = this->orb_manager_.orb ();
  TestShutdown killer (orb.in ());

  int result = this->orb_manager_.run ();

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "NestedUpCalls_Server::run"),
                      -1);
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls.Triangle_Test: Object A Server \n\n"));

  try
    {
      MT_Object_Server mt_object_server {};

      int const init_result = mt_object_server.init (argc, argv);

      if (init_result == -1)
        return init_result;
      return mt_object_server.run ();
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

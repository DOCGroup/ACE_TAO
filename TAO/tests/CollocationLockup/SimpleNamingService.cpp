/**
 * SimpleNamingService.cpp
 * This is part of the regression test against bug #2130.
 * See CollocationLockup.cpp for a description of the test.
 */

#include "SimpleNamingServiceS.h"

#include "ace/String_Base.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

#include "ace/Event_Handler.h"
#include "ace/Sig_Handler.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

class TestShutdown : public ACE_Event_Handler
{
public:
  explicit TestShutdown (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
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

  TestShutdown (TestShutdown const &) = delete;
  TestShutdown (TestShutdown &&) = delete;
  TestShutdown &operator= (TestShutdown const &) = delete;
  TestShutdown &operator= (TestShutdown &&) = delete;

  int handle_signal (int, siginfo_t *, ucontext_t *) override
  {
    this->orb_->shutdown ();
    return 0;
  }

private:
  CORBA::ORB_var orb_;

  ACE_Sig_Handler shutdown_;
};

class SimpleNamingService_i : public virtual POA_SimpleNamingService
{
public:
  SimpleNamingService_i () = default;

  SimpleNamingService_i (SimpleNamingService_i const &) = delete;
  SimpleNamingService_i (SimpleNamingService_i &&) = delete;
  SimpleNamingService_i &operator= (SimpleNamingService_i const &) = delete;
  SimpleNamingService_i &operator= (SimpleNamingService_i &&) = delete;

  //FUZZ: disable check_for_lack_ACE_OS
  void bind (CORBA::Object_ptr obj) override
  {
    obj_ = CORBA::Object::_duplicate (obj);
  }
  //FUZZ: enable check_for_lack_ACE_OS


  CORBA::Object_ptr resolve () override
  {
    return CORBA::Object::_duplicate (obj_.in ());
  }

private:
  CORBA::Object_var obj_;
};

static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::basic_string<ACE_TCHAR> ior_file {ACE_TEXT ("SimpleNamingService.ior")};
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      TestShutdown killer (orb.in ());

      if (parse_args (argc, argv, ior_file) != 0)
        return 1;

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};
      PortableServer::POA_var const poa {PortableServer::POA::_narrow (poa_object.in ())};
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      PortableServer::ServantBase_var const servant {new SimpleNamingService_i};
      PortableServer::ObjectId_var const oid {poa->activate_object (servant.in ())};
      CORBA::Object_var const obj {poa->id_to_reference (oid.in ())};
      CORBA::String_var const str {orb->object_to_string (obj.in ())};

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             ior_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", str.in ());
      }

      orb->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_ERROR, "Corba Exception: %s\n", ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

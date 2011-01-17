// $Id$

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

class TestShutdown : public ACE_Event_Handler
{
public:
  TestShutdown (CORBA::ORB_ptr orb)
    : orb_(CORBA::ORB::_duplicate (orb))
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.register_handler (SIGTERM, this);
    this->shutdown_.register_handler (SIGINT, this);
#elif defined(ACE_WIN32)
    this->shutdown_.register_handler (SIGINT, this);
#endif
  }

  ~TestShutdown (void)
  {
#if !defined(ACE_LACKS_UNIX_SIGNALS)
    this->shutdown_.remove_handler (SIGTERM);
    this->shutdown_.remove_handler (SIGINT);
#elif defined(ACE_WIN32)
    this->shutdown_.remove_handler (SIGINT);
#endif
  }

  virtual int handle_signal (int, siginfo_t*, ucontext_t*)
  {
    this->orb_->shutdown ();
    return 0;
  }

private:
  CORBA::ORB_var orb_;

  ACE_Sig_Handler shutdown_;
};

namespace
{
  const ACE_TCHAR* iorFileName = ACE_TEXT("SimpleNamingService.ior");
}

class SimpleNamingService_i : public virtual POA_SimpleNamingService
{
public:

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void bind (CORBA::Object_ptr obj)
  {
    obj_ = CORBA::Object::_duplicate (obj);
  }
  //FUZZ: enable check_for_lack_ACE_OS


  virtual CORBA::Object_ptr resolve ()
  {
    return CORBA::Object::_duplicate (obj_.in ());
  }

private:

  CORBA::Object_var obj_;

};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        iorFileName = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      TestShutdown killer (orb.in ());

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var pPoaObj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (pPoaObj.in ());
      PortableServer::POAManager_var pMgr = poa->the_POAManager ();
      pMgr->activate ();

      SimpleNamingService_i* servant = new SimpleNamingService_i;
      PortableServer::ObjectId_var oid = poa->activate_object (servant);
      CORBA::Object_var obj = poa->id_to_reference (oid.in ());
      CORBA::String_var str = orb->object_to_string (obj.in ());

      FILE *output_file= ACE_OS::fopen (iorFileName, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           iorFileName),
                           1);
      ACE_OS::fprintf (output_file, "%s", str.in ());
      ACE_OS::fclose (output_file);

      orb->run ();
    }
  catch (CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_ERROR, "Corba Exception: %s\n", ex._info ().c_str ()));
      return 1;
    }

  return 0;
}

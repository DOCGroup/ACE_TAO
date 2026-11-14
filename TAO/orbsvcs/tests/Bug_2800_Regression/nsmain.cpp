#include "NamingTask.h"
#include "Hello.h"
#include "NsShutdown.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <cstdio>
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("ns.ior") };
  ACE_TCHAR const *ior_shutdown_file { ACE_TEXT ("shutdown.ior") };
  bool shutdown_ns {};
};

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

class TestTask : public ACE_Task_Base
{
public:
  TestTask (int argc, ACE_TCHAR **argv);
  TestTask (TestTask const &) = delete;
  TestTask (TestTask &&) = delete;
  TestTask &operator= (TestTask const &) = delete;
  TestTask &operator= (TestTask &&) = delete;

  int svc () override;

  int parse_args (int argc, ACE_TCHAR **argv);

private:
  NamingTask namingServiceA_;
  NamingTask namingServiceB_;
  CORBA::ORB_var orb_ {};
  Server_Options options_;
};

TestTask::TestTask (int argc, ACE_TCHAR **argv)
 : namingServiceA_ (ACE_TEXT ("NamingORBA"), argc, argv, 9931),
   namingServiceB_ (ACE_TEXT ("NamingORBB"), argc, argv, 9932)
{
  orb_ = CORBA::ORB_init (argc, argv, ACE_TEXT ("ServerORB"));
  parse_args (argc, argv);
}

int
TestTask::parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options_.ior_output_file = get_opts.opt_arg ();
        break;
      case 's':
        options_.ior_shutdown_file = get_opts.opt_arg ();
        options_.shutdown_ns = true;
        break;
      }
  return 0;
}

int
TestTask::svc ()
{
  try
    {
      namingServiceA_.activate ();
      namingServiceA_.waitInit ();
      namingServiceB_.activate ();
      namingServiceB_.waitInit ();

      {
        File_Ptr const output_file (
          ACE_OS::fopen (options_.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options_.ior_output_file),
                            1);
        if (std::fprintf (output_file.get (), "%s", namingServiceA_.ior ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), 1);
      }

      CORBA::Object_var obj = orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->activate ();

      obj = orb_->string_to_object (namingServiceB_.ior ());
      CosNaming::NamingContext_var root = CosNaming::NamingContext::_narrow (obj.in ());
      if (CORBA::is_nil (root.in ()))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, Nil Naming Context reference\n")));
          return 1;
        }

      CosNaming::NamingContext_var example_nc;
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("example");
      try
        {
          obj = root->resolve (name);
          example_nc = CosNaming::NamingContext::_narrow (obj.in ());
        }
      catch (CosNaming::NamingContext::NotFound const &)
        {
          example_nc = root->bind_new_context (name);
        }

      name.length (2);
      name[1].id = CORBA::string_dup ("Hello");
      // The POA borrows this stack servant, which remains alive through orb_->run().
      Hello servant (orb_.in ());
      PortableServer::ObjectId_var const oid = poa->activate_object (&servant);
      obj = poa->id_to_reference (oid.in ());
      root->rebind (name, obj.in ());
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Hello object bound in Naming Service B\n")));

      name.length (1);
      obj = orb_->string_to_object (namingServiceA_.ior ());
      root = CosNaming::NamingContext::_narrow (obj.in ());
      root->bind_context (name, example_nc.in ());
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("'example' context of NS B bound in Naming Service A\n")));

      if (options_.shutdown_ns)
        {
          namingServiceB_.end ();
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("Naming Service B shut down\n")));
        }

      // This servant also remains alive until orb_->run() returns.
      NsShutdown shutdown_servant (orb_.in ());
      PortableServer::ObjectId_var const shutdown_oid = poa->activate_object (&shutdown_servant);
      obj = poa->id_to_reference (shutdown_oid.in ());
      CORBA::String_var const ior = orb_->object_to_string (obj.in ());
      {
        File_Ptr const output_file (
          ACE_OS::fopen (options_.ior_shutdown_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                             options_.ior_shutdown_file),
                            1);
        if (std::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), 1);
      }

      orb_->run ();
      orb_->destroy ();
      namingServiceA_.end ();
      if (!options_.shutdown_ns)
        namingServiceB_.end ();
      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CORBA exception: ");
    }

  return -1;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TestTask test (argc, argv);
  if (test.activate () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to start test task.\n")), -1);
    }

  test.thr_mgr ()->wait ();

  return 0;
}

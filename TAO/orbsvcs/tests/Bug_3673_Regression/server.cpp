#include "Hello.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT ("test.ior") };
};

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
  Server_Options options_ {};
  CORBA::ORB_var orb_ {};
};

TestTask::TestTask (int argc, ACE_TCHAR **argv)
  : orb_ (CORBA::ORB_init (argc, argv, "ServerORB"))
{
  this->parse_args (argc, argv);
}

int
TestTask::parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        this->options_.ior_output_file = get_opts.opt_arg ();
        break;
      }
  return 0;
}

int
TestTask::svc ()
{
  try
    {
      CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      obj = this->orb_->string_to_object ("corbaloc:iiop:1.2@localhost:9932/NameService");
      CosNaming::NamingContext_var root =
        CosNaming::NamingContext::_narrow (obj.in ());

      if (CORBA::is_nil (root.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Error, Nil Naming Context reference\n")),
                          1);

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

      Hello servant (this->orb_.in ());
      PortableServer::ObjectId_var const oid = poa->activate_object (&servant);
      obj = poa->id_to_reference (oid.in ());
      root->rebind (name, obj.in ());

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Hello object bound in Naming Service B\n")));

      name.length (1);
      obj = this->orb_->string_to_object ("corbaloc:iiop:1.2@localhost:9931/NameService");
      root = CosNaming::NamingContext::_narrow (obj.in ());
      root->bind_context (name, example_nc.in ());

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("'example' context of NS B bound in Naming Service A\n")));

      CORBA::String_var const ior = this->orb_->object_to_string (obj.in ());
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (this->options_.ior_output_file, ACE_TEXT ("w")),
        ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file %s for writing IOR: %C\n"),
                           this->options_.ior_output_file,
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Wrote IOR file\n")));

      this->orb_->run ();
      this->orb_->destroy ();

      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CORBA exception: ");
    }

  return -1;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TestTask test_ (argc, argv);
  if (test_.activate () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to start test task.\n")),
                        -1);
    }

  test_.wait ();

  return 0;
}

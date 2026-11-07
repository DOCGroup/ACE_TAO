#include "Transport_Idle_TimeoutS.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

struct Server_Options
{
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("server.ior")};
};

class Test_i : public virtual POA_Transport_Idle_Timeout_Test::Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i & operator= (Test_i const &) = delete;
  Test_i & operator= (Test_i &&) = delete;

  void ping () override
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) server: ping received\n"));
  }

  void shutdown () override
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) server: shutdown received\n"));
    this->orb_->shutdown (false);
  }

private:
  CORBA::ORB_var const orb_;
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 'o':
          options.ior_output_file = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s [-o <iorfile>]\n"),
                             argv [0]),
                            -1);
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "nil RootPOA\n"), 1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      Test_i *servant {};
      ACE_NEW_RETURN (servant, Test_i (orb.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (servant);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (servant);

      CORBA::Object_var const object =
        root_poa->id_to_reference (id.in ());

      CORBA::String_var const ior =
        orb->object_to_string (object.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file <%s>\n"),
                             options.ior_output_file.c_str ()),
                            1);

        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();
      orb->run ();

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

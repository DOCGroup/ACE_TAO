#include "Transport_Idle_Timeout_Long_RequestS.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

void
sleep_with_reactor (CORBA::ORB_ptr orb, int seconds)
{
  ACE_Time_Value const deadline = ACE_OS::gettimeofday () + ACE_Time_Value (seconds);

  while (ACE_OS::gettimeofday () < deadline)
    {
      ACE_Time_Value tv (0, 50000); // 50 ms slices
      orb->perform_work (tv);
    }
}

class Test_i
  : public virtual POA_Transport_Idle_Timeout_Long_Request_Test::Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  void long_request () override
  {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) server: long_request, sleeping 2 seconds\n"));
    sleep_with_reactor (orb_.in(), 2);
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) server: long_request finished\n"));
  }

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
  CORBA::ORB_var orb_;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          ior_output_file = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s [-o <iorfile>]\n", argv[0]), -1);
        }
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Test_i *servant = 0;
      ACE_NEW_RETURN (servant, Test_i (orb.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (servant);

      PortableServer::ObjectId_var id = root_poa->activate_object (servant);
      CORBA::Object_var object = root_poa->id_to_reference (id.in ());
      CORBA::String_var ior = orb->object_to_string (object.in ());

      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output file <%s>\n",
                           ior_output_file), 1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();
      orb->run ();
      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}

#include "Test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"
#include <memory>
#include <string>

static std::basic_string<ACE_TCHAR> ior_output_file = ACE_TEXT ("test.ior");

class MT_Task : public ACE_Task_Base
{
public:
  MT_Task (PortableServer::POA_ptr p,
           PortableServer::Servant one,
           PortableServer::Servant two,
           PortableServer::Servant three)
    : p_ (PortableServer::POA::_duplicate (p))
    , one_ (one)
    , two_ (two)
    , three_ (three)
  {
  }

  MT_Task (MT_Task const &) = delete;
  MT_Task (MT_Task &&) = delete;
  MT_Task & operator= (MT_Task const &) = delete;
  MT_Task & operator= (MT_Task &&) = delete;

  int svc () override;

private:
  PortableServer::POA_var const p_;
  // The POA retains ownership of these servants while the worker uses them.
  PortableServer::Servant const one_;
  PortableServer::Servant const two_;
  PortableServer::Servant const three_;
};

int
MT_Task::svc ()
{
  constexpr CORBA::Long iterations = 2000;
  try
    {
      for (CORBA::Long i {}; i < iterations; ++i)
        {
          CORBA::Object_var const one_ref =
            this->p_->servant_to_reference (this->one_);

          CORBA::Object_var const two_ref =
            this->p_->servant_to_reference (this->two_);

          CORBA::Object_var const three_ref =
            this->p_->servant_to_reference (this->three_);
        }
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("(%P|%t) Caugh exception\n");
    }

  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
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
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      One_Impl *one_impl {};
      ACE_NEW_RETURN (one_impl, One_Impl (), 1);
      Two_Impl *two_impl {};
      ACE_NEW_RETURN (two_impl, Two_Impl (), 1);

      Three_Impl *three_impl {};
      ACE_NEW_RETURN (three_impl, Three_Impl (), 1);

      // ServantBase_var takes each allocated servant pointer and balances its reference count.
      PortableServer::ServantBase_var owner_transfer1 (one_impl);
      PortableServer::ServantBase_var owner_transfer2 (two_impl);
      PortableServer::ServantBase_var owner_transfer3 (three_impl);

      CORBA::PolicyList const policies;  // Empty policy list.

      PortableServer::POA_var const first_poa =
        root_poa->create_POA ("first POA",
                              poa_manager.in (),
                              policies);

      PortableServer::ObjectId_var const oid1 =
        first_poa->activate_object (one_impl);

      PortableServer::ObjectId_var const oid2 =
        first_poa->activate_object (two_impl);

      PortableServer::ObjectId_var const oid3 =
        first_poa->activate_object (three_impl);

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", "started");
      }

      MT_Task task (first_poa.in (),
                    one_impl,
                    two_impl,
                    three_impl);

      if (task.activate (THR_NEW_LWP | THR_JOINABLE,
                         32) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate threads\n"),
                          1);

      task.thr_mgr ()->wait ();

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - test finished\n"));

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

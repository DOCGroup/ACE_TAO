//$Id$

#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const char* filename = "test.ior";

class test_impl : public POA_test
{
public:

  test_impl (CORBA::ORB_ptr orb,
             RTScheduling::Current_ptr current)
    : orb_ (orb),
    current_ (RTScheduling::Current::_duplicate (current))
  {
  }

  virtual void one_way (const char * message)
  {
    ACE_DEBUG ((LM_DEBUG,
                "One-Way Message = %s\n",
                message));
  }

  virtual char * two_way (const char * message)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Two-Way Message = %s\n",
                message));

    RTScheduling::DistributableThread_var DT =
      this->current_->lookup (*(this->current_->id ()));

    DT->cancel ();

    return CORBA::string_dup (message);
  }

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void shutdown (void)
  {
    orb_->shutdown ();
  }
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_ptr orb_;
  RTScheduling::Current_var current_;
};

int
parse_args (int argc,
            ACE_TCHAR* argv [])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, "f:");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          filename = opts.opt_arg ();
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown Option\n"));
          return -1;
    }
    }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "");

      parse_args (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CORBA::Object_var current_obj = orb->resolve_initial_references ("RTScheduler_Current");

      RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj.in ());

      test_impl* test_i;
      ACE_NEW_RETURN (test_i,
                      test_impl (orb.in (),
                      current.in ()),
                      -1);

      PortableServer::ObjectId_var id;

      id = root_poa->activate_object (test_i);

      CORBA::Object_var server =
        root_poa->id_to_reference (id.in ());

      CORBA::String_var ior;
      if (!CORBA::is_nil (server.in ()))
        {
          ior = orb->object_to_string (server.in ());
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to activate test object\n"),
                             -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "IOR = %s\n",
                  ior.in ()));

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager");

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

      TAO_Scheduler scheduler (orb.in ());
      manager->rtscheduler (&scheduler);

      // Print ior to the file.
      if (filename != 0)
        {
          FILE* output_file = ACE_OS::fopen (filename, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               filename),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}

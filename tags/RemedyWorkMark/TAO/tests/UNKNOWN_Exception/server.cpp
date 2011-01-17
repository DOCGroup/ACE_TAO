// $Id$

#include "ace/Get_Opt.h"
#include "testS.h"
#include "tao/PortableServer/Root_POA.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("ior");
static int done = 0;

void
throw_exception (void)
{
  throw 1;
}

class test_i :
  public POA_test
{
public:

  test_i (CORBA::ORB_ptr orb);

  void normal_method (void);

  void unknown_exception_in_method (void);

  void unknown_exception_during_deactivation (void);

  void _add_ref (void);
  void _remove_ref (void);

  CORBA::ORB_var orb_;

  int reference_count_;
};

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    reference_count_ (1)
{
}

void
test_i::normal_method (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::normal_method() called\n"));
}

void
test_i::unknown_exception_in_method (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::unknown_exception_in_method() called\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Unknown exception being generated: should be propagated to the client\n"));

  throw_exception ();
}

void
test_i::unknown_exception_during_deactivation (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::unknown_exception_during_deactivation() called\n"));

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

void
test_i::_add_ref (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::_add_ref() called; current refcount = %d\n",
              this->reference_count_++));
}

void
test_i::_remove_ref (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i::_remove_ref() called; current refcount = %d\n",
              this->reference_count_--));

  if (this->reference_count_ == 0)
    {
      delete this;

      ACE_DEBUG ((LM_DEBUG,
                  "Unknown exception being generated: should be gobbled up by the POA\n"));

      throw_exception ();
    }
}

//FUZZ: disable check_for_lack_ACE_OS
class test_factory_i :
  public POA_test_factory
{
public:

  test_factory_i (CORBA::ORB_ptr orb);

  test_ptr create_test (void);

  void shutdown (void);

  CORBA::ORB_var orb_;
};
//FUZZ: enable check_for_lack_ACE_OS

test_factory_i::test_factory_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

test_ptr
test_factory_i::create_test (void)
{
  test_i *servant =
    new test_i (this->orb_.in ());

  PortableServer::ServantBase_var safe_servant (servant);
  ACE_UNUSED_ARG (safe_servant);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id_act =
    root_poa->activate_object (servant);

  CORBA::Object_var object = root_poa->id_to_reference (id_act.in ());

  test_var test =
    test::_narrow (object.in ());

  return test._retn ();
}

void
test_factory_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "factory_i::shutdown() called\n"));

  done = 1;
  this->orb_->shutdown (0);
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nusage %s:\n"
                           "\t-o <ior output file> [defaults to %s]\n"
                           "\n",
                           argv [0],
                           ior_output_file),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return -1;

      {
        test_factory_i *servant =
          new test_factory_i (orb.in ());

        PortableServer::ServantBase_var safe_servant (servant);
        ACE_UNUSED_ARG (safe_servant);

        PortableServer::ObjectId_var id_act =
          root_poa->activate_object (servant);

        CORBA::Object_var object = root_poa->id_to_reference (id_act.in ());

        test_factory_var test_factory =
          test_factory::_narrow (object.in ());

        CORBA::String_var ior =
          orb->object_to_string (test_factory.in ());

        FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
        if (output_file == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior_output_file),
                            -1);
        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);
      }

      poa_manager->activate ();

      TAO_Root_POA *tao_poa = dynamic_cast <TAO_Root_POA*> (root_poa.in ());

      while (!done)
        {
          CORBA::ULong outstanding_requests =
            tao_poa->outstanding_requests ();

          ACE_DEBUG ((LM_DEBUG,
                      "Number of outstanding requests before ORB::perform_work(): %d\n",
                      outstanding_requests));

          ACE_ASSERT (outstanding_requests == 0);

          orb->perform_work ();

          // On some systems this loop must yield or else the other threads
          // will not get a chance to run.
          ACE_OS::thr_yield ();
        }
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Failure: Unexpected exception caught\n"),
                        -1);
    }

  return 0;
}

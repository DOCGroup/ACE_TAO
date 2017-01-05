#include "testS.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("server.ior");

class Test_impl : public virtual POA_Test
{
public:
  Test_impl (CORBA::ORB_ptr orb);
  virtual AnySeq *RunTest(const AnySeq &params);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS
private:
  CORBA::ORB_var orb_;
};

Test_impl::Test_impl (CORBA::ORB_ptr orb) : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_impl::shutdown(void)
{
  this->orb_->shutdown (0);
}

AnySeq *Test_impl::RunTest(const AnySeq &params)
{
  ACE_DEBUG ((LM_DEBUG, "RunTest: params.length == %d\n", params.length()));
  for (CORBA::ULong count = 0; count < params.length(); ++count)
  {
    const Container* container = 0;
    if (!(params[count] >>= container))
    {
      ACE_ERROR ((LM_ERROR, "ERROR, failed extract\n"));
    }
    else
    {
      const Inner* inner = 0;
      if (!(container->contents >>= inner))
      {
        ACE_ERROR ((LM_ERROR, "ERROR, failed extract\n"));
      }
      else
        ACE_DEBUG ((LM_DEBUG, "%d %d %d %d %d\n", inner->value1, inner->value2, inner->value3, inner->value4, inner->value5));
    }
  }

  AnySeq *result = new AnySeq(params);
  return result;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Get reference to Root POA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var mgr = root_poa->the_POAManager ();

      PortableServer::LifespanPolicy_var lifespan =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] = PortableServer::LifespanPolicy::_duplicate (
                                                         lifespan.in ());
      PortableServer::POA_var persistent_poa =
        root_poa->create_POA ("PersistentPOA", mgr.in (),
                              policy_list);
      lifespan->destroy ();

      Test_impl *test_servant = 0;
      ACE_NEW_RETURN (test_servant,
                      Test_impl (orb.in ()),
                      1);
      PortableServer::ServantBase_var receiver_owner_transfer(test_servant);

      PortableServer::ObjectId_var id =
        persistent_poa->activate_object (test_servant);

      obj =
        persistent_poa->id_to_reference (id.in ());

      CORBA::String_var ior = orb->object_to_string (obj.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Activate POA manager
      mgr->activate ();

      // Accept requests
      orb->run ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception caught: ");
      return 1;
    }
  return 0;
}









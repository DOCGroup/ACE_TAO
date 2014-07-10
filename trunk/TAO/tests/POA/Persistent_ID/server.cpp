
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  In this example of using Persistent IDs.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "testS.h"
#include "ace/OS_NS_stdio.h"
#include "tao/PortableServer/PortableServer.h"

class test_i :
  public virtual POA_test
{
public:
  test_i (CORBA::ORB_ptr orb_ptr,
          PortableServer::POA_ptr poa);

  void method (void);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  void shutdown (void);

  test_ptr create_POA (void);

  void destroy_POA (void);

  PortableServer::POA_ptr _default_POA (void);

protected:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  PortableServer::POA_var child_poa_;
  PortableServer::ObjectId_var oid_;
};

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
test_i::method (void)
{
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("POACurrent");

  PortableServer::Current_var current =
    PortableServer::Current::_narrow (obj.in ());

  PortableServer::POA_var poa =
    current->get_POA ();

  CORBA::String_var poa_name =
    poa->the_name ();

  ACE_DEBUG ((LM_DEBUG,
              "Method invoked on servant in POA = %C\n",
              poa_name.in ()));
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

test_ptr
test_i::create_POA (void)
{
  CORBA::PolicyList policies (2);
  policies.length (2);

  policies[0] =
    this->poa_->create_id_assignment_policy
    (this->oid_.ptr () == 0 ? PortableServer::SYSTEM_ID :
     PortableServer::USER_ID);

  policies[1] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT);

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  ACE_CString name = "childPOA";
  this->child_poa_ =
    this->poa_->create_POA (name.c_str (),
                            poa_manager.in (),
                            policies);

  // Destroy the policies
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy ();
    }

  test_i *servant =
    new test_i (this->orb_.in (),
                this->child_poa_.in ());

  PortableServer::ServantBase_var safe_servant (servant);

  if (this->oid_.ptr () == 0)
    {
      this->oid_ =
        this->child_poa_->activate_object (servant);
    }
  else
    {
      this->child_poa_->activate_object_with_id (this->oid_.in (),
                                                 servant);
    }

  CORBA::Object_var object = this->child_poa_->id_to_reference (this->oid_.in ());

  test_var test = test::_narrow (object.in ());

  return test._retn ();
}

void
test_i::destroy_POA (void)
{
  this->child_poa_->destroy (1, 0);
}

const ACE_TCHAR *ior_file = ACE_TEXT ("ior");

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-f ior_file"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

static int
write_ior_to_file (const char *ior)
{
  FILE *output_file =
    ACE_OS::fopen (ior_file, "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_file),
                      -1);

  u_int result = 0;

  result = ACE_OS::fprintf (output_file,
                            "%s",
                            ior);
  if (result != ACE_OS::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       ior,
                       ior_file),
                      -1);

  ACE_OS::fclose (output_file);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      test_i servant (orb.in (),
                      root_poa.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      test_var test =
        test::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      int write_result =
        write_ior_to_file (ior.in ());
      if (write_result != 0)
        return write_result;

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}

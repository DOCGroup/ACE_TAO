
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  In this example of using Persistent IDs.
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "testS.h"
#include "ace/OS_NS_stdio.h"
#include "tao/PortableServer/PortableServer.h"
#include <cstring>
#include <memory>

class test_i :
  public virtual POA_test
{
public:
  test_i (CORBA::ORB_ptr orb_ptr,
          PortableServer::POA_ptr poa);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  void method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  void shutdown () override;

  test_ptr create_POA () override;

  void destroy_POA () override;

  PortableServer::POA_ptr _default_POA () override;

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
test_i::method ()
{
  CORBA::Object_var const obj =
    this->orb_->resolve_initial_references ("POACurrent");

  PortableServer::Current_var const current =
    PortableServer::Current::_narrow (obj.in ());

  PortableServer::POA_var const poa =
    current->get_POA ();

  CORBA::String_var const poa_name =
    poa->the_name ();

  ACE_DEBUG ((LM_DEBUG,
              "Method invoked on servant in POA = %C\n",
              poa_name.in ()));
}

void
test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

test_ptr
test_i::create_POA ()
{
  CORBA::PolicyList policies (2);
  policies.length (2);

  policies[0] =
    this->poa_->create_id_assignment_policy (
      this->oid_.ptr () == nullptr ? PortableServer::SYSTEM_ID : PortableServer::USER_ID);

  policies[1] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT);

  PortableServer::POAManager_var const poa_manager =
    this->poa_->the_POAManager ();

  ACE_CString const name = "childPOA";
  this->child_poa_ =
    this->poa_->create_POA (name.c_str (),
                            poa_manager.in (),
                            policies);

  // Destroy the policies
  for (CORBA::ULong i {};
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy ();
    }

  PortableServer::ServantBase_var safe_servant (
    new test_i (this->orb_.in (),
                this->child_poa_.in ()));

  if (this->oid_.ptr () == nullptr)
    {
      this->oid_ =
        this->child_poa_->activate_object (safe_servant.in ());
    }
  else
    {
      this->child_poa_->activate_object_with_id (this->oid_.in (),
                                                 safe_servant.in ());
    }

  CORBA::Object_var const object = this->child_poa_->id_to_reference (this->oid_.in ());

  test_var test = test::_narrow (object.in ());

  return test._retn ();
}

void
test_i::destroy_POA ()
{
  this->child_poa_->destroy (true, false);
}

static int
parse_args (int argc, ACE_TCHAR **argv, ACE_TCHAR const *&ior_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c {};

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
write_ior_to_file (ACE_TCHAR const *ior_file,
                   char const *ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (ior_file, "w"), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output files for writing IOR: %s\n",
                       ior_file),
                      -1);

  int const result = ACE_OS::fprintf (output_file.get (), "%s", ior);
  if (result < 0 || static_cast<size_t> (result) != std::strlen (ior))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_OS::fprintf failed while writing %C to %s\n",
                       ior,
                       ior_file),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior_file = ACE_TEXT ("ior");
      int const result = parse_args (argc, argv, ior_file);
      if (result != 0)
        return result;

      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      test_i servant (orb.in (),
                      root_poa.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      test_var const test =
        test::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (test.in ());

      int const write_result =
        write_ior_to_file (ior_file, ior.in ());
      if (write_result != 0)
        return write_result;

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}

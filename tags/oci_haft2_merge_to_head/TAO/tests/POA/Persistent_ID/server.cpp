// $Id$

// ================================================================
//
// = LIBRARY
//    TAO/tests/POA/Persistent_ID
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    In this example of using Persistent IDs.
//
// = AUTHOR
//    Irfan Pyarali
//
// ================================================================

#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "testS.h"

ACE_RCSID(Explicit_Activation, server, "$Id$")

class test_i : public virtual PortableServer::RefCountServantBase,
               public virtual POA_test
{
public:
  test_i (CORBA::ORB_ptr orb_ptr,
          PortableServer::POA_ptr poa);

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  test_ptr create_POA (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy_POA (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

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
test_i::method (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("POACurrent"
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::Current_var current =
    PortableServer::Current::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var poa =
    current->get_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var poa_name =
    poa->the_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Method invoked on servant in POA = %s\n",
              poa_name.in ()));
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

test_ptr
test_i::create_POA (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::PolicyList policies (2);
  policies.length (2);

  policies[0] =
    this->poa_->create_id_assignment_policy (PortableServer::SYSTEM_ID
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (test::_nil ());

  policies[1] =
    this->poa_->create_lifespan_policy (PortableServer::PERSISTENT
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (test::_nil ());

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (test::_nil ());

  ACE_CString name = "POA";
  this->child_poa_ =
    this->poa_->create_POA (name.c_str (),
                            poa_manager.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (test::_nil ());

  test_i *servant =
    new test_i (this->orb_.in (),
                this->child_poa_.in ());

  PortableServer::ServantBase_var safe_servant (servant);

  if (this->oid_.ptr () == 0)
    {
      this->oid_ =
        this->child_poa_->activate_object (servant
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (test::_nil ());
    }
  else
    {
      this->child_poa_->activate_object_with_id (this->oid_.in (),
                                                 servant
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (test::_nil ());
    }

  test_var test =
    servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (test::_nil ());

  return test._retn ();
}

void
test_i::destroy_POA (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->child_poa_->destroy (1, 0
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

const char *ior_file = "ior";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:");
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
                       "ACE_OS::fprintf failed while writing %s to %s\n",
                       ior,
                       ior_file),
                      -1);

  ACE_OS::fclose (output_file);

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i servant (orb.in (),
                      root_poa.in ());

      test_var test =
        servant._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (test.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int write_result =
        write_ior_to_file (ior.in ());
      if (write_result != 0)
        return write_result;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

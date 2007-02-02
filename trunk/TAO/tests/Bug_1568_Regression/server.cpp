/**
 * @file server.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "TestS.h"

#include "tao/Utils/Servant_Var.h"
#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

ACE_RCSID(Bug_1568_Regression, server, "$Id$")

/**
 * @class Simple_C
 *
 * @brief A simple implementation of the 'C' object.
 *
 */
class Simple_C
  : public virtual POA_Baz::AMH_C
{
public:
  Simple_C (CORBA::ORB_ptr orb);

  void op1 (
      Foo::Bar::AMH_AResponseHandler_ptr _tao_rh)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void op2 (
      Foo::Bar::AMH_AResponseHandler_ptr _tao_rh)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void op3 (
      Foo::Bar::AMH_BResponseHandler_ptr _tao_rh)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void op4 (
      Baz::AMH_CResponseHandler_ptr _tao_rh)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  CORBA::ORB_ptr orb_;
};


/***************************/
/*** Servant Definition ***/

Simple_C::Simple_C (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Simple_C::op1(
    Foo::Bar::AMH_AResponseHandler_ptr _tao_rh)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->op1();
}

void
Simple_C::op2(
    Foo::Bar::AMH_AResponseHandler_ptr _tao_rh)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->op2();
}

void
Simple_C::op3(
    Foo::Bar::AMH_BResponseHandler_ptr _tao_rh)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->op3();
}

void
Simple_C::op4(
    Baz::AMH_CResponseHandler_ptr _tao_rh)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->op4();
}

// ****************************************************************

const char *ior_output_file = "test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
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
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      TAO::Utils::Servant_Var<Simple_C> simple_c_impl(
          new Simple_C(orb.in()));

      Baz::C_var simple_c =
        simple_c_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (simple_c.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

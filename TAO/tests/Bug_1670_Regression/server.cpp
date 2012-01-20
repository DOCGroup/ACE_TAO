/**
 * @file server.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "TestS.h"

#include "tao/corba.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"

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

  void op1 (Foo::Bar::AMH_AResponseHandler_ptr _tao_rh);
  void op2 (Foo::Bar::AMH_AResponseHandler_ptr _tao_rh);
  void op3 (Foo::Bar::AMH_BResponseHandler_ptr _tao_rh);
  void op4 (Baz::AMH_CResponseHandler_ptr _tao_rh);
  void shutdown (Baz::AMH_CResponseHandler_ptr);

protected:
  CORBA::ORB_var orb_;
};


/***************************/
/*** Servant Definition ***/

Simple_C::Simple_C (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Simple_C::op1(Foo::Bar::AMH_AResponseHandler_ptr _tao_rh)
{
  _tao_rh->op1(1);
}

void
Simple_C::op2(Foo::Bar::AMH_AResponseHandler_ptr _tao_rh)
{
  _tao_rh->op2(2);
}

void
Simple_C::op3(Foo::Bar::AMH_BResponseHandler_ptr _tao_rh)
{
  _tao_rh->op3(3);
}

void
Simple_C::op4(Baz::AMH_CResponseHandler_ptr _tao_rh)
{
  _tao_rh->op4(4);
}

void
Simple_C::shutdown (Baz::AMH_CResponseHandler_ptr)
{
  this->orb_->shutdown ();
}

// ****************************************************************

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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

      PortableServer::Servant_var<Simple_C> simple_c_impl(
          new Simple_C(orb.in()));

      PortableServer::ObjectId_var id =
        root_poa->activate_object (simple_c_impl.in ());

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Baz::C_var simple_c =
        Baz::C::_narrow (object.in ());

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

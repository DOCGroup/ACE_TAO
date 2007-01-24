// $Id$

#include "fooS.h"
#include "ace/OS_NS_stdio.h"

const char* ior_output_file = "server.ior";

class foo_i: public POA_foo
{
public:
  foo_i (CORBA::ORB_ptr orb)
   : orb_ (CORBA::ORB::_duplicate (orb)) {
  }

  void shutdown ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy ()
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

void
foo_i::shutdown ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown ();
}

void
foo_i::destroy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  bool expected_exception_raised = false;

  ACE_TRY
    {
      // This should case an BAD_INV_ORDER exception
      this->orb_->destroy ();
    }
  ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
    {
      if ((ex.minor() & 0xFFFU) == 3)
        {
          expected_exception_raised = true;
        }
    }
  ACE_ENDTRY;

  if (!expected_exception_raised)
    ACE_ERROR ((LM_ERROR, "ERROR: Caught incorrect exception\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Caught correct exception\n"));
}


int
main (int argc, char** argv)
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

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

      foo_i* server_impl = 0;
      ACE_NEW_RETURN (server_impl,
                      foo_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(server_impl);

      foo_var server = server_impl->_this ();

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

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

      root_poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CORBA::Exception");
    }

  return 0;
}

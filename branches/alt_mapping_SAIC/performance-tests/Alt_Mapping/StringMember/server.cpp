// $Id$

// ============================================================================
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION
//    The server process for the StringMember test
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#include "ace/OS_NS_stdio.h"

#include "StringMemberS.h"

const char *ior_file = "StringMember.ior";

class Test_i : public virtual POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb)
    : orb_ (orb)
  {
  }

  virtual void
  test_op (const Foo & /* inarg */)
  {
  }

  virtual void
  shutdown (void)
  {
    this->orb_->shutdown (false);
  }

private:
  CORBA::ORB_ptr orb_;
};

int
ACE_TMAIN (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      Test_i servant (orb.in ());
      Test_var objref =  servant._this ();

      CORBA::String_var ior = orb->object_to_string (objref.in ());

      FILE *output_file= ACE_OS::fopen (ior_file, "w");

      if (output_file == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot open output file for writing IOR: %s",
                      ior_file));
          return -1;
        }

      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      orb->run ();

      root_poa->destroy (false, false);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server: CORBA Exception caught");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "server: unknown exception caught\n"),
                        -1);
    }

  return 0;
}

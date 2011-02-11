// $Id$

#include "fooS.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

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

class foo_i: public POA_foo
{
public:
  foo_i (CORBA::ORB_ptr orb)
   : orb_ (CORBA::ORB::_duplicate (orb)) {
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown ();
  //FUZZ: enable check_for_lack_ACE_OS

  void destroy ();

private:
  CORBA::ORB_var orb_;
};

void
foo_i::shutdown ()
{
  this->orb_->shutdown ();
}

void
foo_i::destroy ()
{
  bool expected_exception_raised = false;

  try
    {
      // This should case an BAD_INV_ORDER exception
      this->orb_->destroy ();
    }
  catch (const CORBA::BAD_INV_ORDER& ex)
    {
      if ((ex.minor() & 0xFFFU) == 3)
        {
          expected_exception_raised = true;
        }
    }

  if (!expected_exception_raised)
    ACE_ERROR ((LM_ERROR, "ERROR: Caught incorrect exception\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "Caught correct exception\n"));
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

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

      PortableServer::ObjectId_var id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      foo_var server = foo::_narrow (object.in ());

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

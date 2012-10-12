//$Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"

class Test_i: public virtual POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb);

  void hello (CORBA::Long howmany);

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown  (void);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var orb_;
};

Test_i::Test_i (CORBA::ORB_ptr orb)
 : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::hello (CORBA::Long howmany)
{
  ACE_DEBUG ((LM_DEBUG, "hello called with : %i\n", howmany));
}

void
Test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

static const ACE_TCHAR *ior_output_file = 0;

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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    // Obtain RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in());

    // Get the POAManager of the RootPOA
    PortableServer::POAManager_var poa_mgr =
      root_poa->the_POAManager ();

    poa_mgr->activate ();

    // Create a servant
    Test_i servant (orb.in ());

    PortableServer::ObjectId_var oid =
      root_poa->activate_object (&servant);

    obj = root_poa->id_to_reference (oid.in());

    CORBA::String_var ior =
      orb->object_to_string (obj.in());

    // If the ior_output_file exists, output the ior to it
    if (ior_output_file != 0)
      {
        FILE *output_file =
          ACE_OS::fopen (ior_output_file, "w");

        if (output_file == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file %s for writing IOR: %C",
                             ior_output_file,
                             ior.in ()),
                            1);

        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);
      }
    else
      ACE_ERROR_RETURN ((LM_ERROR,"ior file name is null\n"),1);

    orb->run ();

    ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

    root_poa->destroy (1,
                       1);

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Exception in setting up server");
    return 1;
  }

  return 0;
}

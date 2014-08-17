
//=============================================================================
/**
 *  @file     server.cpp
 *
 *  $Id$
 *
 *   This is the server program that tests TAO's Smart Proxy extension.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"

// The servant

class Test_i : public POA_Test
{
public:
  Test_i (CORBA::ORB_ptr orb);

  CORBA::Short method  (CORBA::Short boo);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  void shutdown  (void);

private:
  CORBA::ORB_var orb_;

};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Short
Test_i :: method (CORBA::Short boo)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test_i::method () invoked\n")));
  if (boo == 5)
    throw Test::Oops ("Invalid boo\n");

  return 0;
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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_i servant (orb.in ());
      // Obtain RootPOA.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());


      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&servant);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test_var Test_object =
        Test::_narrow (object_act.in ());

      CORBA::String_var ior =
        orb->object_to_string (Test_object.in ());

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file =
            ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);

          ACE_OS::fprintf (output_file,
                           "%s",
                           ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

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

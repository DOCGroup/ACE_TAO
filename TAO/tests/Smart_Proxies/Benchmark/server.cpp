
//=============================================================================
/**
 *  @file     server.cpp
 *
 *   This is the server program that tests TAO's Smart Proxy extension.
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#include "testS.h"
#include "ace/Get_Opt.h"
#include <memory>

struct Options
{
  ACE_TCHAR const *ior_output_file {};
};

// The servant

class Test_i : public POA_Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb);

  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  CORBA::Short box_prices () override;

  CORBA::Long tickets (CORBA::Short number) override;

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Short
Test_i::box_prices ()
{
  return 125;
}

CORBA::Long
Test_i::tickets (CORBA::Short number)
{
  return 125 * number;
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

static int
parse_args (Options &options, int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);
      if (parse_args (options, argc, argv) != 0)
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
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);

          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               options.ior_output_file),
                              1);

          ACE_OS::fprintf (output_file.get (),
                           "%s",
                           ior.in ());
        }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG,
                  "event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception in setting up server");
      ACE_ASSERT (0);
    }
  return 0;
}

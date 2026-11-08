#include "testS.h"
#include "ace/Get_Opt.h"
#include <memory>

struct Options
{
  ACE_TCHAR const *ior_output_file {};
};

class Test_i : public virtual POA_Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb);

  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  void hello (CORBA::Long howmany) override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (options, argc, argv) != 0)
        return 1;

      // Obtain RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_mgr = root_poa->the_POAManager ();

      poa_mgr->activate ();

      // Create a servant.
      Test_i servant (orb.in ());

      PortableServer::ObjectId_var oid = root_poa->activate_object (&servant);

      obj = root_poa->id_to_reference (oid.in ());

      CORBA::String_var ior = orb->object_to_string (obj.in ());

      // If an IOR output file was provided, write the IOR to it.
      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);

          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file %s for writing IOR: %C",
                               options.ior_output_file,
                               ior.in ()),
                              1);

          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR, "ior file name is null\n"), 1);

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception in setting up server");
      return 1;
    }

  return 0;
}

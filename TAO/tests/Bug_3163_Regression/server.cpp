#include <cstdio>
#include <cstdlib>
#include <memory>

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
  return 0;
}

class ctest_impl : public virtual POA_ctest
{
public:
  /// Constructor
  explicit ctest_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }
  ctest_impl (ctest_impl const &) = delete;
  ctest_impl &operator= (ctest_impl const &) = delete;
  ctest_impl (ctest_impl &&) = delete;
  ctest_impl &operator= (ctest_impl &&) = delete;

  CORBA::Long ctestfn (CORBA::Long size, ctest::UCSeq_out data) override
  {
    // The out argument takes ownership of the sequence allocation.
    data = new ctest::UCSeq;

    data->length (size);

    for (CORBA::ULong i = 0; i < data->length (); ++i)
      {
        data[i] = (std::rand () % 26) + 'A';
      }

    return 0;
  }
  void shutdown () override
  {
    this->orb_->shutdown (false);
  }

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var const orb_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      ctest_impl *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      ctest_impl (orb.in ()),
                      1);
      // ServantBase_var takes ownership; hello_impl is a non-owning alias after this transfer.
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      ctest_var const hello = ctest::_narrow (object.in ());

      CORBA::String_var const ior = orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

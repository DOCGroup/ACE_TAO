// -*- C++ -*-

//=============================================================================
/**
 *  @file    server.cpp
 *
 *  A simple server to demonstrate the use of codeset translation
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

// IDL generated header
#include "simpleS.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <memory>

struct Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

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
                           argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

// ------------------------------------------------------------
// Servant for associated CORBA object
// ------------------------------------------------------------
class SimpleImpl : public POA_simple
{
public:
  explicit SimpleImpl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {}

  SimpleImpl (SimpleImpl const &) = delete;
  SimpleImpl (SimpleImpl &&) = delete;
  SimpleImpl &operator= (SimpleImpl const &) = delete;
  SimpleImpl &operator= (SimpleImpl &&) = delete;

  // implementation of corba interface
  char * op1 (char const * name,
              CORBA::Any const & inany,
              CORBA::Any_out outany) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "Server: bare string: %C\n", name));
    char const *any_str {};
    inany >>= any_str;
    ACE_DEBUG ((LM_DEBUG,
                "Server: inserted string: %C\n\n", any_str));

    std::unique_ptr<CORBA::Any> out_ptr = std::make_unique<CORBA::Any> ();
    *out_ptr <<= any_str;
    outany = out_ptr.release ();

    return CORBA::string_dup (name);
  };

  ACE_CDR::WChar * op2 (ACE_CDR::WChar const *s1) override
  {
    return CORBA::wstring_dup (s1);
  };

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override
  {
    this->orb_->shutdown (false);
  };
  //FUZZ: enable check_for_lack_ACE_OS

private:
  /// Use an ORB reference to shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

// ------------------------------------------------------------
// Main routine
// ------------------------------------------------------------
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Init the orb
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      // Initialize POA
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      // Check POA
      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Couldn't initialize POA\n"));
          return 1;
        }

      // Get the ROOT POA
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Get the POA manager
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Create a C++ implementation of CORBA object
      SimpleImpl *my_impl {};
      ACE_NEW_RETURN (my_impl,
                      SimpleImpl (orb.in ()),
                      -1);
      PortableServer::ServantBase_var const safe (my_impl);

      // Create CORBA object for servant and REGISTER with POA
      PortableServer::ObjectId_var const id =
        root_poa->activate_object (my_impl);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      simple_var const server = simple::_narrow (object_act.in ());

      // Get the IOR for our object
      CORBA::String_var const ior = orb->object_to_string (server.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: server.ior"),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      // Activate POA manager
      poa_manager->activate ();

      // Wait for calls
      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in server:");
      return 1;
    }

  return 0;
}

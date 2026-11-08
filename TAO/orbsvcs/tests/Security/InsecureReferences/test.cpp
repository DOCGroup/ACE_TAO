#include "tao/PortableServer/POAC.h"
#include "tao/ORB_Constants.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_strings.h"

#include <memory>

struct InsecureReferences_Options
{
  ACE_TCHAR const *ior_file {ACE_TEXT ("test.ior")};
  ACE_TCHAR const *ior_string {};
  int endpoint_port {12345};
  bool as_server {true};
};

int
parse_args (int argc, ACE_TCHAR *argv[], InsecureReferences_Options &options)
{
  for (int c = 1; c < argc; ++c)
    {
      if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-o")) == 0 ||
          ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-k")) == 0 ||
          ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-p")) == 0)
        {
          if (c + 1 >= argc)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("usage: %s [-o <iorfile>] [-p <port>] [-k <ior>]\n"),
                               argv[0]),
                              -1);

          if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-o")) == 0)
            options.ior_file = argv[++c];
          else if (ACE_OS::strcasecmp (argv[c], ACE_TEXT ("-k")) == 0)
            {
              options.as_server = false;
              options.ior_string = argv[++c];
            }
          else
            {
              ACE_TCHAR *end {};
              const long port = ACE_OS::strtol (argv[++c], &end, 10);
              if (end == argv[c] || *end != ACE_TEXT ('\0') || port < 1 || port > 65525)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("Port must be between 1 and 65525\n")),
                                  -1);
              options.endpoint_port = static_cast<int> (port);
            }
        }
      else if (ACE_OS::strstr (argv[c], ACE_TEXT ("-ORB")) == argv[c])
        {
          if (c + 1 < argc && argv[c + 1][0] != ACE_TEXT ('-'))
            ++c;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-o <iorfile>] [-p <port>] [-k <ior>]\n"),
                           argv[0]),
                          -1);
    }

  return 0;
}

int
gen_objref (ACE_TCHAR *argv[], InsecureReferences_Options const &options)
{
  ACE_TCHAR first_endpoint[100] {};
  ACE_TCHAR second_endpoint[100] {};
  ACE_OS::snprintf (first_endpoint,
                    sizeof (first_endpoint) / sizeof (first_endpoint[0]),
                    ACE_TEXT ("iiop://localhost:%d"),
                    options.endpoint_port);
  ACE_OS::snprintf (second_endpoint,
                    sizeof (second_endpoint) / sizeof (second_endpoint[0]),
                    ACE_TEXT ("iiop://localhost:%d"),
                    options.endpoint_port + 10);

  ACE_TCHAR const first_option[] = ACE_TEXT ("-ORBEndpoint");
  ACE_TCHAR const second_option[] = ACE_TEXT ("-ORBEndpoint");
  ACE_ARGV orb_argv {};
  if (orb_argv.add (argv) == -1 ||
      orb_argv.add (first_option) == -1 ||
      orb_argv.add (first_endpoint) == -1 ||
      orb_argv.add (second_option) == -1 ||
      orb_argv.add (second_endpoint) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Failed to build ORB argument list\n")), 1);

  int orb_argc = orb_argv.argc ();
  CORBA::ORB_var orb {};
  CORBA::Object_var obj {};
  PortableServer::POA_var root_poa {};
  try
    {
      orb = CORBA::ORB_init (orb_argc, orb_argv.argv ());
      obj = orb->resolve_initial_references ("RootPOA");
      root_poa = PortableServer::POA::_narrow (obj.in ());
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                          1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("initialization error ");
      return 1;
    }

  CORBA::Object_var object = root_poa->create_reference ("IDL:JustATest:1.0");
  CORBA::String_var ior = orb->object_to_string (object.in ());
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (options.ior_file, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                       options.ior_file),
                      1);
  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
  return 0;
}

int
parse_objref (int argc, ACE_TCHAR *argv[], InsecureReferences_Options const &options)
{
  CORBA::ORB_var orb {};
  CORBA::Object_var obj {};
  try
    {
      orb = CORBA::ORB_init (argc, argv);
      obj = orb->string_to_object (options.ior_string);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("initialization error ");
      return 1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  InsecureReferences_Options options;
  if (parse_args (argc, argv, options) != 0)
    return 1;
  if (options.as_server)
    return gen_objref (argv, options);
  return parse_objref (argc, argv, options);
}

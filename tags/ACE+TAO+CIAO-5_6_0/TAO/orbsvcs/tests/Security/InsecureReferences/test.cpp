// $Id$

#include "tao/PortableServer/POAC.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"

const char *ior_file = "test.ior";
const char *iorstr = "";

CORBA::Short endpoint_port = 12345;
int verbose = 0;
bool as_server = true;

int
parse_args (int argc, char *argv[])
{
  for (int c = 1; c < argc; c++) {
    if (ACE_OS::strcasecmp(argv[c],"-o") == 0)
      {
        ior_file = argv[++c];
      }
    else if (ACE_OS::strcasecmp(argv[c],"-k") == 0)
      {
        as_server = false;
        iorstr = argv[++c];
      }
    else if (ACE_OS::strcasecmp(argv[c],"-p") == 0)
      {
        endpoint_port = ACE_OS::atoi (argv[++c]);
      }
    else if (ACE_OS::strstr(argv[c],"-ORB") == argv[c])
      {
        c++;
        continue;
      }
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "usage:  %s "
                         "-o <iorfile> "
                         "-p <port> "
                         "-v "
                         "\n",
                         argv [0]),
                        -1);
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
gen_objref (int argc, char *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var obj;
  PortableServer::POA_var root_poa;

  char **largv = new char *[argc+4];
  char *extra[4];
  int i = 0;
  for (i = 0; i < argc; i++)
    largv[i] = argv[i];

  extra[0] = CORBA::string_dup("-ORBEndpoint");
  extra[1] = CORBA::string_alloc(100);
  ACE_OS::sprintf (extra[1],
                   "iiop://localhost:%d",
                   endpoint_port);
  extra[2] = CORBA::string_dup("-ORBEndpoint");
  extra[3] = CORBA::string_alloc(100);
  ACE_OS::sprintf (extra[3],
                   "iiop://localhost:%d",
                   endpoint_port+10);
  for (i = 0; i < 4; i++)
    largv[argc++] = extra[i];

  try
    {
      orb =
        CORBA::ORB_init (argc, largv, "");

      obj =
        orb->resolve_initial_references("RootPOA");

      root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception("initialization error ");
      return 1;
    }

  for (i = 0; i < 4; i++)
    delete[] extra[i];

  delete [] largv;

  CORBA::Object_var o = root_poa->create_reference ("IDL:JustATest:1.0");

  CORBA::String_var ior =
    orb->object_to_string (o.in ());

  FILE *output_file = ACE_OS::fopen (ior_file, "w");
  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       ior_file),
                      1);
  ACE_OS::fprintf (output_file, "%s", ior.in ());
  ACE_OS::fclose (output_file);
  return 0;
}

int
parse_objref (int argc, char *argv[])
{
  CORBA::ORB_var orb;
  CORBA::Object_var obj;

  try
    {
      orb =
        CORBA::ORB_init (argc, argv, "");

      obj = orb->string_to_object (iorstr);
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception("initialization error ");
      return 1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;
  if (as_server)
    return gen_objref(argc, argv);
  return parse_objref(argc, argv);


}

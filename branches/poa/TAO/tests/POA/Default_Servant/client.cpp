#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "FileC.h"

static char *ior = 0;
static char *filename = "test";
static char *message = "POA rules!!";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'f':
        filename = get_opts.optarg;
        break;
      case 'm':
        message = get_opts.optarg;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k IOR"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  parse_args (argc, argv);

  CORBA::Object_var object = orb->string_to_object (ior, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::string_to_object");
      return -1;
    }

  File::System_var file_system = File::System::_narrow (object.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("File::System::_narrow");
      return -1;
    }

  File::Descriptor_var fd = file_system->open (filename, O_CREAT | O_RDWR, env);
  if (env.exception () != 0)
    {
      env.print_exception ("File::System::open");
      return -1;
    }

  int message_length = ACE_OS::strlen (message) + 1;
  CORBA::Octet *buffer = File::Descriptor::DataBuffer::allocbuf (message_length);
  ACE_OS::strcpy ((char *) buffer, message);
  File::Descriptor::DataBuffer data_sent (message_length, message_length, buffer, CORBA::B_TRUE);

  fd->write (data_sent, env);
  if (env.exception () != 0)
    {
      env.print_exception ("File::Descriptor::write");
      return -1;
    }

  fd->lseek (0, SEEK_SET, env);
  if (env.exception () != 0)
    {
      env.print_exception ("File::Descriptor::lseek");
      return -1;
    }

  File::Descriptor::DataBuffer_var data_received = fd->read (message_length, env);
  if (env.exception () != 0)
    {
      env.print_exception ("File::Descriptor::read");
      return -1;
    }

  char *result = (char *) &data_received[0];
  cout << result << endl;

  fd->destroy (env);
  if (env.exception () != 0)
    {
      env.print_exception ("File::Descriptor::destroy");
      return -1;
    }

  return 0;
}

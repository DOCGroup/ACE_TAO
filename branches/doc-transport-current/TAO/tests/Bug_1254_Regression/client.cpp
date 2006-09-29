#include "testC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int main(int argc, char* argv[])
{
    ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      BlobServer_var blobServer = BlobServer::_narrow(tmp.in());
      if(CORBA::is_nil(blobServer.in()))
      {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test\n"));

      BlobType1 blob1;

      blob1.length(16384);
      for(CORBA::ULong i = 0; i <  blob1.length(); i++)
        {
          blob1[i] = i & 255;
        }

      BlobType2 blob2;

      blobServer->test(blob1, blob2);

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - checking reply for ABCDEFGHIJ\n"));

      for(CORBA::ULong i = 0; i <  blob1.length(); i++)
        {
          if (blob1[i] != static_cast <CORBA::Octet> ('A' + i))
            {
              ACE_ERROR ((LM_ERROR,
                         "Error invalid value detected on position <%d> in blob1\n",
                         i));
            }
        }

      for(CORBA::ULong i = 0; i <  blob2.length(); i++)
        {
         if (blob2[i] != static_cast <CORBA::Octet> ('A' + i))
           {
             ACE_ERROR ((LM_ERROR,
                        "Error invalid value detected on position <%d> in blob2\n",
                        i));
           }
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - shutdown\n"));

      blobServer->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}


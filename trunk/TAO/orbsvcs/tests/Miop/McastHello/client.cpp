// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/Object_T.h"
#include "orbsvcs/PortableGroup/MIOP.h"

ACE_RCSID(Hello, client, "$Id$")

#define MAX_MIOP_OCTET_SEQUENCE  (ACE_MAX_DGRAM_SIZE - 272 /* MIOP_MAX_HEADER_SIZE */)

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
  // Indicates successful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);


      /* Do an unchecked narrow since there's no way to do an is_a on
       * a multicast reference (yet...).
       */
      Test::McastHello_var hello =
        TAO::Narrow_Utils<Test::McastHello>::unchecked_narrow (
            tmp.in (),
            Test__TAO_McastHello_Proxy_Broker_Factory_function_pointer
          );

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      hello->send_forty_two (42 ACE_ENV_ARG_PARAMETER);

      Test::Octets payload (MAX_MIOP_OCTET_SEQUENCE);
      payload.length (MAX_MIOP_OCTET_SEQUENCE);

      for (CORBA::ULong j = 0; j != MAX_MIOP_OCTET_SEQUENCE; ++j)
        {
          payload[j] = j % 256;
        }

      hello->send_large_octet_array (payload ACE_ENV_ARG_PARAMETER);

      hello->shutdown ();

      orb->destroy ();
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

// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/Object_T.h"
#include "orbsvcs/PortableGroup/MIOP.h"



#define MAX_MIOP_OCTET_SEQUENCE  (ACE_MAX_DGRAM_SIZE - 272 /* MIOP_MAX_HEADER_SIZE */)

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      /* Do an unchecked narrow since there's no way to do an is_a on
       * a multicast reference (yet...).
       */
      Test::McastHello_var hello =
        TAO::Narrow_Utils<Test::McastHello>::unchecked_narrow (
            tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      hello->send_forty_two (42);

      Test::Octets payload (MAX_MIOP_OCTET_SEQUENCE);
      payload.length (MAX_MIOP_OCTET_SEQUENCE);

      for (CORBA::ULong j = 0; j != MAX_MIOP_OCTET_SEQUENCE; ++j)
        {
          payload[j] = j % 256;
        }

      hello->send_large_octet_array (payload);

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

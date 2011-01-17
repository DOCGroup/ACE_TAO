// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Strategies/advanced_resource.h"

const ACE_TCHAR *ior = ACE_TEXT("server.ior");

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

      int done = 0;
      test_var holder;
      for(int i = 0; !done; i++)
        {
          ACE_TCHAR number[64];
          ACE_TString iorfile(ior);

          ACE_OS::sprintf (number, ACE_TEXT(".%d"), i);
          iorfile += number;

          if (ACE_OS::access(iorfile.c_str (), R_OK) == 0)
            {
              iorfile = ACE_TEXT("file://") + iorfile;
              CORBA::Object_var tmp =
                orb->string_to_object(iorfile.c_str ());

              test_var test =
                test::_narrow(tmp.in ());

              if (CORBA::is_nil (test.in ()))
                {
                  ACE_ERROR_RETURN ((LM_DEBUG,
                                     "Nil test reference <%s>\n",
                                     ior),
                                    1);
                }

              test->send_stuff ("Some stuff to send");

              // Test for LFU strategy.  The transport to any other
              // server should be removed before the first one.
              if (i == 0)
                {
                  test->send_stuff ("Some stuff to send");

                  holder = test;
                }
              else if (i == 5)  // let a few go by before we reuse holder
                {               // This will test the LRU strategy
                                // This transport should be the 6th one
                                // removed.
                  if (!CORBA::is_nil(holder.in ()))
                    {
                      holder->send_stuff ("Some stuff to send");
                    }
                }
            }
          else
            {
              done = 1;
            }
        }

      orb->shutdown (1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

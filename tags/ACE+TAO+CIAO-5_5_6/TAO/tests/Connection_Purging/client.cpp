// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Strategies/advanced_resource.h"

const char *ior = "server.ior";

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

int
main (int argc, char *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        return 1;

      int done = 0;
      test_var holder;
      for(int i = 0; !done; i++)
        {
          char number[64];
          ACE_CString iorfile(ior);

          ACE_OS::sprintf (number, ".%d", i);
          iorfile += number;

          if (ACE_OS::access(iorfile.c_str (), R_OK) == 0)
            {
              iorfile = "file://" + iorfile;
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

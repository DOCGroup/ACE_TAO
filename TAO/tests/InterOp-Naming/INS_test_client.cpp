#include "INSC.h"

#include "tao/ObjectIdListC.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int i {};

  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));

      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("No Name specified to be resolved\n")));
          return -1;
        }

      for (i = 1; i < argc; ++i)
        {
          if (ACE_OS::strcmp (argv [i], ACE_TEXT ("-l")) == 0)
            {
              CORBA::ORB::ObjectIdList_var const list (orb->list_initial_services ());
              size_t const length = list->length ();

              if (length > 0)
                {
                  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Listing initial references...\n")));

                  for (CORBA::ULong n {}; n < length; ++n)
                    ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("  Reference %u: %C\n"),
                                n,
                                static_cast<char const *> (list [n])));
                }
              else
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("\nNo initial references. "
                                               "This condition should not happen.\n")),
                                    -1);
                }
            }
          else
            {
              CORBA::Object_var const objref (
                orb->resolve_initial_references (ACE_TEXT_ALWAYS_CHAR (argv [i])));

              if (CORBA::is_nil (objref.in ()))
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT (" (%P|%t) Unable to resolve the given name.\n")),
                                  -1);

              INS_var const server (INS::_narrow (objref.in ()));
              CORBA::String_var const iorstr (orb->object_to_string (server.in ()));
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Resolved IOR for %s : %C\n"),
                          argv [i],
                          iorstr.in ()));

              CORBA::String_var const test_ins_result (server->test_ins ());
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("\nResult of Remote Call : %C\n"),
                          test_ins_result.in ()));
            }
        }
    }
  catch (CORBA::ORB::InvalidName const &)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Cannot resolve <%s>\n"), argv [i]));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception:");
      return -1;
    }

  return 0;
}

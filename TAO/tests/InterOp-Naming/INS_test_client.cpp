// $Id$


#include "INSC.h"

#include "tao/ObjectIdListC.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int i = 0;

  try
    {
      // Retrieve a reference to the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "No Name specified to be resolved\n"));
          return -1;
        }

      CORBA::Object_var objref;

      for (i = 1; i < argc; ++i)
        {
          if (ACE_OS::strcmp (argv[i], "-l") == 0)
            {
              // List initial services
              CORBA::ORB::ObjectIdList_var list =
                orb->list_initial_services ();

              size_t length = list->length ();

              if (length > 0)
                {
                  ACE_DEBUG ((LM_INFO,
                              "Listing initial references...\n"));

                  for (CORBA::ULong n = 0; n < length; ++n)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  "  Reference %u: %s\n",
                                  n,
                                  ACE_TEXT_CHAR_TO_TCHAR (static_cast<char const*>(list[n]))));
                    }
                }
              else
                {
                  // We should at least get TAO's initial references,
                  // such as "RootPOA", "NameService", etc.
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "\n"
                                     "No initial references. "
                                     "This condition shouldn't happen.\n"),
                                    -1);
                }
            }
          else
            {
              objref = orb->resolve_initial_references (argv[i]);

              if (CORBA::is_nil (objref.in ()))
                ACE_ERROR_RETURN ((LM_ERROR,
                                   " (%P|%t) Unable to resolve the "
                                   "given name.\n"),
                                  -1);

              INS_var server = INS::_narrow (objref.in ());

              CORBA::String_var iorstr =
                orb->object_to_string (server.in ());
              ACE_DEBUG ((LM_DEBUG,
                          "Resolved IOR for %s : %s\n",
                          argv[i],
                          ACE_TEXT_CHAR_TO_TCHAR (iorstr.in())));

              CORBA::String_var test_ins_result =
                server->test_ins ();

              ACE_DEBUG ((LM_DEBUG,
                          "\nResult of Remote Call : %s\n",
                          ACE_TEXT_CHAR_TO_TCHAR (test_ins_result.in ())));
            }
        }
    }
  catch (const CORBA::ORB::InvalidName& )
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot resolve <%s>\n", argv[i]));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception:");
    }


  return 0;
}

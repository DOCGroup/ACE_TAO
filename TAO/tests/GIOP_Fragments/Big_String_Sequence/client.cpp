#include "EchoC.h"
#include "ace/Get_Opt.h"
#include <cstring>

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-f <ior> "
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
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return -1;

    CORBA::Object_var tmp = orb->string_to_object(ior);

    Echo_var echo = Echo::_narrow(tmp.in ());

    if (CORBA::is_nil (echo.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Nil Echo reference <%s>\n",
                         ior),
                        -1);
    }

    Echo::List_var list = echo->return_list();

    ACE_DEBUG ((LM_DEBUG,
                "Received list of length %u\n",
                list->length()));
    if (list->length() != 2)
    {
       ACE_ERROR_RETURN ((LM_ERROR, "Expected length 2\n"), -1);
    }
    const char* value = (*list)[0].in();
    size_t length = std::strlen(value);
    ACE_DEBUG ((LM_DEBUG,
                "First element has length %u\n",
                length));
    for (size_t n = 0; n < length; ++n)
    {
       if (value[n] != 'A')
       {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Character at position %u should be 'A',"
                             " but is '%c'\n", value[n]), -1);
       }
    }
    value = (*list)[1].in();
    length = std::strlen(value);
    ACE_DEBUG ((LM_DEBUG,
                "Second element has length %u, value: <%C>\n",
                length, value));
    if (strcmp(value, "Hello World") != 0)
    {
       ACE_ERROR_RETURN ((LM_ERROR, "Expected \"Hello World\""), -1);
    }

    Echo::WList_var wlist = echo->return_wlist();

    ACE_DEBUG ((LM_DEBUG,
                "Received wide list of length %u\n",
                wlist->length()));
    if (wlist->length() != 2)
    {
       ACE_ERROR_RETURN ((LM_ERROR, "Expected length 2\n"), -1);
    }

    echo->shutdown ();
    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

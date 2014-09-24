// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/Utils/catior
//
// = FILENAME
//    catior.cpp
//
// = DESCRIPTION
//    Reads stringified IORs and outputs the encoded information.
//
// = AUTHORS
//      Jeff Hopper <jrhopper@cts.com>
//      SCIOP and Tagged component modifications by:
//      Jason Cohen, Lockheed Martin ATL <jcohen@atl.lmco.com>
//
// ============================================================================

#include "Catior_i.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "tao/ORB.h"
#include "orbsvcs/CosNamingC.h"

int
ACE_TMAIN (int argcw, ACE_TCHAR *argvw[])
{
  CORBA::ORB_var orb_var;
  try
    {
      Catior_i catior_impl;
      orb_var =  CORBA::ORB_init (argcw, argvw);
      CORBA::Boolean b = false;
      CORBA::Boolean have_argument = false;
      int opt;

      ACE_Get_Opt get_opt (argcw, argvw, ACE_TEXT ("f:n:x"));

      while ((opt = get_opt ()) != EOF)
        {
          // some arguments have been supplied
          have_argument = 1;
          switch (opt)
            {
            case 'n':
              {
                //  Read the CosName from the NamingService convert the
                //  object_ptr to a CORBA::String_var via the call to
                //  object_to_string.
                ACE_DEBUG ((LM_DEBUG,
                            "opening a connection to the NamingService\n"
                            "resolving the CosName %s\n",
                            get_opt.opt_arg ()));

                CORBA::Object_var server_object;

                try
                  {
                    // Find the Naming Service.
                    CORBA::Object_var naming_context_object =
                      orb_var->resolve_initial_references ("NameService");
                    CosNaming::NamingContextExt_var naming_context =
                      CosNaming::NamingContextExt::_narrow (naming_context_object.in ());

                    if (CORBA::is_nil (naming_context.in ()))
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                        "NameService cannot be resolved\n"),
                                      -1);
                    }

                    CosNaming::Name *name =
                      naming_context->to_name (ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ()));

                    try
                      {
                        server_object = naming_context->resolve (*name);
                          if (CORBA::is_nil (server_object.in ()))
                          {
                            ACE_ERROR_RETURN ((LM_ERROR,
                                  "name is not resolved to a valid object\n"),
                                              -1);
                          }
                      }
                    catch (const CosNaming::NamingContext::NotFound& nf)
                      {
                        const ACE_TCHAR *reason;

                        switch (nf.why)
                          {
                            case CosNaming::NamingContext::missing_node:
                              reason = ACE_TEXT ("missing node");
                              break;
                            case CosNaming::NamingContext::not_context:
                              reason = ACE_TEXT ("not context");
                              break;
                            case CosNaming::NamingContext::not_object:
                              reason = ACE_TEXT ("not object");
                              break;
                            default:
                              reason = ACE_TEXT ("not known");
                              break;
                          }
                        ACE_ERROR_RETURN ((LM_ERROR,
                                  "%s cannot be resolved, exception reason = %s\n",
                                          get_opt.opt_arg (),
                                          reason),
                                        -1);
                      }
                    catch (const CosNaming::NamingContext::InvalidName&)
                      {
                        ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s cannot be resolved, exception reason = "
                                          "Invalid Name"
                                          "\n",
                                          get_opt.opt_arg ()),
                                        -1);
                      }
                    catch (const CosNaming::NamingContext::CannotProceed&)
                      {
                        ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s cannot be resolved, exception reason = "
                                          "Cannot Proceed"
                                          "\n",
                                          get_opt.opt_arg ()),
                                        -1);
                      }
                    catch (const CORBA::Exception&)
                      {
                        ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s cannot be resolved, exception reason = "
                                          "Unexpected Exception"
                                          "\n",
                                          argvw[0]),
                                          -1);
                      }

                    ACE_CString aString;

                    aString = orb_var->object_to_string (server_object.in ());

                    ACE_DEBUG ((LM_DEBUG,
                                "\nhere is the IOR\n%C\n\n",
                                aString.rep ()));

                    ACE_CString str;
                    b = catior_impl.decode(aString, str);
                    ACE_DEBUG ((LM_DEBUG, "%s", str.c_str()));
                  }
                catch (const CORBA::Exception&)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s cannot be resolved, exception reason = "
                                      "Unexpected Exception"
                                      "\n",
                                      argvw[0]),
                                      -1);
                  }

                if (b == 1)
                  ACE_DEBUG ((LM_DEBUG,
                              "catior returned true\n"));
                else
                  ACE_DEBUG ((LM_DEBUG,
                              "catior returned false\n"));
                break;
              }
            case 'f':
              {
                int have_some_input = 0;
                int decode_pass_count = 0;

                //  Read the file into a CORBA::String_var.
                ACE_DEBUG ((LM_DEBUG,
                            "reading the file %s\n",
                            get_opt.opt_arg ()));

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
                ifstream ifstr (ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ()));

                if (!ifstr.good ())
                  {
                    ifstr.close ();
                    ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s "
                                      "-f %s "
                                      "\n"
                                      "Invalid IOR file nominated"
                                      "\n",
                                      argvw[0],
                                      get_opt.opt_arg ()),
                                      -1);
                  }

                while (!ifstr.eof())
                  {
                    ACE_CString aString;

                    have_some_input = 0;

                    while (!ifstr.eof ())
                      {
                        char ch = 0;
                        ifstr.get (ch);
                        if (ifstr.eof () || ch == '\n' || ch == '\r')
                          break;
                        aString += ch;
                        ++have_some_input;
                      }
#else
                FILE* ifstr = ACE_OS::fopen (get_opt.opt_arg (), ACE_TEXT ("r"));

                if (!ifstr || ferror (ifstr))
                  {
                    if (ifstr)
                      {
                      ACE_OS::fclose (ifstr);
                      }
                      ACE_ERROR_RETURN ((LM_ERROR,
                                        "%s "
                                        "-f %s "
                                        "\n"
                                        "Invalid IOR file nominated"
                                        "\n",
                                        argvw[0],
                                        get_opt.opt_arg ()),
                                        -1);
                  }

                while (!feof (ifstr))
                  {
                    char ch;
                    ACE_CString aString;

                    have_some_input = 0;

                    while (!feof (ifstr))
                      {
                        ch = ACE_OS::fgetc (ifstr);
                        if (ch == EOF || ch == '\n' || ch == '\r')
                          break;
                        aString += ch;
                        ++have_some_input;
                      }
#endif /* !defined (ACE_LACKS_IOSTREAM_TOTALLY) */
                    if (have_some_input == 0 || !aString.length())
                      {
                        if (!decode_pass_count)
                          {
                              ACE_ERROR_RETURN ((LM_ERROR,
                                                "%s "
                                                "-f %s "
                                                "\n"
                                                "Empty IOR file nominated"
                                                "\n",
                                                argvw[0],
                                                get_opt.opt_arg ()),
                                                -1);
                          }
                        else
                          {
                            ACE_DEBUG ((LM_DEBUG,
                                        "catior returned true\n"));
                            return 0;               // All done now
                          }
                      }

                    ++decode_pass_count;

                    ACE_DEBUG ((LM_DEBUG,
                                "IOR %d: here is the IOR\n%C\n\n",
                                decode_pass_count, aString.rep ()));

                    ACE_CString str;
                    Catior_i fcatior_impl;

                    b = fcatior_impl.decode(aString, str);
                    ACE_DEBUG ((LM_DEBUG, "%s", str.c_str()));
                  }
                if (b == 1)
                  ACE_DEBUG ((LM_DEBUG,
                              "catior returned true\n"));
                else
                  ACE_DEBUG ((LM_DEBUG,
                              "catior returned false\n"));
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
                ifstr.close ();
#else
                ACE_OS::fclose (ifstr);
#endif /* !defined (ACE_LACKS_IOSTREAM_TOTALLY) */
              }
              break;
            case 'x':
              {
                int have_some_input = 0;
                int decode_pass_count = 0;

                //  Read the input into a CORBA::String_var.
                ACE_DEBUG ((LM_DEBUG,
                            "reading from stdin\n"));

    #if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
                if (!cin.good ())
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s "
                                      "-x"
                                      "\n"
                                      "Invalid input stream"
                                      "\n",
                                      argvw[0]),
                                      -1);
                  }

                while (!cin.eof())
                  {
                    ACE_CString aString;

                    have_some_input = 0;

                    while (!cin.eof ())
                      {
                        char ch = 0;
                        cin.get (ch);
                        if (cin.eof () || ch == '\n' || ch == '\r')
                          break;
                        aString += ch;
                        ++have_some_input;
                      }
#else
                FILE* ifstr = stdin;

                if (!ifstr || ferror (ifstr))
                  {
                    if (ifstr)
                      {
                      ACE_OS::fclose (ifstr);
                      }
                    ACE_ERROR_RETURN ((LM_ERROR,
                                      "%s "
                                      "-x"
                                      "\n"
                                      "Invalid input stream"
                                      "\n",
                                      argvw[0]),
                                      -1);
                  }

                while (!feof (ifstr))
                  {
                    char ch;
                    ACE_CString aString;

                    have_some_input = 0;

                    while (!feof (ifstr))
                      {
                        ch = ACE_OS::fgetc (ifstr);
                        if (ch == EOF || ch == '\n' || ch == '\r')
                          break;
                        aString += ch;
                        ++have_some_input;
                      }
#endif /* !defined (ACE_LACKS_IOSTREAM_TOTALLY) */

                    if (have_some_input == 0)
                      {
                        if (!decode_pass_count)
                          {
                            ACE_ERROR_RETURN ((LM_ERROR,
                                              "%s "
                                              "-x"
                                              "\n"
                                              "Empty input stream"
                                              "\n",
                                              argvw[0]),
                                              -1);
                          }
                        else
                          {
                            return 0;               // All done now
                          }
                      }

                    ++decode_pass_count;

                    ACE_DEBUG ((LM_DEBUG,
                                "IOR: %d\nhere is the IOR\n%C\n\n",
                                decode_pass_count, aString.rep ()));

                    ACE_CString str;
                    b = catior_impl.decode(aString, str);
                    ACE_DEBUG ((LM_DEBUG, "%s", str.c_str()));
                  }
                if (b == 1)
                  ACE_DEBUG ((LM_DEBUG,
                              "catior returned true\n"));
                else
                  ACE_DEBUG ((LM_DEBUG,
                              "catior returned false\n"));
              }
              break;
            case '?':
            case 'h':
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                "Usage: %s "
                                "-f filename "
                                "-n CosName "
                                "\n"
                                "Reads an IOR "
                                "and dumps the contents to stdout "
                                "\n",
                                argvw[0]),
                                1);
            }
        }

        // check that some relevant arguments have been supplied
        if (have_argument == 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                "Usage: %s "
                                "-f filename "
                                "-n CosName "
                                "\n"
                                "Reads an IOR "
                                "and dumps the contents to stdout "
                                "\n",
                                argvw[0]),
                                1);
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "\nError:\n"));
      ex._tao_print_exception ("Exception in nsadd");
      orb_var->destroy ();
      return 1;
    }
  return 0;
}

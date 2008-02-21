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

#include "tao/Messaging_PolicyValueC.h"
#include "tao/Messaging/Messaging_TypesC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/IIOP_Profile.h"
#include "tao/ORB_Constants.h"
#include "tao/Transport_Acceptor.h"
#include "tao/IIOP_EndpointsC.h"
#include "tao/Tagged_Components.h"
#include "tao/CDR.h"
#include "ace/Codeset_Registry.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Log_Msg.h"
#include "orbsvcs/CosNamingC.h"

CORBA::Boolean
catiiop (char* string)
{
  // NIL objref encodes as just "iiop:" ... which has already been
  // removed, so we see it as an empty string.

  if (!string || !*string)
    return 0;

  // Type ID not encoded in this string ... makes narrowing rather
  // expensive, though it does ensure that type-safe narrowing code
  // gets thoroughly excercised/debugged!  Without a typeID, the
  // _narrow will be required to make an expensive remote "is_a" call.

  // Remove the "N.N//" prefix, and verify the version's one that we
  // accept

  CORBA::Short  iiop_version_major, iiop_version_minor;

  if (ACE_OS::ace_isdigit (string [0])
      && ACE_OS::ace_isdigit (string [2])
      && string [1] == '.'
      && string [3] == '/'
      && string [4] == '/')
    {
      iiop_version_major = (char) (string [0] - '0');
      iiop_version_minor = (char) (string [2] - '0');
      string += 5;
    }
  else
    {
      iiop_version_major = 0;
      iiop_version_minor = 0;
      string += 2;
    }

  ACE_DEBUG ((LM_DEBUG,
              "IIOP Version:\t%d.%d\n",
              iiop_version_major,
              iiop_version_minor));

  // Pull off the "hostname:port/" part of the objref Get host and
  // port.
  CORBA::UShort port_number;
  char *cp = ACE_OS::strchr (string, ':');

  if (cp == 0)
    {
      throw CORBA::DATA_CONVERSION ();
    }

  CORBA::String_var hostname = CORBA::string_alloc (1 + cp - string);

  for (cp = hostname.inout ();
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  cp = ACE_OS::strchr ((char *) string, '/');

  if (cp == 0)
    {
      throw CORBA::DATA_CONVERSION ();
    }

  port_number = (short) ACE_OS::atoi ((char *) string);
  string = ++cp;

  ACE_DEBUG ((LM_DEBUG,
              "Host Name:\t%s\n",
              hostname.in ()));
  ACE_DEBUG ((LM_DEBUG,
              "Port Number:\t%d\n",
              port_number));

  // Parse the object key.
  // dump the object key to stdout
  //  TAO_POA::decode_string_to_sequence (data->profile.object_key,
  //                                      string);
  ACE_DEBUG ((LM_DEBUG,
              "\nThe Object Key as string:\n%s\n",
              string));
  return 1;
}

CORBA::Boolean
cat_iiop_profile (TAO_InputCDR& cdr);

CORBA::Boolean
cat_sciop_profile (TAO_InputCDR& cdr);

CORBA::Boolean
cat_uiop_profile (TAO_InputCDR& cdr);

CORBA::Boolean
cat_shmiop_profile (TAO_InputCDR& cdr);

CORBA::Boolean
cat_coiop_profile (TAO_InputCDR& cdr);

CORBA::Boolean
cat_nskpw_profile (TAO_InputCDR& cdr);

static CORBA::Boolean
cat_nskfs_profile (TAO_InputCDR& cdr);

CORBA::Boolean
cat_octet_seq (const char *object_name,
               TAO_InputCDR& stream);

CORBA::Boolean
cat_profile_helper(TAO_InputCDR& stream, const char *protocol);

CORBA::Boolean
catior (char const * str)
{
  // Unhex the bytes, and make a CDR deencapsulation stream from the
  // resulting data.

  ACE_Message_Block mb (ACE_OS::strlen (str)  / 2 + 1
                        + ACE_CDR::MAX_ALIGNMENT);
  ACE_CDR::mb_align (&mb);

  char * const buffer = mb.rd_ptr ();
  char const * tmp = (char *) str;
  size_t len = 0;

  CORBA::Boolean continue_decoding;

  // The prefix of the IOR must be removed, and the string must start
  // with the encapsulation byte

  while (tmp [0] && tmp [1])
    {
      u_char byte;

      if (! (ACE_OS::ace_isxdigit (tmp [0]) && ACE_OS::ace_isxdigit (tmp [1])))
        break;

      byte = (u_char) (ACE::hex2byte (tmp [0]) << 4);
      byte |= ACE::hex2byte (tmp [1]);

      buffer[len++] = byte;
      tmp += 2;
    }

  // Create deencapsulation stream ... then unmarshal objref from that
  // stream.

  int byteOrder = *(mb.rd_ptr ());

  mb.rd_ptr (1);
  mb.wr_ptr (len);
  TAO_InputCDR stream (&mb, static_cast<int> (byteOrder));

  if (byteOrder == 1)
    ACE_DEBUG ((LM_DEBUG,
                "The Byte Order:\tLittle Endian\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "The Byte Order:\tBig Endian\n"));

  // First, read the type hint. This will be the type_id encoded in an
  // object reference.
  CORBA::String_var type_hint;

  if (!(stream >> type_hint.inout ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read type id\n"));
      return TAO::TRAVERSE_STOP;
    }

  ACE_DEBUG ((LM_DEBUG,
              "The Type Id:\t\"%s\"\n",
              type_hint.in ()));

  // Read the profiles, discarding all until an IIOP profile comes by.
  // Once we see an IIOP profile, ignore any further ones.
  //
  // XXX this will need to change someday to let different protocol
  // code be accessed, not just IIOP.  Protocol modules will be
  // dynamically loaded from shared libraries via ORB_init (), and we
  // just need to be able to access such preloaded libraries here as
  // we unmarshal objrefs.

  CORBA::ULong profiles = 0;

  continue_decoding = stream.read_ulong (profiles);

  // Get the count of profiles that follow.
  if (continue_decoding == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read the profile count\n"));
      return TAO::TRAVERSE_STOP;
    }

  CORBA::ULong profile_counter = 0;

  ACE_DEBUG ((LM_DEBUG,
              "Number of Profiles in IOR:\t%d\n",
              profiles));

  // No profiles means a NIL objref.
  if (profiles == 0)
    return TAO::TRAVERSE_CONTINUE;
  else
    while (profiles-- != 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Profile number:\t%d\n",
                    ++profile_counter));

        // We keep decoding until we find a valid IIOP profile.
        CORBA::ULong tag;

        continue_decoding = stream.read_ulong (tag);

        // Get the profile ID tag.
        if (continue_decoding == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "cannot read profile tag\n"));
            continue;
          }

        if (tag == IOP::TAG_INTERNET_IOP)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding = cat_iiop_profile (stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }

        else if (tag == TAO_TAG_SCIOP_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding = cat_sciop_profile (stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }

        else if (tag == TAO_TAG_UIOP_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding = cat_uiop_profile (stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
        else if (tag == TAO_TAG_SHMEM_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding = cat_shmiop_profile (stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
        else if (tag == TAO_TAG_DIOP_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding =  cat_profile_helper(stream, "DIOP (GIOP over UDP)");
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
        else if (tag == TAO_TAG_COIOP_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding =  cat_coiop_profile(stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
        else if (tag == TAO_TAG_NSKPW_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding = cat_nskpw_profile (stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
        else if (tag == TAO_TAG_NSKFS_PROFILE)
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            continue_decoding = cat_nskfs_profile (stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "%{"));
            ACE_DEBUG ((LM_DEBUG,
                        "%I Profile tag = %d (unknown protocol)\n", tag));
            continue_decoding = cat_octet_seq("Profile body", stream);
            ACE_DEBUG ((LM_DEBUG, "%}"));
          }
      }
  return 1;
}

// Parse the Orbix-style POOP object references, which have the form:
//:\ntlj3corba:NS:NC_2::IR:CosNaming_NamingContext
// :\ hostname
// : server_name
// : marker
// : IR_host
// : IR_server
// : interface_marker

CORBA::Boolean
catpoop (char* string)
{
  if (!string || !*string)
    return 0;

  if (string [0] == ':'
      && string [1] == '\\')
    ACE_DEBUG ((LM_DEBUG,
                "\nPerhaps we've found some POOP\n"));
  string += 2;

  char *cp = ACE_OS::strchr (string, ':');

  if (cp == 0)
    {
      throw CORBA::DATA_CONVERSION ();
    }

  // Read the hostname.
  char* hostname = CORBA::string_alloc (1 + cp - string);

  for (cp = hostname;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  ACE_DEBUG ((LM_DEBUG,
              "Host Name:\t%s\n",
              hostname));
  CORBA::string_free (hostname);

  //  read the server name
  cp = ACE_OS::strchr (string, ':');

  char* server_name = CORBA::string_alloc (1 + cp - string);

  for (cp = server_name;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  ACE_DEBUG ((LM_DEBUG,
              "Server Name:\t%s\n",
              server_name));

  CORBA::string_free (server_name);

  // Read the Orbix specific marker.
  cp = ACE_OS::strchr (string, ':');

  char* marker = CORBA::string_alloc (1 + cp - string);

  for (cp = marker;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  ACE_DEBUG ((LM_DEBUG,
              "Marker:\t\t%s\n",
              marker));
  CORBA::string_free (marker);

  cp = ACE_OS::strchr (string, ':');

  // Read the IR_host.
  char* IR_host = CORBA::string_alloc (1 + cp - string);

  for (cp = IR_host;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  ACE_DEBUG ((LM_DEBUG,
              "IR Host:\t\t%s\n",
              IR_host));
  CORBA::string_free (IR_host);

  // Read the IR_server
  cp = ACE_OS::strchr (string, ':');

  char* IR_server = CORBA::string_alloc (1 + cp - string);

  for (cp = IR_server;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  ACE_DEBUG ((LM_DEBUG,
              "IR Server:\t\t%s\n",
              IR_server));
  CORBA::string_free (IR_server);

  // Read the interface_marker
  ACE_DEBUG ((LM_DEBUG,
              "Interface Marker:\t%s\n",
              string));
  return 1;
}

int
ACE_TMAIN (int argcw, ACE_TCHAR *argvw[])
{
  ACE_Argv_Type_Converter argcon (argcw, argvw);
  CORBA::ORB_var orb_var =  CORBA::ORB_init (argcon.get_argc (),
                                             argcon.get_ASCII_argv (),
                                             "TAO");
  CORBA::Boolean b = 0;
  CORBA::Boolean have_argument = 0;
  int opt;

  ACE_Get_Opt get_opt (argcon.get_argc (), argcon.get_TCHAR_argv (),
                       ACE_TEXT ("f:n:x"));

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
                  naming_context->to_name (get_opt.opt_arg ());

                try
                  {
                    server_object = naming_context->resolve (*name);
                      if (CORBA::is_nil (server_object.in ()))
                      {
                        ACE_ERROR_RETURN ((LM_ERROR,
                              "name %s is not resolved to a valid object\n"),
                                          -1);
                      }
                  }
                catch (const CosNaming::NamingContext::NotFound& nf)
                  {
                    const char     *reason;

                    switch (nf.why)
                      {
                        case CosNaming::NamingContext::missing_node:
                          reason = "missing node";
                          break;
                        case CosNaming::NamingContext::not_context:
                          reason = "not context";
                          break;
                        case CosNaming::NamingContext::not_object:
                          reason = "not object";
                          break;
                        default:
                          reason = "not known";
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
                            "\nhere is the IOR\n%s\n\n",
                            aString.rep ()));

                char * str = 0;
                if (aString.find ("IOR:") == 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an IOR:\n"));

                    // Strip the IOR: off the string.
                    ACE_CString prefix = "IOR:";
                    size_t prefixLength = prefix.length ();

                    ACE_CString subString =
                      aString.substring (prefixLength,
                                         aString.length () - prefixLength);
                    subString[subString.length ()] = '\0';
                    str = subString.rep ();
                    b = catior (str);
                  }
                else if (aString.find ("iiop:") == 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an IIOP URL IOR\n"));

                    ACE_CString prefix = "IIOP:";
                    size_t prefixLength = prefix.length ();

                    ACE_CString subString =
                      aString.substring (prefixLength,
                                         aString.length () - prefixLength);
                    //subString[subString.length () - 1] = '\0';
                    str = subString.rep ();
                    b = catiiop (str);
                  }
                else if (aString.find (":IR:") != ACE_CString::npos)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an POOP IOR\n"));

                    str = aString.rep ();
                    b = catpoop (str);
                  }
                else
                  ACE_ERROR ((LM_ERROR,
                             "Don't know how to decode this IOR\n"));

                delete [] str;
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
            ifstream ifstr (get_opt.opt_arg ());

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
                            "\nhere is the IOR\n%s\n\n",
                            aString.rep ()));

                char* str;
                if (aString.find ("IOR:") == 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an IOR:\n"));

                    // Strip the IOR: off the string.
                    ACE_CString prefix = "IOR:";
                    size_t prefixLength = prefix.length ();

                    ACE_CString subString =
                      aString.substring (prefixLength,
                                         aString.length () - prefixLength);
                    subString[subString.length ()] = '\0';
                    str = subString.rep ();
                    b = catior (str);
                  }
                else if (aString.find ("iiop:") == 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an IIOP URL IOR\n"));

                    ACE_CString prefix = "IIOP:";
                    size_t prefixLength = prefix.length ();

                    ACE_CString subString =
                      aString.substring (prefixLength,
                                         aString.length () - prefixLength);
                    //subString[subString.length () - 1] = '\0';
                    str = subString.rep ();
                    b = catiiop (str);
                  }
                else if (aString.find (":IR:") != ACE_CString::npos)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an POOP IOR\n"));

                    str = aString.rep ();
                    b = catpoop (str);
                  }
                else
                  ACE_ERROR ((LM_ERROR,
                             "Don't know how to decode this IOR\n"));
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
                            "\nhere is the IOR\n%s\n\n",
                            aString.rep ()));

                char* str;
                if (aString.find ("IOR:") == 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an IOR:\n"));

                    // Strip the IOR: off the string.
                    ACE_CString prefix = "IOR:";
                    size_t prefixLength = prefix.length ();

                    ACE_CString subString =
                      aString.substring (prefixLength,
                                         aString.length () - prefixLength);
                    subString[subString.length ()] = '\0';
                    str = subString.rep ();
                    b = catior (str);
                  }
                else if (aString.find ("iiop:") == 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an IIOP URL IOR\n"));

                    ACE_CString prefix = "IIOP:";
                    size_t prefixLength = prefix.length ();

                    ACE_CString subString =
                      aString.substring (prefixLength,
                                         aString.length () - prefixLength);
                    //subString[subString.length () - 1] = '\0';
                    str = subString.rep ();
                    b = catiiop (str);
                  }
                else if (aString.find (":IR:") != ACE_CString::npos)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "decoding an POOP IOR\n"));

                    str = aString.rep ();
                    b = catpoop (str);
                  }
                else
                  ACE_ERROR ((LM_ERROR,
                             "Don't know how to decode this IOR\n"));
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
  return 0;
}


CORBA::Boolean
cat_tag_orb_type (TAO_InputCDR& stream) {
  CORBA::ULong length = 0;
  if (stream.read_ulong (length) == 0)
    return 1;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  CORBA::ULong orbtype;

  if (!(stream2 >> orbtype))
    return false;

  if (orbtype == TAO_ORB_TYPE)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I ORB Type: 0x%x (TAO)\n",
                  orbtype));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I ORB Type: 0x%x\n",
                  orbtype));
    }

  return true;
}


CORBA::Boolean
cat_tao_tag_endpoints (TAO_InputCDR& stream)
{
  CORBA::ULong length = 0;
  if (stream.read_ulong (length) == 0)
    return 1;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  TAO::IIOPEndpointSequence epseq;
  stream2 >> epseq;

  for (unsigned int iter=0; iter < epseq.length() ; iter++) {
    ACE_DEBUG ((LM_DEBUG,
                "%I Endpoint #%d:\n",iter+1));
    const char *host = epseq[iter].host;
    ACE_DEBUG ((LM_DEBUG,
                "%I Host: %s\n",host));
    CORBA::UShort port = epseq[iter].port;
    ACE_DEBUG ((LM_DEBUG,
                "%I Port: %d\n",port));
    ACE_DEBUG ((LM_DEBUG,
                "%I Priority: %d\n",epseq[iter].priority));
  }

  return 1;
}

CORBA::Boolean
cat_tag_alternate_endpoints (TAO_InputCDR& stream) {
  CORBA::ULong length = 0;
  if (stream.read_ulong (length) == 0)
    return 1;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  CORBA::String_var host;
  CORBA::UShort port;
  if ((stream2  >> host.out()) == 0 ||
      (stream2 >> port) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"cannot extract endpoint info\n"),0);
  ACE_DEBUG ((LM_DEBUG,
              "%I endpoint: %s:%d\n",host.in(),port));
  return 1;
}

CORBA::Boolean
cat_tag_policies (TAO_InputCDR& stream) {
  CORBA::ULong length = 0;
  if (stream.read_ulong (length) == 0)
    return 1;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  Messaging::PolicyValueSeq policies;
  stream2 >> policies;

  ACE_DEBUG ((LM_DEBUG,
              "%I Number of policies: %d\n",
              policies.length()));

  for (unsigned int iter=0; iter < policies.length() ; iter++) {
    // Create new stream for pvalue contents
    char *pmbuf = new char [policies[iter].pvalue.length()];

    for (unsigned int biter=0 ;
         biter < policies[iter].pvalue.length() - sizeof(int) ;
         biter++) {
      pmbuf[biter] = policies[iter].pvalue[biter + sizeof(int)];
    }

    int byteOrder = policies[iter].pvalue[0];
    TAO_InputCDR stream3 (pmbuf,
                          policies[iter].pvalue.length(),
                          static_cast<int> (byteOrder));

    if (policies[iter].ptype == RTCORBA::PRIORITY_MODEL_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (PRIORITY_MODEL_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));

      RTCORBA::PriorityModel priority_model;
      RTCORBA::Priority server_priority;

      stream3 >> priority_model;
      stream3 >> server_priority;

      if (priority_model == RTCORBA::CLIENT_PROPAGATED) {
        ACE_DEBUG ((LM_DEBUG,"%I\t Priority Model: %d (CLIENT_PROPAGATED)\n",
                    priority_model));
      } else if (priority_model == RTCORBA::SERVER_DECLARED) {
        ACE_DEBUG ((LM_DEBUG,"%I\t Priority Model: %d (SERVER_DECLARED)\n",
                    priority_model));
      } else {
        ACE_DEBUG ((LM_DEBUG,"%I\t Priority Model: %d (UNKNOWN!)\n",
                    priority_model));
      }
      ACE_DEBUG ((LM_DEBUG,
                  "%I\t Priority: %d\n",
                  server_priority));

    } else if (policies[iter].ptype == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (PRIORITY_BANDED_CONNECTION_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::REBIND_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REBIND_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
    } else if (policies[iter].ptype == Messaging::SYNC_SCOPE_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (SYNC_SCOPE_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
#endif
    } else if (policies[iter].ptype == Messaging::REQUEST_PRIORITY_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REQUEST_PRIORITY_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::REPLY_PRIORITY_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REPLY_PRIORITY_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::REQUEST_START_TIME_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REQUEST_START_TIME_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::REQUEST_END_TIME_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REQUEST_END_TIME_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::REPLY_START_TIME_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REPLY_START_TIME_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::REPLY_END_TIME_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (REPLY_END_TIME_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (RELATIVE_REQ_TIMEOUT_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (RELATIVE_RT_TIMEOUT_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::ROUTING_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (ROUTING_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::MAX_HOPS_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (MAX_HOPS_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else if (policies[iter].ptype == Messaging::QUEUE_ORDER_POLICY_TYPE) {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (QUEUE_ORDER_POLICY_TYPE)\n",
                  iter+1,
                  policies[iter].ptype));
    } else {
      ACE_DEBUG ((LM_DEBUG,
                  "%I Policy #%d Type: %d (UNKNOWN)\n", iter+1,
                  policies[iter].ptype));
    }

    delete [] pmbuf;
  }

  return 1;
}

CORBA::Boolean
cat_security_association (const CORBA::UShort& a) {
  // Copied from Security.idl
  typedef CORBA::UShort AssociationOptions;
  const AssociationOptions NoProtection = 1;
  const AssociationOptions Integrity = 2;
  const AssociationOptions Confidentiality = 4;
  const AssociationOptions DetectReplay = 8;
  const AssociationOptions DetectMisordering = 16;
  const AssociationOptions EstablishTrustInTarget = 32;
  const AssociationOptions EstablishTrustInClient = 64;
  const AssociationOptions NoDelegation = 128;
  const AssociationOptions SimpleDelegation = 256;
  const AssociationOptions CompositeDelegation = 512;

#define CHECK_PRINT(X) \
  if (a & X) ACE_DEBUG ((LM_DEBUG, "%I" #X "\n"))

  CHECK_PRINT(NoProtection);
  CHECK_PRINT(Integrity);
  CHECK_PRINT(Confidentiality);
  CHECK_PRINT(DetectReplay);
  CHECK_PRINT(DetectMisordering);
  CHECK_PRINT(EstablishTrustInTarget);
  CHECK_PRINT(EstablishTrustInClient);
  CHECK_PRINT(NoDelegation);
  CHECK_PRINT(SimpleDelegation);
  CHECK_PRINT(CompositeDelegation);

  return 0;
}

CORBA::Boolean
cat_tag_ssl_sec_trans (TAO_InputCDR& cdr) {
  /*
    Decode the following from the stream (copied from SSLIOP.idl)

      module Security {
        typedef unsigned short AssociationOptions;
      };
      ...
      module SSLIOP {
        struct SSL {
          Security::AssociationOptions    target_supports;
          Security::AssociationOptions    target_requires;
          unsigned short        port;
        };
      };

    We duplicate the code here because we do not want the catior
    utility to be dependent upon SSLIOP.
   */

  CORBA::ULong length = 0;
  if (cdr.read_ulong (length) == 0)
    return false;

  TAO_InputCDR stream (cdr, length);
  cdr.skip_bytes(length);

  CORBA::UShort target_supports;
  CORBA::UShort target_requires;
  CORBA::UShort port;

  if (stream.read_ushort(target_supports) == 0)
    return false;

  if (stream.read_ushort(target_requires) == 0)
    return false;

  if (stream.read_ushort(port) == 0)
    return false;

  ACE_DEBUG ((LM_DEBUG, "%Iport = %d\n", port));
  ACE_DEBUG ((LM_DEBUG, "%Itarget_supports = 0x%x\n", target_supports));
  ACE_DEBUG ((LM_DEBUG, "%{"));
  cat_security_association(target_supports);
  ACE_DEBUG ((LM_DEBUG, "%}"));
  ACE_DEBUG ((LM_DEBUG, "%Itarget_requires = 0x%x\n", target_requires));
  ACE_DEBUG ((LM_DEBUG, "%{"));
  cat_security_association(target_requires);
  ACE_DEBUG ((LM_DEBUG, "%}"));

  return true;
}

CORBA::Boolean
cat_octet_seq (const char *object_name,
               TAO_InputCDR& stream)
{
  CORBA::ULong length = 0;
  if (stream.read_ulong (length) == 0)
    return true;

  ACE_DEBUG ((LM_DEBUG,
              "%I %s len:\t%d\n",
              object_name,
              length));

  ACE_DEBUG ((LM_DEBUG,
              "%I %s as hex:\n",
              object_name));

  CORBA::Octet anOctet;
  CORBA::String_var objKey = CORBA::string_alloc (length + 1);

  short counter = -1;

  ACE_DEBUG ((LM_DEBUG, "%I "));
  CORBA::ULong i = 0;

  for (; i < length; ++i)
    {
      if (++counter == 16)
        {
          ACE_DEBUG ((LM_DEBUG, "\n%I "));
          counter = 0;
        }

      if (!stream.read_octet (anOctet))
        return false;

      ACE_DEBUG ((LM_DEBUG, "%02.2x ", anOctet));
      objKey[i] = (char) anOctet;
    }

  objKey[i] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              "\n%I The %s as string:\n%I ",
              object_name));

  for (i = 0; i < length; ++i)
    {
      char c = objKey[i];
      int tmp = (unsigned char) c; // isprint doesn't work with negative vals.(except EOF)
      if (ACE_OS::ace_isprint (static_cast<ACE_TCHAR> (tmp)))
        ACE_DEBUG ((LM_DEBUG, "%c", c));
      else
        ACE_DEBUG ((LM_DEBUG, "."));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  return true;
}

CORBA::Boolean
cat_object_key (TAO_InputCDR& stream)
{
  // ... and object key.

  return cat_octet_seq ("Object Key", stream);
}

ACE_CString
 _find_info (CORBA::ULong id)
{
  ACE_CString locale = "";
  ACE_Codeset_Registry::registry_to_locale (id, locale, 0, 0);
  return locale;
}

void displayHex (TAO_InputCDR & str)
{
  if (str.good_bit () == 0)
    return;

  TAO_InputCDR clone_str (str);
  CORBA::ULong theSetId ;
  if (!str.read_ulong (theSetId))
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to read codeset ID.\n"));
      return;
    }
  ACE_DEBUG ((LM_DEBUG," Hex - %x", theSetId));
  ACE_DEBUG ((LM_DEBUG,"\tDescription - "));
  ACE_CString theDescr = _find_info (theSetId);

  if (theDescr.length () == 0)
    ACE_DEBUG ((LM_DEBUG," Unknown CodeSet \n "));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" %s \n"), theDescr.c_str ()));
}

CORBA::Boolean
cat_codeset_info (TAO_InputCDR& cdr)
{
  CORBA::ULong length = 0;
  if (cdr.read_ulong (length) == 0)
    return false;

  TAO_InputCDR stream (cdr, length);
  cdr.skip_bytes(length);

  ACE_DEBUG ((LM_DEBUG, "\tComponent length: %u \n", length));

  ACE_DEBUG ((LM_DEBUG,
               "\tComponent byte order:\t%s Endian\n",
              (stream.byte_order () ? "Little" : "Big")));

  // CodesetId for char
  // CORBA::ULong c_ncsId;
  ACE_DEBUG ((LM_DEBUG, "\tNative CodeSet for char: "));
  displayHex (stream);

  // number of Conversion Codesets for char
  CORBA::ULong c_ccslen = 0;

  if (!(stream >> c_ccslen))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read number of conversion codesets "
                       "for char.\n"),
                      false);

  ACE_DEBUG ((LM_DEBUG, "\tNumber of CCS for char %u \n", c_ccslen));

  if (c_ccslen)
    ACE_DEBUG ((LM_DEBUG, "\tConversion Codesets for char are: \n"));

  //  Loop through and display them
  CORBA::ULong index = 0;
  for ( ; index < c_ccslen; ++index)
    {
      // CodesetId for char
      ACE_DEBUG ((LM_DEBUG, "\t%u) ", index + 1L));
      displayHex (stream);
    }

  // CodesetId for wchar
  ACE_DEBUG ((LM_DEBUG, "\tNative CodeSet for wchar: "));
  displayHex (stream);

  // number of Conversion Codesets for wchar
  CORBA::ULong w_ccslen=0;

  if (!(stream >> w_ccslen))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read number of conversion codesets "
                       "for wchar.\n"),
                      false);

  ACE_DEBUG ((LM_DEBUG, "\tNumber of CCS for wchar %u \n", w_ccslen));

  if (w_ccslen)
    ACE_DEBUG ((LM_DEBUG, "\tConversion Codesets for wchar are: \n"));

  //  Loop through and display them
  for (index = 0; index < w_ccslen; ++index)
    {
      ACE_DEBUG ((LM_DEBUG, "\t %u) ", index + 1L));
      displayHex (stream);
    }
  return true;
}

CORBA::Boolean
cat_tagged_components (TAO_InputCDR& stream)
{
  // ... and object key.

  CORBA::ULong len;
  if (!(stream >> len))
    return false;

  for (CORBA::ULong i = 0;
       i != len;
       ++i)
    {
      CORBA::ULong tag;
      if (!(stream >> tag))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read component tag.\n"),
                            false);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "%I The component <%d> ID is ", i+1, tag));

      if (tag == IOP::TAG_ORB_TYPE) {
        ACE_DEBUG ((LM_DEBUG,"%d (TAG_ORB_TYPE)\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{%{"));
        cat_tag_orb_type(stream);
        ACE_DEBUG ((LM_DEBUG, "%}%}"));

      } else if (tag == IOP::TAG_CODE_SETS) {
        ACE_DEBUG ((LM_DEBUG,"%d (TAG_CODE_SETS)\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{%{"));
        cat_codeset_info(stream);
        ACE_DEBUG ((LM_DEBUG, "%}%}"));
      } else if (tag == IOP::TAG_ALTERNATE_IIOP_ADDRESS) {
        ACE_DEBUG ((LM_DEBUG,"%d (TAG_ALTERNATE_IIOP_ADDRESS)\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{%{"));
        cat_tag_alternate_endpoints (stream);
        ACE_DEBUG ((LM_DEBUG, "%}%}"));
      } else if (tag == TAO_TAG_ENDPOINTS) {
        ACE_DEBUG ((LM_DEBUG,"%d (TAO_TAG_ENDPOINTS)\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{%{"));
        cat_tao_tag_endpoints(stream);
        ACE_DEBUG ((LM_DEBUG, "%}%}"));

      } else if (tag == IOP::TAG_POLICIES) {
        ACE_DEBUG ((LM_DEBUG,"%d (TAG_POLICIES)\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{%{"));
        cat_tag_policies(stream);
        ACE_DEBUG ((LM_DEBUG, "%}%}"));
      } else if (tag == 20U /* SSLIOP::TAG_SSL_SEC_TRANS */) {
        ACE_DEBUG ((LM_DEBUG,"%d (TAG_SSL_SEC_TRANS)\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{"));
        cat_tag_ssl_sec_trans(stream);
        ACE_DEBUG ((LM_DEBUG, "%}"));
      } else {
        ACE_DEBUG ((LM_DEBUG,"%d\n", tag));
        ACE_DEBUG ((LM_DEBUG, "%{%{"));
        cat_octet_seq ("Component Value", stream);
        ACE_DEBUG ((LM_DEBUG, "%}%}"));
      }
    }

  return 1;
}

CORBA::Boolean
cat_profile_helper (TAO_InputCDR& stream,
                    const char *protocol)
{
  // OK, we've got an IIOP profile.  It's going to be
  // encapsulated ProfileData.  Create a new decoding stream and
  // context for it, and tell the "parent" stream that this data
  // isn't part of it any more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read encap length\n"));
      return 0;
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return 0;

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major;
  CORBA::Octet iiop_version_minor = CORBA::Octet();
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 2))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I detected new v%d.%d %s profile that catior cannot decode\n",
                  iiop_version_major,
                  iiop_version_minor,
                  protocol));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%s Version:\t%d.%d\n",
              protocol,
              iiop_version_major,
              iiop_version_minor));

  // Get host and port.
  CORBA::UShort port_number;
  CORBA::String_var hostname;
  if (!(str >> hostname.inout ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I problem decoding hostname\n"));
      return 1;
    }

  if (!(str >> port_number))
    return false;

  ACE_DEBUG ((LM_DEBUG,
              "%I Host Name:\t%s\n",
              hostname.in ()));
  ACE_DEBUG ((LM_DEBUG,
              "%I Port Number:\t%d\n",
              port_number));

  if (cat_object_key (str) == 0)
    return 0;

  //IIOP 1.0 does not have tagged_components.
  if (!(iiop_version_major == 1 && iiop_version_minor == 0))
    {
      if (cat_tagged_components (str) == 0)
        return 0;

      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
cat_coiop_profile (TAO_InputCDR& stream)
{
  // OK, we've got an COIOP profile.  It's going to be
  // encapsulated ProfileData.  Create a new decoding stream and
  // context for it, and tell the "parent" stream that this data
  // isn't part of it any more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read encap length\n"));
      return false;
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return false;

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major;
  CORBA::Octet iiop_version_minor = CORBA::Octet();
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 2))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I detected new v%d.%d COIOP profile that catior cannot decode",
                  iiop_version_major,
                  iiop_version_minor));
      return true;
    }

  ACE_DEBUG ((LM_DEBUG,
              "COIOP Version:\t%d.%d\n",
              iiop_version_major,
              iiop_version_minor));

  // Get host and port.
  CORBA::String_var uuid;
  if (!(str >> uuid.inout ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I problem decoding uuid\n"));
      return true;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%I UUID:\t%s\n",
              uuid.in ()));

  if (cat_object_key (str) == 0)
    return false;

  //IIOP 1.0 does not have tagged_components.
  if (!(iiop_version_major == 1 && iiop_version_minor == 0))
    {
      if (cat_tagged_components (str) == 0)
        return false;

      return true;
    }
  else
    return false;
}

CORBA::Boolean
cat_iiop_profile (TAO_InputCDR& stream)
{
  return cat_profile_helper (stream, "IIOP");
}

CORBA::Boolean
cat_shmiop_profile (TAO_InputCDR& stream)
{
  return cat_profile_helper (stream, "SHMIOP");
}

CORBA::Boolean
cat_uiop_profile (TAO_InputCDR& stream)
{
  // OK, we've got a UIOP profile.  It's going to be encapsulated
  // ProfileData.  Create a new decoding stream and context for it,
  // and tell the "parent" stream that this data isn't part of it any
  // more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    return 0;

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return 0;

  // Read and verify major, minor versions, ignoring UIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet uiop_version_major;
  CORBA::Octet uiop_version_minor = CORBA::Octet();
  // It appears that as of April 2002 UIOP version is 1.2
  if (! (str.read_octet (uiop_version_major)
         && uiop_version_major == 1
         && str.read_octet (uiop_version_minor)
         && uiop_version_minor <= 2))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I detected new v%d.%d UIOP profile",
                  uiop_version_major,
                  uiop_version_minor));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%I UIOP Version:\t%d.%d\n",
              uiop_version_major,
              uiop_version_minor));

  // Get host and port.
  CORBA::String_var rendezvous;
  if ((str >> rendezvous.out ()) == 0)
    return 0;

  ACE_DEBUG ((LM_DEBUG,
              "%I Rendezvous point:\t%s\n",
              rendezvous.in ()));

  if (cat_object_key (str) == 0)
    return 0;

  if (cat_tagged_components (str) == 0)
    return 0;

  return 1;
}

CORBA::Boolean
cat_sciop_profile (TAO_InputCDR& stream)
{
  // OK, we've got an SCIOP profile.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read encap length\n"));
      return 0;
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return 0;

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major;
  CORBA::Octet iiop_version_minor = CORBA::Octet();
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 0))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I detected new v%d.%d SCIOP profile that catior cannot decode",
                  iiop_version_major,
                  iiop_version_minor));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "SCIOP Version:\t%d.%d\n",
              iiop_version_major,
              iiop_version_minor));

  // Get host and port.
  CORBA::UShort port_number;
  CORBA::UShort max_streams;
  CORBA::ULong addresses;

  if (!(str >> addresses))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to decode number of addresses\n."),
                      false);

  ACE_DEBUG ((LM_DEBUG,
              "%I Addresses:\t%d\n",
              addresses));

  for (CORBA::ULong i = 0; i < addresses; ++i)
    {
      CORBA::String_var hostname;
      if (!(str >> hostname.inout ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%I problem decoding hostname\n"),
                            false);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "%I Host Name:\t%s\n",
                  hostname.in ()));
    }


  if (!(str >> port_number))
    return false;

  ACE_DEBUG ((LM_DEBUG,
              "%I Port Number:\t%d\n",
              port_number));

  if (!(str >> max_streams))
    return false;

  ACE_DEBUG ((LM_DEBUG,
              "%I Max Streams:\t%d\n",
              max_streams));

  if (cat_object_key (str) == 0
      || // Unlike IIOP (1.0), SCIOP always has tagged_components.
      cat_tagged_components (str) == 0)
    return false;

  return true;
}


CORBA::Boolean
cat_nsk_profile_helper (TAO_InputCDR& stream,
                        const char *protocol)
{
  // OK, we've got an NSK profile.  It's going to be
  // encapsulated ProfileData.  Create a new decoding stream and
  // context for it, and tell the "parent" stream that this data
  // isn't part of it any more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read encap length\n"));
      return 0;
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return 0;

  // Read and verify major, minor versions, ignoring NSK
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major;
  CORBA::Octet iiop_version_minor = CORBA::Octet();
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 2))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I detected new v%d.%d %s profile that catior cannot decode",
                  iiop_version_major,
                  iiop_version_minor,
                  protocol));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "%s Version:\t%d.%d\n",
              protocol,
              iiop_version_major,
              iiop_version_minor));

  // Get address
  char* fsaddress;
  if ((str >> fsaddress) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%I problem decoding file system address\n"));
      return 1;
    }


  ACE_DEBUG ((LM_DEBUG,
              "%I FS Address:\t%s\n",
              fsaddress));
  CORBA::string_free (fsaddress);

  if (cat_object_key (str) == 0)
    return 0;

  // Version 1.0 does not have tagged_components.
  if (!(iiop_version_major == 1 && iiop_version_minor == 0))
    {
      if (cat_tagged_components (str) == 0)
        return 0;

      return 1;
    }
  else
    return 0;
}

CORBA::Boolean
cat_nskpw_profile (TAO_InputCDR& stream)
{
  return cat_nsk_profile_helper (stream, "NSKPW");
}

CORBA::Boolean
cat_nskfs_profile (TAO_InputCDR& stream)
{
  return cat_nsk_profile_helper (stream, "NSKFS");
}

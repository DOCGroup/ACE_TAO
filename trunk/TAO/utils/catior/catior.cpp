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
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "tao/corba.h"
#include "tao/IIOP_Profile.h"

static CORBA::Boolean
catiiop (CORBA::String string,
         CORBA::Environment &env)
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
  if (isdigit (string [0])
      && isdigit (string [2])
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
  CORBA::String hostname;
  char *cp = ACE_OS::strchr (string, ':');

  if (cp == 0)
    {
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  hostname = CORBA::string_alloc (1 + cp - string);

  for (cp = hostname;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

  cp = ACE_OS::strchr ((char *) string, '/');

  if (cp == 0)
    {
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      CORBA::string_free (hostname);
      return 0;
    }

  port_number = (short) ACE_OS::atoi ((char *) string);
  string = ++cp;

  ACE_DEBUG ((LM_DEBUG,
              "Host Name:\t%s\n",
              hostname));
  ACE_DEBUG ((LM_DEBUG,
              "Port Number:\t%d\n",
              port_number));
  CORBA::string_free (hostname);

  // Parse the object key.
  // dump the object key to stdout
  //  TAO_POA::decode_string_to_sequence (data->profile.object_key,
  //                                      string);
  ACE_DEBUG ((LM_DEBUG,
              "\nThe Object Key as string:\n%s\n",
              string));
  return 1;
}

static CORBA::Boolean
catior (CORBA::String str,
        CORBA::Environment &env)
{
  // Unhex the bytes, and make a CDR deencapsulation stream from the
  // resulting data.

  ACE_Message_Block mb (ACE_OS::strlen ((char *) str)  / 2 + 1
                        + ACE_CDR::MAX_ALIGNMENT);
  ACE_CDR::mb_align (&mb);

  char *buffer = mb.rd_ptr ();
  char *tmp = (char *) str;
  size_t len = 0;

  CORBA::Boolean continue_decoding;

  // The prefix of the IOR must be removed, and the string must start
  // with the encapsulation byte

  while (tmp [0] && tmp [1])
    {
      u_char byte;

      if (! (isxdigit (tmp [0]) && isxdigit (tmp [1])))
        break;

      byte = (u_char) (ACE::hex2byte (tmp [0]) << 4);
      byte |= ACE::hex2byte (tmp [1]);

      buffer [len++] = byte;
      tmp += 2;
    }

  // Create deencapsulation stream ... then unmarshal objref from that
  // stream.

  CORBA::Boolean byteOrder;

  {
    TAO_InputCDR encapStream (&mb);
    continue_decoding = encapStream.read_boolean (byteOrder);
  }

  mb.rd_ptr (1);
  mb.wr_ptr (2 * len - 1);
  TAO_InputCDR stream (&mb, ACE_static_cast(int,byteOrder));

  if (byteOrder == 1)
    ACE_DEBUG ((LM_DEBUG,
                "The Byte Order:\tLittle Endian\n"));
  else
    ACE_DEBUG ((LM_DEBUG,
                "The Byte Order:\tBig Endian\n"));

  // First, read the type hint. This will be the type_id encoded in an
  // object reference.
  CORBA::String type_hint;

  continue_decoding = stream.decode (CORBA::_tc_string,
                                     &type_hint,
                                     0,
                                     env);
  if (continue_decoding == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "cannot read type id\n"));
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  ACE_DEBUG ((LM_DEBUG,
              "The Type Id:\t\"%s\"\n",
              type_hint));

  // Release any memory associated with the type_hint.
  CORBA::string_free (type_hint);

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
      return CORBA::TypeCode::TRAVERSE_STOP;
    }

  CORBA::ULong profile_counter = 0;

  // No profiles means a NIL objref.
  if (profiles == 0)
    return CORBA_TypeCode::TRAVERSE_CONTINUE;
  else
    while (profiles-- != 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Profile Count:\t%d\n",
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

        if (tag != TAO_IOP_TAG_INTERNET_IOP)
          {
            continue_decoding = stream.skip_string ();
            ACE_DEBUG ((LM_DEBUG,
                        "unknown tag %d skipping\n", tag));
            continue;
          }

        // OK, we've got an IIOP profile.  It's going to be
        // encapsulated ProfileData.  Create a new decoding stream and
        // context for it, and tell the "parent" stream that this data
        // isn't part of it any more.

        CORBA::ULong encap_len;
        continue_decoding = stream.read_ulong (encap_len);

        // ProfileData is encoded as a sequence of octet. So first get
        // the length of the sequence.
        if (continue_decoding == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "cannot read encap length\n"));
            continue;
          }

        // Create the decoding stream from the encapsulation in the
        // buffer, and skip the encapsulation.
        TAO_InputCDR str (stream, encap_len);

        continue_decoding = str.good_bit ()
          && stream.skip_bytes (encap_len);

        if (!continue_decoding)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "problem decoding encapsulated stream, "
                        "len = %d\n",
                        encap_len));
            continue;
          }

        // Read and verify major, minor versions, ignoring IIOP
        // profiles whose versions we don't understand.
        //
        // XXX this doesn't actually go back and skip the whole
        // encapsulation...
        CORBA::Octet iiop_version_major, iiop_version_minor;
        if (! (str.read_octet (iiop_version_major)
              && iiop_version_major == TAO_IIOP_Profile::DEF_IIOP_MAJOR
              && str.read_octet (iiop_version_minor)
              && iiop_version_minor <= TAO_IIOP_Profile::DEF_IIOP_MINOR))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "detected new v%d.%d IIOP profile",
                        iiop_version_major,
                        iiop_version_minor));
            continue;
          }

        ACE_DEBUG ((LM_DEBUG,
                    "IIOP Version:\t%d.%d\n",
                    iiop_version_major,
                    iiop_version_minor));

        // Get host and port.
        CORBA::UShort port_number;
        CORBA::String hostname;
        if (str.decode (CORBA::_tc_string,
                        &hostname,
                        0,
                        env) != CORBA::TypeCode::TRAVERSE_CONTINUE
            || !str.read_ushort (port_number))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "error decoding IIOP host/port"));
            return CORBA::TypeCode::TRAVERSE_STOP;
          }

        ACE_DEBUG ((LM_DEBUG,
                    "Host Name:\t%s\n",
                    hostname));
        ACE_DEBUG ((LM_DEBUG,
                    "Port Number:\t%d\n",
                    port_number));
        CORBA::string_free (hostname);

        // ... and object key.

        CORBA::ULong objKeyLength = 0;
        continue_decoding = str.read_ulong (objKeyLength);

        ACE_DEBUG ((LM_DEBUG,
                    "Object Key len:\t%d\n",
                    objKeyLength));

        ACE_DEBUG ((LM_DEBUG,
                    "Object Key as hex:\n"));

        CORBA::Octet anOctet;
        CORBA::String objKey = CORBA::string_alloc (objKeyLength + 1);

        short counter = -1;

        u_int i = 0;

        for (; i < objKeyLength; i++)
          {
            if (++counter == 8)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "\n"));
                counter = 0;
              }
            str.read_octet (anOctet);

            ACE_DEBUG ((LM_DEBUG,
                        "%x ",
                        anOctet));
            objKey[i] = (char) anOctet;
          }

        objKey[i] = '\0';

        ACE_DEBUG ((LM_DEBUG,
                    "\nThe Object Key as string:\n"));

        for (i = 0; i < objKeyLength; i++)
          ACE_DEBUG ((LM_DEBUG,
                      "%c",
                      objKey[i]));

        CORBA::string_free (objKey);
        ACE_DEBUG ((LM_DEBUG,
                    "\n"));
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

static CORBA::Boolean
catpoop (CORBA::String string,
        CORBA::Environment &env)
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
      env.exception (new CORBA_DATA_CONVERSION (CORBA::COMPLETED_NO));
      return 0;
    }

  // Read the hostname.
  CORBA::String hostname = CORBA::string_alloc (1 + cp - string);

  for (cp = hostname;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

  ACE_DEBUG ((LM_DEBUG,
              "Host Name:\t%s\n",
              hostname));
  CORBA::string_free (hostname);

  //  read the server name
  cp = ACE_OS::strchr (string, ':');

  CORBA::String server_name = CORBA::string_alloc (1 + cp - string);

  for (cp = server_name;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

  ACE_DEBUG ((LM_DEBUG,
              "Server Name:\t%s\n",
              server_name));

  CORBA::string_free (server_name);

  // Read the Orbix specific marker.
  cp = ACE_OS::strchr (string, ':');

  CORBA::String marker = CORBA::string_alloc (1 + cp - string);

  for (cp = marker;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

  ACE_DEBUG ((LM_DEBUG,
              "Marker:\t\t%s\n",
              marker));
  CORBA::string_free (marker);

  cp = ACE_OS::strchr (string, ':');

  // Read the IR_host.
  CORBA::String IR_host = CORBA::string_alloc (1 + cp - string);

  for (cp = IR_host;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

  ACE_DEBUG ((LM_DEBUG,
              "IR Host:\t\t%s\n",
              IR_host));
  CORBA::string_free (IR_host);

  // Read the IR_server
  cp = ACE_OS::strchr (string, ':');

  CORBA::String IR_server = CORBA::string_alloc (1 + cp - string);

  for (cp = IR_server;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  string++;

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
main (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "f:n:");

  CORBA::Environment env;
  CORBA::ORB_var orb_var =  CORBA::ORB_init (argc, argv, "TAO", env);
  CORBA::Boolean b;
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          //  Read the CosName from the NamingService convert the
          //  object_ptr to a CORBA::String_var via the call to
          //  object_to_string.
          ACE_DEBUG ((LM_DEBUG,
                      "opening a connection to the NamingService\n"
                      "resolving the CosName %s\n",
                      get_opt.optarg));
          break;
        case 'f':
          {
            //  Read the file into a CORBA::String_var.
            ACE_DEBUG ((LM_DEBUG,
                        "reading the file %s\n",
                        get_opt.optarg));

            ifstream ifstr (get_opt.optarg);

            if (!ifstr.good ())
              {
                ifstr.close ();
                return -1;
              }

            char ch;
            ACE_CString aString;

            while (!ifstr.eof ())
              {
                ifstr.get (ch);
                if (ch == '\n' || ch == EOF)
                  break;
                aString += ch;
              }

            ACE_DEBUG ((LM_DEBUG,
                        "\nhere is the IOR\n%s\n\n",
                        aString.rep ()));

            CORBA::String str;
            if (aString.find ("IOR:") == 0)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "decoding an IOR:\n"));

                // Strip the IOR: off the string.
                ACE_CString prefix = "IOR:";
                short prefixLength = prefix.length ();

                ACE_CString subString =
                  aString.substring (prefixLength,
                                     aString.length () - prefixLength);
                subString[subString.length ()] = '\0';
                str = subString.rep ();
                b = catior (str, env);
              }
            else if (aString.find ("iiop:") == 0)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "decoding an IIOP URL IOR\n"));

                ACE_CString prefix = "IIOP:";
                short prefixLength = prefix.length ();

                ACE_CString subString =
                  aString.substring (prefixLength,
                                     aString.length () - prefixLength);
                //subString[subString.length () - 1] = '\0';
                str = subString.rep ();
                b = catiiop (str, env);
              }
            else if (aString.find (":IR:"))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "decoding an POOP IOR\n"));

                str = aString.rep ();
                b = catpoop (str, env);
              }
            else
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 "Don't know how to decode this IOR\n"),
                                -1);
            if (b == 1)
              ACE_DEBUG ((LM_DEBUG,
                          "catior returned true\n"));
            else
              ACE_DEBUG ((LM_DEBUG,
                          "catior returned false\n"));
            ifstr.close ();
          }
        break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: %s "
                             "-f filename "
                             "-n CosName "
                             "\n"
                             "Reads an IOR "
                             "and dumps the contents to stdout "
                             "\n",
                             argv[0]),
                            1);
        }
    }

  return 0;
}

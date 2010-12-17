// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/Utils/catior
//
// = FILENAME
//    Catior_i.cpp
//
// = DESCRIPTION
//    Reads stringified IORs and outputs the encoded information.
//
// = AUTHORS
//      Jeff Hopper <jrhopper@cts.com>
//      SCIOP and Tagged component modifications by:
//      Jason Cohen, Lockheed Martin ATL <jcohen@atl.lmco.com>
//      Split into a separate library by:
//      Chad Elliott <elliott_c@ociweb.com>
//
// ============================================================================

#include "Catior_i.h"
#include "tao/Messaging_PolicyValueC.h"
#include "tao/Messaging/Messaging_TypesC.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/Compression.h"
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

Catior_i::Catior_i()
 : trace_depth_(0),
   buffer_()
{
}


CORBA::Boolean
Catior_i::decode (const ACE_CString& aString, ACE_CString& output)
{
  const ACE_CString iorPrefix = "IOR:";
  const ACE_CString iiopPrefix = "iiop:";
  const ACE_CString poopPrefix = ":IR:";

  char * str = 0;
  CORBA::Boolean b = false;
  if (aString.find (iorPrefix) == 0)
    {
      buffer_ += "Decoding an IOR:\n";

      // Strip the IOR: off the string.
      size_t prefixLength = iorPrefix.length ();
      ACE_CString subString =
        aString.substring (prefixLength,
                           aString.length () - prefixLength);
      subString[subString.length ()] = '\0';
      str = subString.rep ();
      b = catior (str);
    }
  else if (aString.find (iiopPrefix) == 0)
    {
      buffer_ += "Decoding an IIOP URL IOR\n";

      size_t prefixLength = iiopPrefix.length ();
      ACE_CString subString =
        aString.substring (prefixLength,
                           aString.length () - prefixLength);
      str = subString.rep ();
      b = catiiop (str);
    }
  else if (aString.find (poopPrefix) != ACE_CString::npos)
    {
      buffer_ += "Decoding a POOP IOR\n";

      str = aString.rep ();
      b = catpoop (str);
    }
  else
    {
      buffer_ += "Don't know how to decode this IOR\n";
    }

  delete [] str;
  output = buffer_;
  return b;
}



CORBA::Boolean
Catior_i::catiiop (char* string)
{
  // NIL objref encodes as just "iiop:" ... which has already been
  // removed, so we see it as an empty string.

  if (!string || !*string)
    return false;

  // Type ID not encoded in this string ... makes narrowing rather
  // expensive, though it does ensure that type-safe narrowing code
  // gets thoroughly excercised/debugged!  Without a typeID, the
  // _narrow will be required to make an expensive remote "is_a" call.

  // Remove the "N.N//" prefix, and verify the version's one that we
  // accept

  CORBA::Short iiop_version_major = 1;
  CORBA::Short iiop_version_minor = 0;

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
      string += 2;
    }

  static const size_t bufsize = 512;
  char buf[bufsize];
  ACE_OS::snprintf (buf, bufsize, "IIOP Version:\t%d.%d\n",
                    iiop_version_major, iiop_version_minor);
  buffer_ += buf;

  // Pull off the "hostname:port/" part of the objref Get host and
  // port.
  CORBA::UShort port_number;
  char *cp = ACE_OS::strchr (string, ':');

  if (cp == 0)
    {
      throw CORBA::DATA_CONVERSION ();
    }

  CORBA::String_var hostname = CORBA::string_alloc (1 +
    ACE_Utils::truncate_cast<CORBA::ULong> (cp - string));

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

  ACE_OS::snprintf (buf, bufsize, "Host Name:\t%s\n"
                                  "Port Number:\t%d\n",
                                  hostname.in (), port_number);
  buffer_ += buf;

  // Parse the object key.
  // dump the object key to stdout
  //  TAO_POA::decode_string_to_sequence (data->profile.object_key,
  //                                      string);
  buffer_ += "\nThe Object Key as string:\n";
  buffer_ += string;
  buffer_ += "\n";

  return true;
}

CORBA::Boolean
Catior_i::catior (char const * str)
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

  buffer_ += "The Byte Order:\t";
  if (byteOrder == 1)
    buffer_ += "Little Endian\n";
  else
    buffer_ += "Big Endian\n";

  // First, read the type hint. This will be the type_id encoded in an
  // object reference.
  CORBA::String_var type_hint;

  if (!(stream >> type_hint.inout ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "cannot read type id\n"),
                        TAO::TRAVERSE_STOP);
    }

  buffer_ += "The Type Id:\t\"";
  buffer_ += type_hint.in ();
  buffer_ += "\"\n";

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
  if (!continue_decoding)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "cannot read the profile count\n"),
                         TAO::TRAVERSE_STOP);
    }

  CORBA::ULong profile_counter = 0;

  static const size_t bufsize = 512;
  char buf[bufsize];
  ACE_OS::snprintf (buf, bufsize,
                    "Number of Profiles in IOR:\t%d\n", profiles);
  buffer_ += buf;

  // No profiles means a NIL objref.
  if (profiles == 0)
    return TAO::TRAVERSE_CONTINUE;
  else
    while (profiles-- != 0)
      {
        ACE_OS::snprintf (buf, bufsize, "Profile number:\t%d\n",
                                        ++profile_counter);
        buffer_ += buf;

        // We keep decoding until we find a valid IIOP profile.
        CORBA::ULong tag;

        continue_decoding = stream.read_ulong (tag);

        // Get the profile ID tag.
        if (!continue_decoding)
          {
            ACE_ERROR ((LM_ERROR, "cannot read profile tag\n"));
            continue;
          }

        trace_depth_++;
        if (tag == IOP::TAG_INTERNET_IOP)
          {
            continue_decoding = cat_iiop_profile (stream);
          }
        else if (tag == TAO_TAG_SCIOP_PROFILE)
          {
            continue_decoding = cat_sciop_profile (stream);
          }
        else if (tag == TAO_TAG_UIOP_PROFILE)
          {
            continue_decoding = cat_uiop_profile (stream);
          }
        else if (tag == TAO_TAG_SHMEM_PROFILE)
          {
            continue_decoding = cat_shmiop_profile (stream);
          }
        else if (tag == TAO_TAG_DIOP_PROFILE)
          {
            continue_decoding =  cat_profile_helper(stream, "DIOP (GIOP over UDP)");
          }
        else if (tag == TAO_TAG_COIOP_PROFILE)
          {
            continue_decoding =  cat_coiop_profile(stream);
          }
        else if (tag == TAO_TAG_NSKPW_PROFILE)
          {
            continue_decoding = cat_nskpw_profile (stream);
          }
        else if (tag == TAO_TAG_NSKFS_PROFILE)
          {
            continue_decoding = cat_nskfs_profile (stream);
          }
        else
          {
            indent ();
            ACE_OS::snprintf (buf, bufsize,
                        "Profile tag = %d (unknown protocol)\n", tag);
            buffer_ += buf;
            continue_decoding = cat_octet_seq("Profile body", stream);
          }
        trace_depth_--;
      }
  return true;
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
Catior_i::catpoop (char* string)
{
  if (!string || !*string)
    return false;

  //if (string [0] == ':'
  //    && string [1] == '\\')
  //  ACE_DEBUG ((LM_DEBUG,
  //              "\nPerhaps we've found some POOP\n"));
  string += 2;

  char *cp = ACE_OS::strchr (string, ':');

  if (cp == 0)
    {
      throw CORBA::DATA_CONVERSION ();
    }

  // Read the hostname.
  char* hostname = CORBA::string_alloc (1 +
    ACE_Utils::truncate_cast<CORBA::ULong> (cp - string));

  for (cp = hostname;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  buffer_ += "Host Name:\t";
  buffer_ += hostname;
  buffer_ += "\n";
  CORBA::string_free (hostname);

  //  read the server name
  cp = ACE_OS::strchr (string, ':');

  char* server_name = CORBA::string_alloc (1 +
    ACE_Utils::truncate_cast<CORBA::ULong> (cp - string));

  for (cp = server_name;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  buffer_ += "Server Name:\t";
  buffer_ += server_name;
  buffer_ += "\n",

  CORBA::string_free (server_name);

  // Read the Orbix specific marker.
  cp = ACE_OS::strchr (string, ':');

  char* marker = CORBA::string_alloc (1 +
    ACE_Utils::truncate_cast<CORBA::ULong> (cp - string));

  for (cp = marker;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  buffer_ += "Marker:\t\t";
  buffer_ += marker;
  buffer_ += "\n";
  CORBA::string_free (marker);

  cp = ACE_OS::strchr (string, ':');

  // Read the IR_host.
  char* IR_host = CORBA::string_alloc (1 +
    ACE_Utils::truncate_cast<CORBA::ULong> (cp - string));

  for (cp = IR_host;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  buffer_ += "IR Host:\t\t";
  buffer_ += IR_host;
  buffer_ += "\n";
  CORBA::string_free (IR_host);

  // Read the IR_server
  cp = ACE_OS::strchr (string, ':');

  char* IR_server = CORBA::string_alloc (1 +
    ACE_Utils::truncate_cast<CORBA::ULong> (cp - string));

  for (cp = IR_server;
       *string != ':';
       *cp++ = *string++)
    continue;

  *cp = 0;
  ++string;

  buffer_ += "IR Server:\t\t";
  buffer_ += IR_server;
  buffer_ += "\n";
  CORBA::string_free (IR_server);

  // Read the interface_marker
  buffer_ += "Interface Marker:\t";
  buffer_ += string;
  buffer_ += "\n";
  return true;
}

CORBA::Boolean
Catior_i::cat_tag_orb_type (TAO_InputCDR& stream) {
  CORBA::ULong length = 0;
  if (!stream.read_ulong (length))
    return true;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  CORBA::ULong orbtype;

  if (!(stream2 >> orbtype))
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

  indent();

  switch (orbtype)
  {
    case TAO_ORB_TYPE:
     {
       ACE_OS::snprintf (buf, bufsize,
                         "ORB Type: 0x%x (TAO)\n", orbtype);
       break;
     }
   case 0x29A:
     {
       ACE_OS::snprintf (buf, bufsize,
                         "ORB Type: 0x%x (TIDorbC++)\n", orbtype);
       break;
     }
   default:
     {
       ACE_OS::snprintf (buf, bufsize,
                         "ORB Type: 0x%x\n", orbtype);
     }
     break;
  }
  buffer_ += buf;
  return true;
}

CORBA::Boolean
Catior_i::cat_ibm_partner_version (TAO_InputCDR& stream) {
  /*
   * IBM Partner version looks like:
   * 49424d0a 00000008 00000000 1400 0005
   * The three initial bytes (from left to right) are the ASCII code for IBM,
   * followed by 0x0A, which specifies that the following bytes handle the
   * partner version.
   * The next four bytes encode the length of the remaining data (in this
   * case 8 bytes)
   * The next four null bytes are for future use.
   * The two bytes for the Partner Version Major field (0x1400) define the
   * release of the ORB that is being used (1.4.0 in this case).
   * The Minor field (0x0005) distinguishes in the same release, service
   * refreshes that contain changes that have affected the backward
   * compatibility
   */
  CORBA::ULong length = 0;
  if (!(stream.read_ulong (length)))
    return true;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  CORBA::ULong version;
  if (!(stream2 >> version))
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];
  ACE_OS::snprintf (buf, bufsize, "\tPartner Version: 0x%x\n", version);
  buffer_ += buf;

  return true;
}


CORBA::Boolean
Catior_i::cat_tao_tag_endpoints (TAO_InputCDR& stream)
{
  CORBA::ULong length = 0;
  if (!stream.read_ulong (length))
    return true;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  TAO::IIOPEndpointSequence epseq;
  if (!(stream2 >> epseq))
    return true;

  static const size_t bufsize = 512;
  char buf[bufsize];
  for (unsigned int iter=0; iter < epseq.length() ; iter++) {
    const char *host = epseq[iter].host;
    CORBA::UShort port = epseq[iter].port;
    indent ();
    ACE_OS::snprintf (buf, bufsize,
                      "Endpoint #%d:\n", iter+1);
    buffer_ += buf;
    indent ();
    ACE_OS::snprintf (buf, bufsize,
                      "Host: %s\n", host);
    buffer_ += buf;
    indent ();
    ACE_OS::snprintf (buf, bufsize,
                      "Port: %d\n", port);
    buffer_ += buf;
    indent ();
    ACE_OS::snprintf (buf, bufsize,
                      "Priority: %d\n", epseq[iter].priority);
    buffer_ += buf;
  }

  return true;
}

CORBA::Boolean
Catior_i::cat_tag_alternate_endpoints (TAO_InputCDR& stream) {
  CORBA::ULong length = 0;
  if (!stream.read_ulong (length))
    return true;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  CORBA::String_var host;
  CORBA::UShort port;
  if (!(stream2  >> host.out()) ||
      !(stream2 >> port))
    {
      ACE_ERROR_RETURN ((LM_ERROR,"cannot extract endpoint info\n"),false);
    }
  else
    {
      static const size_t bufsize = 512;
      char buf[bufsize];
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "endpoint: %s:%d\n", host.in(), port);
      buffer_ += buf;
    }

  return true;
}

CORBA::Boolean
Catior_i::cat_tag_policies (TAO_InputCDR& stream) {
  CORBA::ULong length = 0;
  if (!stream.read_ulong (length))
    return true;

  TAO_InputCDR stream2 (stream, length);
  stream.skip_bytes(length);

  Messaging::PolicyValueSeq policies;
  if (!(stream2 >> policies))
    return true;

  static const size_t bufsize = 512;
  char buf[bufsize];
  indent ();
  ACE_OS::snprintf (buf, bufsize,
                   "Number of policies: %d\n",
                   policies.length());
  buffer_ += buf;

  for (unsigned int iter=0; iter < policies.length() ; iter++) {
    // Create new stream for pvalue contents
    const CORBA::Octet *pmbuf = policies[iter].pvalue.get_buffer ();

    TAO_InputCDR stream3 (
      reinterpret_cast <const char*>  (pmbuf),
      policies[iter].pvalue.length ());

    CORBA::Boolean byte_order;
    if (!(stream3 >> ACE_InputCDR::to_boolean (byte_order)))
      return true;

    stream3.reset_byte_order (static_cast <int> (byte_order));

    if (policies[iter].ptype == RTCORBA::PRIORITY_MODEL_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (PRIORITY_MODEL_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;

      RTCORBA::PriorityModel priority_model;
      RTCORBA::Priority server_priority;

      if (!(stream3 >> priority_model))
        return true;
      if (!(stream3 >> server_priority))
        return true;

      indent ();
      if (priority_model == RTCORBA::CLIENT_PROPAGATED) {
        ACE_OS::snprintf (buf, bufsize,
                         "\t Priority Model: %d (CLIENT_PROPAGATED)\n",
                         priority_model);
      } else if (priority_model == RTCORBA::SERVER_DECLARED) {
        ACE_OS::snprintf (buf, bufsize,
                          "\t Priority Model: %d (SERVER_DECLARED)\n",
                          priority_model);
      } else {
        ACE_OS::snprintf (buf, bufsize,
                          "\t Priority Model: %d (UNKNOWN!)\n",
                          priority_model);
      }
      buffer_ += buf;
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                       "\t Priority: %d\n", server_priority);
      buffer_ += buf;

    } else if (policies[iter].ptype == RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (PRIORITY_BANDED_CONNECTION_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::REBIND_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                  "Policy #%d Type: %d (REBIND_POLICY_TYPE)\n",
                  iter+1, policies[iter].ptype);
      buffer_ += buf;
#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)
    } else if (policies[iter].ptype == Messaging::SYNC_SCOPE_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (SYNC_SCOPE_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
#endif
    } else if (policies[iter].ptype == Messaging::REQUEST_PRIORITY_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (REQUEST_PRIORITY_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::REPLY_PRIORITY_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (REPLY_PRIORITY_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::REQUEST_START_TIME_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (REQUEST_START_TIME_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::REQUEST_END_TIME_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (REQUEST_END_TIME_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::REPLY_START_TIME_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (REPLY_START_TIME_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::REPLY_END_TIME_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (REPLY_END_TIME_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (RELATIVE_REQ_TIMEOUT_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (RELATIVE_RT_TIMEOUT_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::ROUTING_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (ROUTING_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::MAX_HOPS_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (MAX_HOPS_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == Messaging::QUEUE_ORDER_POLICY_TYPE) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (QUEUE_ORDER_POLICY_TYPE)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
    } else if (policies[iter].ptype == ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (COMPRESSOR_ID_LEVEL_LIST_POLICY_ID)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
      ::Compression::CompressorIdLevelList idlist;
      if (!(stream3 >> idlist))
        return true;
      CORBA::ULong index = 0;
      for (; index < idlist.length(); index++)
        {
          indent ();
          ACE_OS::snprintf (buf, bufsize,
                            "\t CompressorId: %d Level: %d\n",
                            idlist[index].compressor_id, idlist[index].compression_level);
          buffer_ += buf;
        }
    } else if (policies[iter].ptype == ZIOP::COMPRESSION_ENABLING_POLICY_ID) {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (COMPRESSION_ENABLING_POLICY_ID)\n",
                        iter+1, policies[iter].ptype);
      buffer_ += buf;
      CORBA::Boolean status;
      if (!(stream3 >> ACE_InputCDR::to_boolean (status)))
        return true;
      indent ();
      ACE_OS::snprintf (buf, bufsize, "\t Enabled: %d\n", status);
      buffer_ += buf;
    } else {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "Policy #%d Type: %d (UNKNOWN)\n", iter+1,
                        policies[iter].ptype);
      buffer_ += buf;
    }
  }

  return true;
}

CORBA::Boolean
Catior_i::cat_security_association (const CORBA::UShort& a) {
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
  if (a & X) { indent (); buffer_ += "" #X "\n"; }

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

  return false;
}

CORBA::Boolean
Catior_i::cat_tag_ssl_sec_trans (TAO_InputCDR& cdr) {
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
  if (!cdr.read_ulong (length))
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

  static const size_t bufsize = 512;
  char buf[bufsize];

  indent ();
  ACE_OS::snprintf (buf, bufsize, "port = %d\n", port);
  buffer_ += buf;
  indent ();
  ACE_OS::snprintf (buf, bufsize, "target_supports = 0x%x\n", target_supports);
  buffer_ += buf;
  trace_depth_++;
  cat_security_association(target_supports);
  trace_depth_--;
  indent ();
  ACE_OS::snprintf (buf, bufsize, "target_requires = 0x%x\n", target_requires);
  buffer_ += buf;
  trace_depth_++;
  cat_security_association(target_requires);
  trace_depth_--;

  return true;
}

CORBA::Boolean
Catior_i::cat_octet_seq (const char *object_name,
                         TAO_InputCDR& stream)
{
  CORBA::ULong length = 0;
  if (!stream.read_ulong (length))
    return true;

  static const size_t bufsize = 512;
  char buf[bufsize];

  indent ();
  ACE_OS::snprintf (buf, bufsize, "%s len:\t%d\n", object_name, length);
  buffer_ += buf;

  indent ();
  ACE_OS::snprintf (buf, bufsize,  "%s as hex:\n", object_name);
  buffer_ += buf;

  CORBA::Octet anOctet;
  CORBA::String_var objKey = CORBA::string_alloc (length + 1);

  short counter = -1;

  indent ();
  CORBA::ULong i = 0;

  for (; i < length; ++i)
    {
      if (++counter == 16)
        {
          buffer_ += "\n";
          indent ();
          counter = 0;
        }

      if (!stream.read_octet (anOctet))
        return false;

      ACE_OS::snprintf (buf, bufsize, "%02.2x ", anOctet);
      buffer_ += buf;
      objKey[i] = (char) anOctet;
    }

  objKey[i] = '\0';

  buffer_ += "\n";
  indent ();
  ACE_OS::snprintf (buf, bufsize,
              "The %s as string:\n", object_name);
  buffer_ += buf;
  indent ();

  for (i = 0; i < length; ++i)
    {
      char c = objKey[i];
      int tmp = (unsigned char) c; // isprint doesn't work with negative vals.(except EOF)
      if (ACE_OS::ace_isprint (static_cast<ACE_TCHAR> (tmp)))
        buffer_ += c;
      else
        buffer_ += ".";
    }

  buffer_ += "\n";

  return true;
}

CORBA::Boolean
Catior_i::cat_object_key (TAO_InputCDR& stream)
{
  // ... and object key.

  return cat_octet_seq ("Object Key", stream);
}

ACE_CString
Catior_i::_find_info (CORBA::ULong id)
{
  ACE_CString locale = "";
  ACE_Codeset_Registry::registry_to_locale (id, locale, 0, 0);
  return locale;
}

void
Catior_i::displayHex (TAO_InputCDR & str)
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

  static const size_t bufsize = 512;
  char buf[bufsize];
  ACE_OS::snprintf (buf, bufsize, " Hex - %x\tDescription - ", theSetId);
  buffer_ += buf;

  ACE_CString theDescr = _find_info (theSetId);

  if (theDescr.length () == 0)
    buffer_ += "Unknown CodeSet\n";
  else
    {
      buffer_ += theDescr.c_str ();
      buffer_ += "\n";
    }
}

CORBA::Boolean
Catior_i::cat_codeset_info (TAO_InputCDR& cdr)
{
  CORBA::ULong length = 0;
  if (cdr.read_ulong (length) == 0)
    return false;

  TAO_InputCDR stream (cdr, length);
  cdr.skip_bytes(length);

  static const size_t bufsize = 512;
  char buf[bufsize];

  ACE_OS::snprintf (buf, bufsize, "\tComponent length: %u\n", length);
  buffer_ += buf;

  buffer_ += "\tComponent byte order:\t";
  buffer_ += (stream.byte_order () ? "Little" : "Big");
  buffer_ += " Endian\n";

  // CodesetId for char
  // CORBA::ULong c_ncsId;
  buffer_ += "\tNative CodeSet for char: ";
  displayHex (stream);

  // number of Conversion Codesets for char
  CORBA::ULong c_ccslen = 0;

  if (!(stream >> c_ccslen))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read number of conversion codesets "
                       "for char.\n"),
                      false);

  ACE_OS::snprintf (buf, bufsize, "\tNumber of CCS for char %u\n", c_ccslen);
  buffer_ += buf;

  if (c_ccslen)
    buffer_ += "\tConversion Codesets for char are:\n";

  //  Loop through and display them
  CORBA::ULong index = 0;
  for ( ; index < c_ccslen; ++index)
    {
      // CodesetId for char
      ACE_OS::snprintf (buf, bufsize, "\t%u) ", index + 1L);
      buffer_ += buf;
      displayHex (stream);
    }

  // CodesetId for wchar
  buffer_ += "\tNative CodeSet for wchar: ";
  displayHex (stream);

  // number of Conversion Codesets for wchar
  CORBA::ULong w_ccslen=0;

  if (!(stream >> w_ccslen))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read number of conversion codesets "
                       "for wchar.\n"),
                      false);

  ACE_OS::snprintf (buf, bufsize, "\tNumber of CCS for wchar %u\n", w_ccslen);
  buffer_ += buf;

  if (w_ccslen)
    buffer_ += "\tConversion Codesets for wchar are:\n";

  //  Loop through and display them
  for (index = 0; index < w_ccslen; ++index)
    {
      ACE_OS::snprintf (buf, bufsize, "\t %u) ", index + 1L);
      buffer_ += buf;
      displayHex (stream);
    }
  return true;
}

CORBA::Boolean
Catior_i::cat_tagged_components (TAO_InputCDR& stream)
{
  // ... and object key.

  CORBA::ULong len;
  if (!(stream >> len))
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

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

      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "The component <%d> ID is ", i+1, tag);
      buffer_ += buf;

      if (tag == IOP::TAG_ORB_TYPE) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAG_ORB_TYPE)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_tag_orb_type(stream);
        trace_depth_ -= 2;
      } else if (tag == IOP::TAG_CODE_SETS) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAG_CODE_SETS)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_codeset_info(stream);
        trace_depth_ -= 2;
      } else if (tag == IOP::TAG_ALTERNATE_IIOP_ADDRESS) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAG_ALTERNATE_IIOP_ADDRESS)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_tag_alternate_endpoints (stream);
        trace_depth_ -= 2;
      } else if (tag == TAO_TAG_ENDPOINTS) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAO_TAG_ENDPOINTS)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_tao_tag_endpoints(stream);
        trace_depth_ -= 2;
      } else if (tag == IOP::TAG_POLICIES) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAG_POLICIES)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_tag_policies(stream);
        trace_depth_ -= 2;
      } else if (tag == 20U /* SSLIOP::TAG_SSL_SEC_TRANS */) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAG_SSL_SEC_TRANS)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_tag_ssl_sec_trans(stream);
        trace_depth_ -= 2;
      }  else if (tag == 38U /* TAG_RMI_CUSTOM_MAX_STREAM_FORMAT */) {
        ACE_OS::snprintf (buf, bufsize, "%d (TAG_RMI_CUSTOM_MAX_STREAM_FORMAT)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_octet_seq ("Component Value", stream);
        trace_depth_ -= 2;
      } else if (tag == 1229081866U /* IBM_PARTNER_VERSION */) {
        ACE_OS::snprintf (buf, bufsize, "%d (IBM_PARTNER_VERSION)\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_ibm_partner_version (stream);
        trace_depth_ -= 2;
      } else {
        ACE_OS::snprintf (buf, bufsize, "%d\n", tag);
        buffer_ += buf;
        trace_depth_ += 2;
        cat_octet_seq ("Component Value", stream);
        trace_depth_ -= 2;
      }
    }

  return true;
}

CORBA::Boolean
Catior_i::cat_profile_helper (TAO_InputCDR& stream,
                              const char *protocol)
{
  // OK, we've got an IIOP profile.  It's going to be
  // encapsulated ProfileData.  Create a new decoding stream and
  // context for it, and tell the "parent" stream that this data
  // isn't part of it any more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "cannot read encap length\n"), false);
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major = 1;
  CORBA::Octet iiop_version_minor = 0;
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 2))
    {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "detected new v%d.%d %C profile that catior cannot decode\n",
                        iiop_version_major,
                        iiop_version_minor,
                        protocol);
      buffer_ += buf;
      return true;
    }

  ACE_OS::snprintf (buf, bufsize,
                    "%s Version:\t%d.%d\n",
                    protocol, iiop_version_major, iiop_version_minor);
  buffer_ += buf;

  // Get host and port.
  CORBA::UShort port_number;
  CORBA::String_var hostname;
  if (!(str >> hostname.inout ()))
    {
      indent ();
      buffer_ += "problem decoding hostname\n";
      return true;
    }

  if (!(str >> port_number))
    return false;

  indent ();
  buffer_ += "Host Name:\t";
  buffer_ += hostname.in ();
  buffer_ += "\n";

  indent ();
  ACE_OS::snprintf (buf, bufsize, "Port Number:\t%d\n", port_number);
  buffer_ += buf;

  if (!cat_object_key (str))
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
Catior_i::cat_coiop_profile (TAO_InputCDR& stream)
{
  // OK, we've got an COIOP profile.  It's going to be
  // encapsulated ProfileData.  Create a new decoding stream and
  // context for it, and tell the "parent" stream that this data
  // isn't part of it any more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "cannot read encap length\n"), false);
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major = 1;
  CORBA::Octet iiop_version_minor = 0;
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 2))
    {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "detected new v%d.%d COIOP profile that catior cannot decode",
                        iiop_version_major,
                        iiop_version_minor);
      buffer_ += buf;
      return true;
    }

  ACE_OS::snprintf (buf, bufsize,
                    "COIOP Version:\t%d.%d\n",
                    iiop_version_major,
                    iiop_version_minor);
  buffer_ += buf;

  // Get host and port.
  CORBA::String_var uuid;
  if (!(str >> uuid.inout ()))
    {
      indent ();
      buffer_ += "problem decoding uuid\n";
      return true;
    }

  indent ();
  buffer_ += "UUID:\t";
  buffer_ += uuid.in ();
  buffer_ += "\n";

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
Catior_i::cat_iiop_profile (TAO_InputCDR& stream)
{
  return cat_profile_helper (stream, "IIOP");
}

CORBA::Boolean
Catior_i::cat_shmiop_profile (TAO_InputCDR& stream)
{
  return cat_profile_helper (stream, "SHMIOP");
}

CORBA::Boolean
Catior_i::cat_uiop_profile (TAO_InputCDR& stream)
{
  // OK, we've got a UIOP profile.  It's going to be encapsulated
  // ProfileData.  Create a new decoding stream and context for it,
  // and tell the "parent" stream that this data isn't part of it any
  // more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    return false;

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

  // Read and verify major, minor versions, ignoring UIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet uiop_version_major = 1;
  CORBA::Octet uiop_version_minor = 0;
  // It appears that as of April 2002 UIOP version is 1.2
  if (! (str.read_octet (uiop_version_major)
         && uiop_version_major == 1
         && str.read_octet (uiop_version_minor)
         && uiop_version_minor <= 2))
    {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "detected new v%d.%d UIOP profile",
                        uiop_version_major,
                        uiop_version_minor);
      buffer_ += buf;
      return true;
    }

  indent ();
  ACE_OS::snprintf (buf, bufsize,
                    "UIOP Version:\t%d.%d\n",
                    uiop_version_major,
                    uiop_version_minor);
  buffer_ += buf;

  // Get host and port.
  CORBA::String_var rendezvous;
  if ((str >> rendezvous.out ()) == 0)
    return false;

  indent ();
  buffer_ += "Rendezvous point:\t";
  buffer_ += rendezvous.in ();
  buffer_ += "\n";

  if (cat_object_key (str) == 0)
    return false;

  if (cat_tagged_components (str) == 0)
    return false;

  return true;
}

CORBA::Boolean
Catior_i::cat_sciop_profile (TAO_InputCDR& stream)
{
  // OK, we've got an SCIOP profile.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "cannot read encap length\n"), false);
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

  // Read and verify major, minor versions, ignoring IIOP
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major = 1;
  CORBA::Octet iiop_version_minor = 0;
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 0))
    {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "detected new v%d.%d SCIOP profile that catior cannot decode",
                        iiop_version_major,
                        iiop_version_minor);
      buffer_ += buf;
      return true;
    }

  indent ();
  ACE_OS::snprintf (buf, bufsize,
                    "SCIOP Version:\t%d.%d\n",
                    iiop_version_major,
                    iiop_version_minor);
  buffer_ += buf;

  // Get host and port.
  CORBA::UShort port_number;
  CORBA::UShort max_streams;
  CORBA::ULong addresses;

  if (!(str >> addresses))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to decode number of addresses\n."),
                      false);

  indent ();
  ACE_OS::snprintf (buf, bufsize, "Addresses:\t%d\n", addresses);
  buffer_ += buf;

  for (CORBA::ULong i = 0; i < addresses; ++i)
    {
      CORBA::String_var hostname;
      if (!(str >> hostname.inout ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%I problem decoding hostname\n"),
                            false);
        }

      indent ();
      buffer_ += "Host Name:\t";
      buffer_ += hostname.in ();
      buffer_ += "\n";
    }


  if (!(str >> port_number))
    return false;

  indent ();
  ACE_OS::snprintf (buf, bufsize, "Port Number:\t%d\n", port_number);
  buffer_ += buf;

  if (!(str >> max_streams))
    return false;

  indent ();
  ACE_OS::snprintf (buf, bufsize, "Max Streams:\t%d\n", max_streams);
  buffer_ += buf;

  // Unlike IIOP (1.0), SCIOP always has tagged_components.
  if (!cat_object_key (str) || !cat_tagged_components (str))
    return false;

  return true;
}


CORBA::Boolean
Catior_i::cat_nsk_profile_helper (TAO_InputCDR& stream,
                                  const char *protocol)
{
  // OK, we've got an NSK profile.  It's going to be
  // encapsulated ProfileData.  Create a new decoding stream and
  // context for it, and tell the "parent" stream that this data
  // isn't part of it any more.

  CORBA::ULong encap_len;
  if (stream.read_ulong (encap_len) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "cannot read encap length\n"), false);
    }

  // Create the decoding stream from the encapsulation in the
  // buffer, and skip the encapsulation.
  TAO_InputCDR str (stream, encap_len);

  if (str.good_bit () == 0 || stream.skip_bytes (encap_len) == 0)
    return false;

  static const size_t bufsize = 512;
  char buf[bufsize];

  // Read and verify major, minor versions, ignoring NSK
  // profiles whose versions we don't understand.
  //
  // XXX this doesn't actually go back and skip the whole
  // encapsulation...
  CORBA::Octet iiop_version_major = 1;
  CORBA::Octet iiop_version_minor = 0;
  if (! (str.read_octet (iiop_version_major)
         && iiop_version_major == 1
         && str.read_octet (iiop_version_minor)
         && iiop_version_minor <= 2))
    {
      indent ();
      ACE_OS::snprintf (buf, bufsize,
                        "detected new v%d.%d %C profile that catior cannot decode",
                        iiop_version_major,
                        iiop_version_minor,
                        protocol);
      buffer_ += buf;
      return true;
    }

  ACE_OS::snprintf (buf, bufsize,
                    "%s Version:\t%d.%d\n",
                    protocol,
                    iiop_version_major,
                    iiop_version_minor);
  buffer_ += buf;

  // Get address
  char* fsaddress;
  if (!(str >> fsaddress))
    {
      indent ();
      buffer_ += "problem decoding file system address\n";
      return true;
    }


  indent ();
  buffer_ += "FS Address:\t";
  buffer_ += fsaddress;
  buffer_ += "\n";
  CORBA::string_free (fsaddress);

  if (cat_object_key (str) == 0)
    return false;

  // Version 1.0 does not have tagged_components.
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
Catior_i::cat_nskpw_profile (TAO_InputCDR& stream)
{
  return cat_nsk_profile_helper (stream, "NSKPW");
}

CORBA::Boolean
Catior_i::cat_nskfs_profile (TAO_InputCDR& stream)
{
  return cat_nsk_profile_helper (stream, "NSKFS");
}

void
Catior_i::indent()
{
  if (trace_depth_ != 0)
    {
      for(size_t i = 0; i < trace_depth_; i++)
        buffer_ += "    ";
    }
}

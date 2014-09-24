// -*- C++ -*-
// $Id$

#include "tao/CORBALOC_Parser.h"

#if (TAO_HAS_CORBALOC_PARSER == 1)

#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/MProfile.h"
#include "tao/Connector_Registry.h"
#include "tao/Transport_Connector.h"
#include "tao/Protocol_Factory.h"
#include "tao/debug.h"
#include "tao/SystemException.h"
#include "ace/Vector_T.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_string.h"

#include "ace/os_include/os_netdb.h"

#if !defined(__ACE_INLINE__)
#include "tao/CORBALOC_Parser.inl"
#endif /* __ACE_INLINE__ */

static const char prefix[] = "corbaloc:";
static const size_t prefix_len = sizeof prefix - 1;
static const char rir_token[] = "rir:";
static const size_t rir_token_len = sizeof rir_token - 1;
static const char iiop_token[] = "iiop:";
static const char iiop_token_len = sizeof iiop_token - 1;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CORBALOC_Parser::~TAO_CORBALOC_Parser (void)
{
}

bool
TAO_CORBALOC_Parser::match_prefix (const char *ior_string) const
{
  // Check if the prefix is 'corbaloc:' and return the result.
  return (ACE_OS::strncmp (ior_string,
                           prefix,
                           prefix_len) == 0);
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::make_stub_from_mprofile (CORBA::ORB_ptr orb,
                                              TAO_MProfile &mprofile)
{
  // Create a TAO_Stub.
  TAO_Stub *data = orb->orb_core ()->create_stub ((const char *) 0, mprofile);

  TAO_Stub_Auto_Ptr safe_data (data);

  CORBA::Object_var obj = orb->orb_core ()->create_object (data);

  if (!CORBA::is_nil (obj.in ()))
    {
      /// All is well, so release the stub object from its
      /// auto_ptr.
      (void) safe_data.release ();

      /// Return the object reference to the application.
      return obj._retn ();
    }

  /// Shouldnt come here: if so, return nil reference.
  return CORBA::Object::_nil ();
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string_rir_helper (const char * ior,
                                              CORBA::ORB_ptr orb)
{
  // Pass the key string as an argument to resolve_initial_references.
  // NameService is the default if an empty key string is supplied.
  const char *objkey = ior + rir_token_len;
  if (*objkey == '/') // there is an explicit object key, which may
                      // validly be null.
    objkey++;

  CORBA::Object_var rir_obj =
    orb->resolve_initial_references (*objkey == '\0' ? "NameService" : objkey);

  return rir_obj._retn ();
}

CORBA::Object_ptr
TAO_CORBALOC_Parser::parse_string (const char * ior, CORBA::ORB_ptr orb)
{
  // The decomposition of a corbaloc string is in Section 13.6.10.
  //
  // following the "corbaloc:"
  //  a comma separated list of <prot_addr> strings
  //    for each,
  // Separate out the key, delimited by '/'
  // Split out the various parts of our corbaloc string, comma-delimited
  // For each part
  //   Determine the protocol
  //     If rir, defer to another function and return the object
  //     If iiop, make the profile with <endpoint>:<port>/<key>
  //     If another protocol, use <remainder>/<key>
  //   Search through the collection of protocols for the correct one
  //     If not found, throw exception
  //     If found, make our_connector from it.
  //     our_connector->make_mprofile_unchecked (...);
  //     object = this->make_stub_from_mprofile (...);
  // Return the object

  // Skip the prefix.  We know it is there because this method is only
  // called if match_prefix() returns 1.
  ior += ACE_OS::strlen(prefix);

  //  First check for rir
  if (ACE_OS::strncmp (ior,rir_token,rir_token_len) == 0)
    return this->parse_string_rir_helper (ior,orb);

  // set up space for parsed endpoints. there will be at least 1, and
  // most likely commas will separate endpoints, although they could be
  // part of an endpoint address for some protocols.
  size_t max_endpoint_count = 1;
  for (const char *comma = ACE_OS::strchr (ior,',');
       comma;
       comma = ACE_OS::strchr (comma+1,','))
    ++max_endpoint_count;

  ACE_Array<parsed_endpoint> endpoints(max_endpoint_count);
  endpoints.size (0);

  // Get the Connector Registry from the ORB.
  TAO_Connector_Registry *conn_reg =
    orb->orb_core ()->connector_registry();

  while (1) { // will loop on comma only.
    size_t len = 0;
    size_t ndx = endpoints.size();
    endpoints.size(ndx+1);
    int uiop_compatible = 0;
    TAO_ConnectorSetIterator conn_iter = 0;
    for (conn_iter = conn_reg->begin();
         conn_iter != conn_reg->end() &&
           endpoints[ndx].profile_ == 0;
         conn_iter ++)
      {
        endpoints[ndx].profile_ =
          (*conn_iter)->corbaloc_scan(ior,len);

        if (endpoints[ndx].profile_)
          {
            endpoints[ndx].obj_key_sep_ =
              (*conn_iter)->object_key_delimiter();
            uiop_compatible = (endpoints[ndx].obj_key_sep_ == '|');
            this->make_canonical (ior,len,endpoints[ndx].prot_addr_);
            ior += len;
            break;
          }
      }

    if (endpoints[ndx].profile_ == 0)
      {
        if (TAO_debug_level)
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT("TAO (%P|%t) - TAO_CORBALOC_Parser::parse_string ")
                      ACE_TEXT("could not parse from %C\n"),
                      ior));
        throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO);
      }
    if (*ior == ',') // more endpoints follow
      {
        ++ior;
        continue;
      }

    if (*ior == '/') // found key separator
      {
        ++ior;
        break;
      }

    if (*ior == '\0') // no key separator appended, use default key
      {
        break;
      }

    if (uiop_compatible && *(ior - 1) == '|')
      // Assume this is an old uiop style corbaloc. No need to warn here,
      // the UIOP_Connector::corbaloc_scan already did.
      break;

    // anything else is a violation.
    if (TAO_debug_level)
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - TAO_CORBALOC_Parser::parse_string ")
                  ACE_TEXT("could not parse from %C\n"),
                  ior));
    throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO);
  } // end of while

  // At this point, ior points at the start of the object key
  ACE_CString obj_key (*ior ? ior : (const char *)"NameService");

  // now take the collection of endpoints along with the decoded key and
  // mix them together to get the mprofile.
  TAO_MProfile mprofile (endpoints.size());

  for (size_t i = 0; i < endpoints.size(); i++)
    {
      ACE_CString full_ep = endpoints[i].prot_addr_ +
        endpoints[i].obj_key_sep_ +
        obj_key;
      const char * str = full_ep.c_str();
      endpoints[i].profile_->parse_string (str);
      int share = orb->orb_core()->orb_params()->shared_profile();
      if (mprofile.give_profile(endpoints[i].profile_, share) != -1)
        endpoints[i].profile_ = 0;
      else
        {
          // Although this ought never happen, we want to make some
          // indication back to the caller, more as an audit trail than
          // anything else. The only failure possible is that there was
          // insufficient heap to allocate the mprofile, hence the
          // mprofile's size is 0, and give_profile fails.
          if (TAO_debug_level)
            TAOLIB_ERROR ((LM_ERROR,
                        ACE_TEXT("TAO (%P|%t) - TAO_CORBALOC_Parser::parse_string ")
                        ACE_TEXT("mprofile.give_profile failed for i = %d\n"),
                        i));
          throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 10, CORBA::COMPLETED_NO);
        }
    }

  // Get an object stub out.
  return this->make_stub_from_mprofile (orb, mprofile);
}

void
TAO_CORBALOC_Parser::make_canonical (const char *ior,
                                     size_t prot_addr_len,
                                     ACE_CString &canonical_endpoint)
{
  const char *separator = ACE_OS::strchr (ior, ':');

  // A special case for handling iiop
  if (ior[0] != ':' && ACE_OS::strncmp (ior,iiop_token,iiop_token_len) != 0)
    {
      canonical_endpoint.set (separator+1,
                              prot_addr_len - (separator - ior) - 1,1);
      return;
    }

  const char *addr_base = separator+1;
  const char *addr_tail = ior + prot_addr_len;
  // skip past version, if any
  separator = ACE_OS::strchr (addr_base,'@');
  if (separator != 0 && separator < addr_tail)
    {
      canonical_endpoint.set (addr_base,(separator - addr_base)+1,1);
      addr_base = separator + 1;
    }
  else
    canonical_endpoint.clear ();

  ACE_CString raw_host;
  ACE_CString raw_port;
  separator = ACE_OS::strchr (addr_base,':');
#if defined (ACE_HAS_IPV6)
  // IPv6 numeric address in host string?

  // Check if this is an address containing a decimal IPv6 address representation.
  if (addr_base < addr_tail && addr_base[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      const char *cp_pos = ACE_OS::strchr(addr_base, ']');
      if (cp_pos == 0 || cp_pos >= addr_tail)
        {
          // No valid IPv6 address specified but that will come out later.
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) - TAO_CORBALOC_Parser: ")
                         ACE_TEXT ("Invalid IPv6 decimal address specified.\n")));
            }
          separator = 0;
        }
      else
        {
          if (cp_pos[1] == ':')    // Look for a port
            separator = cp_pos + 1;
          else
            separator = 0;
        }
    }
#endif /* ACE_HAS_IPV6 */

  if (separator != 0 && separator < addr_tail)
    {
      // we have a port number
      raw_host.set (addr_base, (separator - addr_base), 1);
      raw_port.set (separator, (addr_tail - separator), 1);
    }
  else
    {
      // we must default port #
      if (addr_base < addr_tail)
        raw_host.set (addr_base, (addr_tail - addr_base),1);
      raw_port.set (":2809");
    }

  if (raw_host.length() == 0)
    {
      ACE_INET_Addr host_addr;

      char tmp_host [MAXHOSTNAMELEN + 1];

      // If no host is specified: assign the default host, i.e. the
      // local host.
      if (host_addr.get_host_name (tmp_host,
                                   sizeof (tmp_host)) != 0)
        {
          // Can't get the IP address since the INET_Addr wasn't
          // initialized.  Just throw an exception.

          if (TAO_debug_level > 0)
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("Cannot determine hostname.\n")));

          throw ::CORBA::INV_OBJREF
                     (CORBA::SystemException::_tao_minor_code
                      (TAO::VMCID, EINVAL),
                      CORBA::COMPLETED_NO);
        }
      else
        {
          canonical_endpoint += tmp_host;
        }
    }
  else
    {
      canonical_endpoint += raw_host;
    }

  canonical_endpoint += raw_port;
}



ACE_STATIC_SVC_DEFINE (TAO_CORBALOC_Parser,
                       ACE_TEXT ("CORBALOC_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CORBALOC_Parser),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_CORBALOC_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBALOC_PARSER == 1 */

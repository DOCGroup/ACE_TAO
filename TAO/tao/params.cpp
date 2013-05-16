// $Id$

#include "tao/params.h"
#include "tao/orbconf.h"
#include "tao/Version.h"

#if !defined (__ACE_INLINE__)
# include "tao/params.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_Thread.h"
#include "ace/Service_Config.h"
#include "tao/Invocation_Utils.h"

#if !defined (TAO_ALLOW_ZIOP_NO_SERVER_POLICIES_DEFAULT)
# define TAO_ALLOW_ZIOP_NO_SERVER_POLICIES_DEFAULT false
#endif /* !TAO_ALLOW_ZIOP_NO_SERVER_POLICIES_DEFAULT */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ORB_Parameters::TAO_ORB_Parameters (void)
  : endpoints_map_ (10)
  , mcast_discovery_endpoint_ ()
  , default_init_ref_ (TAO_DEFAULT_INIT_REFERENCE_INITIALIZER)
  , sock_rcvbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ)
  , sock_sndbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ)
  , nodelay_ (1)
  , sock_keepalive_ (0)
  , sock_dontroute_ (0)
  , ip_hoplimit_ (-1)
  , ip_multicastloop_ (true)
  , cdr_memcpy_tradeoff_ (ACE_DEFAULT_CDR_MEMCPY_TRADEOFF)
  , max_message_size_ (0) // Disable outgoing GIOP fragments by default
  , use_dotted_decimal_addresses_ (0)
  , cache_incoming_by_dotted_decimal_address_ (0)
  , linger_ (-1)
  , accept_error_delay_ (0)
  , std_profile_components_ (1)
  , ace_sched_policy_ (ACE_SCHED_OTHER)
  , sched_policy_ (THR_SCHED_DEFAULT)
  , scope_policy_ (THR_SCOPE_PROCESS)
  , single_read_optimization_ (1)
  , shared_profile_ (0)
  , use_parallel_connects_ (false)
  , parallel_connect_delay_ (0)
  , pref_network_ ()
  , disable_rt_collocation_resolver_ (false)
  , enforce_preferred_interfaces_ (false)
#if defined (ACE_HAS_IPV6)
  , prefer_ipv6_interfaces_ (false)
  , connect_ipv6_only_ (false)
  , use_ipv6_link_local_ (false)
#endif /* ACE_HAS_IPV6 */
  , negotiate_codesets_ (true)
  , ami_collication_ (true)
  , protocols_hooks_name_ ("Protocols_Hooks")
  , stub_factory_name_ ("Default_Stub_Factory")
  , endpoint_selector_factory_name_ ("Default_Endpoint_Selector_Factory")
  , thread_lane_resources_manager_factory_name_ ("Default_Thread_Lane_Resources_Manager_Factory")
  , dynamic_thread_pool_config_name_ ()
  , poa_factory_name_ ("TAO_Object_Adapter_Factory")
  , poa_factory_directive_
      (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("TAO_Object_Adapter_Factory",
                                     "TAO_PortableServer",
                                     TAO_VERSION,
                                     "_make_TAO_Object_Adapter_Factory",
                                     ""))
  , forward_invocation_on_object_not_exist_ (false)
  , forward_once_exception_ (0)
  , collocation_resolver_name_ ("Default_Collocation_Resolver")
  , allow_ziop_no_server_policies_ (!!TAO_ALLOW_ZIOP_NO_SERVER_POLICIES_DEFAULT)
{
  for (int i = 0; i != TAO_NO_OF_MCAST_SERVICES; ++i)
    {
      this->service_port_[i] = 0;
    }
}

void
TAO_ORB_Parameters::get_endpoint_set (const ACE_CString &lane,
                                      TAO_EndpointSet &endpoint_set)
{
  // Look for the lane in the endpoints map.
  endpoints_map_type::iterator const endpoints =
    this->endpoints_map_.find (lane);

  // If lane is not in the map, endpoint_set remains empty
  if (endpoints == this->endpoints_map_.end ())
    return;

  // At this point, the parsing should not fail since they have been
  // parsed successfully before.
  int const result =
    this->parse_and_add_endpoints ((*endpoints).second, endpoint_set);

  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);
}

int
TAO_ORB_Parameters::add_endpoints (const ACE_CString &lane,
                                   const ACE_CString &additional_endpoints)
{
  TAO_EndpointSet endpoint_set;

  // Parse the additional endpoints.
  int const result =
    this->parse_and_add_endpoints (additional_endpoints, endpoint_set);

  // Parse failure.
  if (result != 0)
    return result;

  // Look for the lane in the endpoints map.
  //
  // Return reference to endpoints string corresponding to lane
  // string.  If none, a default constructed string is inserted into
  // the map and returned.
  ACE_CString & existing_endpoints = this->endpoints_map_[lane];

  // Create the resulting endpoints string.
  if (existing_endpoints.length () != 0)
    {
      existing_endpoints += ";";
    }

  existing_endpoints += additional_endpoints;

  return 0;
}

int
TAO_ORB_Parameters::parse_and_add_endpoints (const ACE_CString &endpoints,
                                             TAO_EndpointSet &endpoint_set)
{
  // Parse the string into seperate endpoints, where `endpoints' is of
  // the form:
  //
  //    protocol1://V,v@addr1,...,addrN;protocol2://addr1,...,W.w@addrN;...
  //
  // A single endpoint, instead of several, can be added just as well.

  static char const endpoints_delimiter = ';';

  size_t const length = endpoints.length ();

  if (endpoints[0] == endpoints_delimiter ||
      endpoints[length - 1] == endpoints_delimiter)
    {
      return -1;
      // Failure: endpoints string has an empty endpoint at the
      // beginning or the end of the string
      // (e.g. ";uiop://foo;iiop://1.3@bar")
    }

  int status = 0;
  // Return code:  0 = success,  -1 = failure

  if (length > 0)
    {
      int endpoints_count = 1;

      for (size_t j = 0; j != length; ++j)
        {
          if (endpoints[j] == endpoints_delimiter)
            {
              ++endpoints_count;
            }
        }

      ssize_t begin = 0;
      ssize_t end = endpoints.find (endpoints_delimiter);

      for (int i = 0; i < endpoints_count; ++i)
        {
          if (end == 0)
            {
              // Handle case where two consecutive endpoints `;;'
              // delimiters are found within the endpoints set.
              //
              // Is it enough to just skip over it or should we return an
              // error?
              continue;
            }

          ACE_CString const endpt =
            endpoints.substring (begin, end - begin);
          // The substring call will work even if `end' is equal to
          // ACE_CString::npos since that will just extract the substring
          // from the offset `begin' to the end of the string.

          // Check for a valid URL style endpoint set
          ACE_CString::size_type const check_offset = endpt.find ("://");

          if (check_offset > 0 &&
              check_offset != endpt.npos)
            {
              endpoint_set.enqueue_tail (endpt);
              // Insert endpoint into list
            }
          else
            {
              status = -1;  // Error: invalid URL style endpoint set
            }

          begin = end + 1;
          end = endpoints.find (endpoints_delimiter, begin);
        }
    }
  else
    {
      status = -1;
      // Failure:  Empty string
    }

  return status;
}

bool
TAO_ORB_Parameters::check_preferred_interfaces_string (const char *s)
{
  // Validates that s contains one or more comma separated
  // interfaces each consisting of a string with a single
  // assignment separator ('=' or ':')
  // Any other char is legal, although '*' and '?' will be
  // treated as wildcards.
  bool expect_assign = false;
  bool expect_comma = false;
  bool expect_char = true;
  bool expect_wild = true;
  bool found_remote = false;

  for (const char *p = s; *p; ++p) switch (*p)
    {
#if !defined (ACE_HAS_IPV6)
      // Can't use this as assignment operator when IPv6 decimal
      // addresses may be involved.
    case ':':
#endif /* ACE_HAS_IPV6 */
    case '=':
      if (!expect_assign)
        return false;
      found_remote = true;
      expect_assign = false;
      expect_char = true;
      expect_comma = false;
      expect_wild = true;
      break;

    case ',':
      if (!expect_comma)
        return false;
      found_remote = false;
      expect_assign = false;
      expect_char = true;
      expect_comma = false;
      expect_wild = true;
      break;

    case '*':
    case '?':
      if (!expect_wild)
        return false;
      expect_assign = !found_remote;
      expect_char = true;
      expect_comma = found_remote;
      expect_wild = false;
      break;

    default:
      if (!expect_char)
        return false;
      expect_assign = !found_remote;
      expect_char = true;
      expect_comma = found_remote;
      expect_wild = true;
      break;
    }

  return expect_comma && !expect_assign;
}

bool
TAO_ORB_Parameters::preferred_interfaces (const char *s)
{
  const bool valid= check_preferred_interfaces_string (s);
  if (valid)
    {
      // Append any valid string to those already specified
      // (by other -ORBPreferredInterfaces options that have been
      // seen previously)
      if (this->pref_network_.length ())
        this->pref_network_+= ',';
      this->pref_network_+= s;
    }

  return valid;
}

const char *
TAO_ORB_Parameters::preferred_interfaces (void) const
{
  return this->pref_network_.c_str ();
}

void
TAO_ORB_Parameters::enforce_pref_interfaces (bool p)
{
  this->enforce_preferred_interfaces_ = p;
}

bool
TAO_ORB_Parameters::enforce_pref_interfaces (void) const
{
  return this->enforce_preferred_interfaces_;
}

#if defined (ACE_HAS_IPV6)
void
TAO_ORB_Parameters::prefer_ipv6_interfaces (bool p)
{
  this->prefer_ipv6_interfaces_ = p;
}

bool
TAO_ORB_Parameters::prefer_ipv6_interfaces (void) const
{
  return this->prefer_ipv6_interfaces_;
}

void
TAO_ORB_Parameters::connect_ipv6_only (bool p)
{
  this->connect_ipv6_only_ = p;
}

bool
TAO_ORB_Parameters::connect_ipv6_only (void) const
{
  return this->connect_ipv6_only_;
}

void
TAO_ORB_Parameters::use_ipv6_link_local (bool p)
{
  this->use_ipv6_link_local_ = p;
}

bool
TAO_ORB_Parameters::use_ipv6_link_local (void) const
{
  return this->use_ipv6_link_local_;
}
#endif /* ACE_HAS_IPV6 */

void
TAO_ORB_Parameters::protocols_hooks_name (const char *s)
{
  this->protocols_hooks_name_ = s;
}

const char *
TAO_ORB_Parameters::protocols_hooks_name (void) const
{
  return this->protocols_hooks_name_.c_str ();
}

void
TAO_ORB_Parameters::thread_lane_resources_manager_factory_name (const char *s)
{
  this->thread_lane_resources_manager_factory_name_ = s;
}

const char *
TAO_ORB_Parameters::thread_lane_resources_manager_factory_name (void) const
{
  return this->thread_lane_resources_manager_factory_name_.c_str ();
}

void
TAO_ORB_Parameters::dynamic_thread_pool_config_name (const char *s)
{
  this->dynamic_thread_pool_config_name_ = s;
}

const char *
TAO_ORB_Parameters::dynamic_thread_pool_config_name (void) const
{
  return this->dynamic_thread_pool_config_name_.c_str ();
}

void
TAO_ORB_Parameters::stub_factory_name (const char *s)
{
  this->stub_factory_name_ = s;
}

const char *
TAO_ORB_Parameters::stub_factory_name (void) const
{
  return this->stub_factory_name_.c_str ();
}

void
TAO_ORB_Parameters::poa_factory_name (const char *s)
{
  this->poa_factory_name_ = s;
}

const char *
TAO_ORB_Parameters::poa_factory_name (void) const
{
  return this->poa_factory_name_.c_str ();
}

void
TAO_ORB_Parameters::poa_factory_directive (const ACE_TCHAR *s)
{
  this->poa_factory_directive_ = s;
}

const ACE_TCHAR *
TAO_ORB_Parameters::poa_factory_directive (void) const
{
  return this->poa_factory_directive_.c_str ();
}

void
TAO_ORB_Parameters::endpoint_selector_factory_name (const char *s)
{
  this->endpoint_selector_factory_name_ = s;
}

const char *
TAO_ORB_Parameters::endpoint_selector_factory_name (void) const
{
  return this->endpoint_selector_factory_name_.c_str ();
}

const TAO::Invocation_Retry_Params &
TAO_ORB_Parameters::invocation_retry_params (void) const
{
  return this->invocation_retry_params_;
}

TAO::Invocation_Retry_Params &
TAO_ORB_Parameters::invocation_retry_params (void)
{
  return this->invocation_retry_params_;
}

void
TAO_ORB_Parameters::forward_on_exception_limit (const int ef, const int limit)
{
  this->invocation_retry_params_.forward_on_exception_limit_[ef] = limit;
}

void
TAO_ORB_Parameters::forward_on_exception_delay (const ACE_Time_Value &delay)
{
  this->invocation_retry_params_.init_retry_delay_ = delay;
}

TAO_END_VERSIONED_NAMESPACE_DECL

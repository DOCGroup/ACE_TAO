
#include "tao/params.h"
#include "tao/orbconf.h"

#if !defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_Thread.h"


ACE_RCSID (tao,
           params,
           "$Id$")


TAO_ORB_Parameters::TAO_ORB_Parameters (void)
  : endpoints_map_ (10),
    mcast_discovery_endpoint_ (),
    default_init_ref_ (TAO_DEFAULT_INIT_REFERENCE_INITIALIZER),
    sock_rcvbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    sock_sndbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    nodelay_ (1),
    cdr_memcpy_tradeoff_ (ACE_DEFAULT_CDR_MEMCPY_TRADEOFF),
    use_lite_protocol_ (0),
    use_dotted_decimal_addresses_ (0),
    std_profile_components_ (1),
    ace_sched_policy_ (ACE_SCHED_OTHER),
    sched_policy_ (THR_SCHED_DEFAULT),
    scope_policy_ (THR_SCOPE_PROCESS),
    single_read_optimization_ (1),
    disable_rt_collocation_resolver_ (false)
{
  for (int i = 0; i != TAO_NO_OF_MCAST_SERVICES; ++i)
    {
      this->service_port_[i] = 0;
    }
}

TAO_ORB_Parameters::~TAO_ORB_Parameters (void)
{
}

void
TAO_ORB_Parameters::get_endpoint_set (const ACE_CString &lane,
                                      TAO_EndpointSet &endpoint_set)
{
  ACE_CString endpoints;

  // Look for the lane in the endpoints map.
  int result =
    this->endpoints_map_.find (lane, endpoints);

  // If lane is not in the map, <endpoint_set> remains empty
  if (result != 0)
    return;

  // At this point, the parsing should not fail since they have been
  // parsed successfully before.
  result =
    this->parse_and_add_endpoints (endpoints,
                                   endpoint_set);
  ACE_ASSERT (result == 0);
}

int
TAO_ORB_Parameters::add_endpoints (const ACE_CString &lane,
                                   const ACE_CString &additional_endpoints)
{
  TAO_EndpointSet endpoint_set;

  // Parse the additional endpoints.
  int result =
    this->parse_and_add_endpoints (additional_endpoints,
                                   endpoint_set);

  // Parse failure.
  if (result != 0)
    return result;

  // Look for the lane in the endpoints map.
  ACE_CString existing_endpoints;
  result =
    this->endpoints_map_.find (lane, existing_endpoints);

  // Create the resultant endpoints string.
  ACE_CString new_endpoints;
  if (result == 0)
    new_endpoints =
      existing_endpoints +
      ";" +
      additional_endpoints;
  else
    new_endpoints =
      additional_endpoints;

  result =
    this->endpoints_map_.rebind (lane,
                                 new_endpoints);

  if (result == -1)
    return result;

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

  int status = 0;
  // Return code:  0 = success,  -1 = failure

  const char endpoints_delimiter = ';';

  size_t length = endpoints.length ();

  if (endpoints[0] == endpoints_delimiter ||
      endpoints[length - 1] == endpoints_delimiter)
    {
      return -1;
      // Failure: endpoints string has an empty endpoint at the
      // beginning or the end of the string
      // (e.g. ";uiop://foo;iiop://1.3@bar")
    }

  if (length > 0)
    {
      int endpoints_count = 1;

      for (size_t j = 0; j != length; ++j)
        {
          if (endpoints[j] == endpoints_delimiter)
            {
              endpoints_count++;
            }
        }

      int begin = 0;
      int end = endpoints.find (endpoints_delimiter);

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

          ACE_CString endpt = endpoints.substring (begin, end - begin);
          // The substring call will work even if `end' is equal to
          // ACE_CString::npos since that will just extract the substring
          // from the offset `begin' to the end of the string.

          // Check for a valid URL style endpoint set
          int check_offset = endpt.find ("://");

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

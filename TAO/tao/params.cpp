
#include "tao/params.h"

#if !defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           params,
           "$Id$")


TAO_ORB_Parameters::TAO_ORB_Parameters (void)
  : endpoints_list_ (),
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
    single_read_optimization_ (1)
{
  for (int i = 0; i <= TAO_NO_OF_MCAST_SERVICES; ++i)
    this->service_port_[i] = 0;
}

TAO_ORB_Parameters::~TAO_ORB_Parameters (void)
{
}

int
TAO_ORB_Parameters::parse_endpoints (ACE_CString &endpoints,
                                     TAO_EndpointSet &endpoints_list)
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
            endpoints_count++;
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
              endpoints_list.enqueue_tail (endpt);
              // Insert endpoint into list
            }
          else
            status = -1;  // Error: invalid URL style endpoint set

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

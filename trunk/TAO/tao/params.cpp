// $Id$

#include "tao/params.h"

#if !defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#include "tao/IOR_LookupTable.h"

ACE_RCSID(tao, params, "$Id$")

TAO_ORB_Parameters::TAO_ORB_Parameters (void)
  : preconnects_list_ (),
    preconnect_insertion_strategy_ (this->preconnects_list_),
    endpoints_list_ (),
    endpoint_insertion_strategy_ (this->endpoints_list_),
    name_service_port_ (0),
    trading_service_port_ (0),
    implrepo_service_port_ (0),
    init_ref_ (),
    ior_lookup_table_ (0),
    default_init_ref_ (),
    sock_rcvbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    sock_sndbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    cdr_memcpy_tradeoff_ (ACE_DEFAULT_CDR_MEMCPY_TRADEOFF),
    use_lite_protocol_ (0),
    use_dotted_decimal_addresses_ (0),
    std_profile_components_ (1)
{
}

TAO_ORB_Parameters::~TAO_ORB_Parameters (void)
{
  // Delete the table.
  delete this->ior_lookup_table_;
  this->ior_lookup_table_ = 0;
}

int
TAO_ORB_Parameters::parse_endpoints (ACE_CString &endpoints,
                                     TAO_Base_Endpoint_Insertion_Strategy &
                                       endpoints_list)
{
  // Parse the string into seperate endpoints, where `endpoints' is of
  // the form:
  //
  //    protocol1://V,v@addr1,...,addrN;protocol2://addr1,...,W.w@addrN;...
  //
  // A single endpoint, instead of several, can be added just as well.

  int status = 0;
  // Return code:  0 = success,  1 = failure

  const char endpoints_delimiter = ';';

  int length = endpoints.length ();

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

      for (int j = 0; j != length; ++j)
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

          ACE_CString endpt = endpoints.substring (begin, end);
          // The substring call will work even if `end' is equal to
          // ACE_CString::npos since that will just extract the substring
          // from the offset `begin' to the end of the string.

          // Check for a valid URL style endpoint set
          int check_offset = endpt.find ("://");

          if (check_offset > 0 &&
              check_offset != endpt.npos)
            {
              endpoints_list.insert (endpt);
              // Insert endpoint into list
            }
          else
            status = -1;  // Error: invalid URL style endpoint set

          begin += end + 1;
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

// Don't bother inlining since the most used methods are virtual.

TAO_Preconnect_Insertion_Strategy::
   TAO_Preconnect_Insertion_Strategy (TAO_PreconnectSet &preconnects)
     : preconnects_ (preconnects)
{
}

int
TAO_Preconnect_Insertion_Strategy::insert (const ACE_CString &preconnect)
{
  return this->preconnects_.enqueue_tail (preconnect);
}

TAO_Endpoint_Insertion_Strategy::
  TAO_Endpoint_Insertion_Strategy (TAO_EndpointSet &endpoints)
    : endpoints_ (endpoints)
{
}

int
TAO_Endpoint_Insertion_Strategy::insert (const ACE_CString &endpoint)
{
  return this->endpoints_.insert (endpoint);
}

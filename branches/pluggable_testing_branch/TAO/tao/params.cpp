// $Id$

#include "tao/params.h"

#if !defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#include "tao/IOR_LookupTable.h"

ACE_RCSID(tao, params, "$Id$")

TAO_ORB_Parameters::TAO_ORB_Parameters (void)
  : name_service_port_ (0),
    trading_service_port_ (0),
    ior_lookup_table_ (0),
    sock_rcvbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    sock_sndbuf_size_ (ACE_DEFAULT_MAX_SOCKET_BUFSIZ),
    cdr_memcpy_tradeoff_ (ACE_DEFAULT_CDR_MEMCPY_TRADEOFF),
    use_lite_protocol_ (0)
{
}

TAO_ORB_Parameters::~TAO_ORB_Parameters (void)
{
  // Delete the table.
  delete this->ior_lookup_table_;
}

int 
TAO_ORB_Parameters::parse_endpoints (ACE_CString &endpoints,
                                     TAO_EndpointSet &endpoints_list)
{
  // Parse the string into seperate endpoints, where `endpoints' is of
  // the form:
  //
  //    protocol1:V.v//addr1,...,addrN/;protocol2:W.w//addr1,...,addrN/;...
  //
  // A single endpoint, instead of several, can be added just as well.

  const char endpoints_delimiter = ';';

  int length = endpoints.length ();

  if (endpoints[0] == endpoints_delimiter ||
      endpoints[length - 1] == endpoints_delimiter)
    {
      return -1;
      // Failure:  endpoints string has an empty endpoint at the beginning
      //           or the end of the string (e.g. ";uiop://foo;iiop:1.3//bar")
    }

  if (length > 0)
    {
      int endpoints_count = 1;

      for (int i = 0;
           i < length;
           ++i)
        {
          if (endpoints[i] == endpoints_delimiter)
            endpoints_count++;
        }

      int begin = 0;
      int end = endpoints.find (endpoints_delimiter);

      for (int i = 1; i < endpoints_count; ++i)
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

          endpoints_list.insert (endpoints.substring (begin, end));
          // The substring call will work even if `end' is equal to
          // ACE_CString::npos since that will just extract the substring
          // from the offset `begin' to the end of the string.

          begin += end + 1;
          end = endpoints.find (endpoints_delimiter, begin);
        }

      return 0;  // Success
    }
  else
    {
      return -1;
      // Failure:  Empty string
    }
}


#if defined (ACE_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_Cstring>;

template class ACE_Unbounded_Set<ACE_CString>;

template class ACE_Unbounded_Set_Iterator<ACE_CString, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_Cstring>

#pragma instantiate ACE_Unbounded_Set<ACE_CString>

#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_CString, ACE_Null_Mutex>


#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

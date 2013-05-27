// $Id$

#include "tao/MCAST_Parser.h"

#if (TAO_HAS_MCAST_PARSER == 1)

#include "tao/default_ports.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/debug.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

#if !defined(__ACE_INLINE__)
#include "tao/MCAST_Parser.inl"
#endif /* __ACE_INLINE__ */

static const char mcast_prefix[] = "mcast:";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MCAST_Parser::~TAO_MCAST_Parser (void)
{
}

bool
TAO_MCAST_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (ior_string,
                           ::mcast_prefix,
                           sizeof (::mcast_prefix) - 1) == 0);
}

CORBA::Object_ptr
TAO_MCAST_Parser::parse_string (const char *ior, CORBA::ORB_ptr orb)
{
  char const * const mcast_name =
    ior + sizeof (::mcast_prefix) + 1;

  this->assign_to_variables (mcast_name);

  /*
   * Now that we got the global variables.
   * we can invoke multicast_to_service and multicast_query
   */
  ACE_Time_Value *timeout = orb->get_timeout ();

  return
    this->multicast_to_service (service_name_.in (),
                                this->mcast_port_,
                                this->mcast_address_.in (),
                                this->mcast_ttl_,
                                this->mcast_nic_.in (),
                                orb,
                                timeout);
}

CORBA::Object_ptr
TAO_MCAST_Parser::multicast_to_service (const char *service_name,
                                        unsigned short port,
                                        const char *mcast_address,
                                        int mcast_ttl,
                                        const char *mcast_nic,
                                        CORBA::ORB_ptr orb,
                                        ACE_Time_Value *timeout)
{
  char buf[TAO_DEFAULT_IOR_SIZE];
  char * ior = buf;

  // Use UDP multicast to locate the  service.
  int const result = this->multicast_query (ior,
                                            service_name,
                                            port,
                                            mcast_address,
                                            mcast_ttl,
                                            mcast_nic,
                                            timeout,
                                            orb);

  CORBA::Object_var return_value;

  if (result == 0)
    {
      CORBA::String_var cleaner;
      // If the IOR didn't fit into <buf>, memory for it was dynamically
      // allocated - make sure it gets deallocated.
      if (ior != buf)
        cleaner = ior;

      // Convert IOR to an object reference.
      return_value =
        orb->string_to_object (ior);
    }

  // Return object reference.
  return return_value._retn ();
}

int
TAO_MCAST_Parser::multicast_query (char* & buf,
                                   const char *service_name,
                                   unsigned short port,
                                   const char *mcast_address,
                                   int mcast_ttl,
                                   const char *mcast_nic,
                                   ACE_Time_Value *timeout,
                                   CORBA::ORB_ptr orb)
{
  ACE_INET_Addr my_addr;
  ACE_SOCK_Acceptor acceptor;
  ACE_SOCK_Stream stream;
  ACE_SOCK_Dgram dgram;

  ssize_t result = 0;

  // Bind listener to any port and then find out what the port was.
#if defined (ACE_HAS_IPV6)
  if (acceptor.open (ACE_Addr::sap_any, 0, AF_INET6) == -1
#else /* ACE_HAS_IPV6 */
  if (acceptor.open (ACE_Addr::sap_any) == -1
#endif /* !ACE_HAS_IPV6 */
      || acceptor.get_local_addr (my_addr) == -1)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("acceptor.open () || ")
                  ACE_TEXT ("acceptor.get_local_addr () failed\n")));
      result = -1;
    }
  else
    {
      if (TAO_debug_level > 0)
        {
          ACE_TCHAR addr[64];
          my_addr.addr_to_string (addr, sizeof(addr));
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT(" TAO (%P|%t) - MCAST_Parser: acceptor local address %s.\n"),
                      addr));
        }

      ACE_INET_Addr multicast_addr (port,
                                    mcast_address);

      // Set the address if multicast_discovery_endpoint option
      // is specified for the Naming Service.
      ACE_CString mde (orb->orb_core ()->orb_params ()
                       ->mcast_discovery_endpoint ());

      if (ACE_OS::strcasecmp (service_name,
                              "NameService") == 0
          && mde.length () != 0)
        if (multicast_addr.set (mde.c_str()) == -1)
          {
            TAOLIB_ERROR ((LM_ERROR,
                        ACE_TEXT("ORB.cpp: Multicast address setting failed\n")));
            stream.close ();
            dgram.close ();
            acceptor.close ();
            return -1;
          }

      // Open the datagram.
      if (dgram.open (ACE_Addr::sap_any, multicast_addr.get_type ()) == -1)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to open the Datagram!\n")));
          result = -1;
        }
      else
        {
          // Set NIC
          dgram.set_nic (ACE_TEXT_CHAR_TO_TCHAR (mcast_nic),
                         multicast_addr.get_type ());

          // Set TTL
          int mcast_ttl_optval = mcast_ttl;

#if defined (ACE_HAS_IPV6)
          if (multicast_addr.get_type () == AF_INET6)
            {
              if (dgram.set_option (
                    IPPROTO_IPV6,
                    IPV6_MULTICAST_HOPS,
                    &mcast_ttl_optval,
                    sizeof (mcast_ttl_optval)) != 0)
                return -1;
            }
          else
#endif  /* ACE_HAS_IPV6 */
          if (dgram.set_option (
                IPPROTO_IP,
                IP_MULTICAST_TTL,
                &mcast_ttl_optval,
                sizeof (mcast_ttl_optval)) != 0)
            result = -1;

          // Convert the acceptor port into network byte order.
          ACE_UINT16 response_port =
            (ACE_UINT16) ACE_HTONS (my_addr.get_port_number ());

          // Length of service name we will send.
          CORBA::Short data_len =
            (CORBA::Short) ACE_HTONS (ACE_OS::strlen (service_name) + 1);

          // Vector we will send.  It contains: 1) length of service
          // name string, 2)port on which we are listening for
          // replies, and 3) name of service we are looking for.
          const int iovcnt = 3;
          iovec iovp[iovcnt];

          // The length of service name string.
          iovp[0].iov_base = (char *) &data_len;
          iovp[0].iov_len  = sizeof (CORBA::Short);

          // The port at which we are listening.
          iovp[1].iov_base = (char *) &response_port;
          iovp[1].iov_len  = sizeof (ACE_UINT16);

          // The service name string.
          iovp[2].iov_base = (char *) service_name;
          iovp[2].iov_len  =
            static_cast<u_long> (ACE_OS::strlen (service_name) + 1);

          // Send the multicast.
          result = dgram.send (iovp,
                               iovcnt,
                               multicast_addr);

          if (TAO_debug_level > 0)
            TAOLIB_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\nsent multicast request.")));

          // Check for errors.
          if (result == -1)
            TAOLIB_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("error sending IIOP multicast")));
          else
            {
              if (TAO_debug_level > 0)
                TAOLIB_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("\n%N; Sent multicast.")
                            ACE_TEXT ("# of bytes sent is %d.\n"),
                            result));
              // Wait for response until timeout.
              ACE_Time_Value tv (
                timeout == 0
                ? ACE_Time_Value (TAO_DEFAULT_SERVICE_RESOLUTION_TIMEOUT)
                : *timeout);

              // Accept reply connection from server.
              if (acceptor.accept (stream,
                                   0,
                                   &tv) == -1)
                {
                  TAOLIB_ERROR ((LM_ERROR,
                              ACE_TEXT ("%p\n"),
                              ACE_TEXT ("multicast_query: unable to accept")));
                  result = -1;
                }
              else
                {
                  // Receive the IOR.

                  // IOR length.
                  CORBA::Short ior_len;
                  result = stream.recv_n (&ior_len,
                                          sizeof ior_len,
                                          0,
                                          &tv);
                  if (result != sizeof (ior_len))
                    {
                      TAOLIB_ERROR ((LM_ERROR,
                                  ACE_TEXT ("%p\n"),
                                  ACE_TEXT ("multicast_query: unable to receive ")
                                  ACE_TEXT ("ior length")));
                      result = -1;
                    }
                  else
                    {
                      // Allocate more space for the ior if we don't
                      // have enough.
                      ior_len = (CORBA::Short) ACE_NTOHS (ior_len);
                      if (ior_len >= TAO_DEFAULT_IOR_SIZE)
                        {
                          buf = CORBA::string_alloc (ior_len);
                          if (buf == 0)
                            {
                              TAOLIB_ERROR ((LM_ERROR,
                                          ACE_TEXT ("%p\n"),
                                          ACE_TEXT ("multicast_query: unable to ")
                                          ACE_TEXT ("allocate memory")));
                              result = -1;
                            }
                        }

                      if (result != -1)
                        {
                          // Receive the ior.
                          result = stream.recv_n (buf,
                                                  ior_len,
                                                  0,
                                                  &tv);
                          if (result == -1)
                            TAOLIB_ERROR ((LM_ERROR,
                                        ACE_TEXT ( "%p\n"),
                                        ACE_TEXT ("error reading ior")));
                          else if (TAO_debug_level > 0)
                            TAOLIB_DEBUG ((LM_DEBUG,
                                        ACE_TEXT ("%N: service resolved to IOR <%C>\n"),
                                        buf));
                        }
                    }
                }
            }
        }
        if (result == -1)
          {
            TAOLIB_ERROR ((LM_ERROR,
                        ACE_TEXT("\nmulticast discovery of %C failed.\n"),
                        service_name));

            if (ACE_OS::strcasecmp (service_name,
                                    "NameService") == 0)
             {
               TAOLIB_ERROR ((LM_ERROR,
                           ACE_TEXT("Specify -m 1 when starting Naming_Service,\n")
                           ACE_TEXT("or see http://www.theaceorb.com/faq/#115\n")
                           ACE_TEXT("for using NameService without multicast.\n\n")));
             }
          }
    }

  // Clean up.
  stream.close ();
  dgram.close ();
  acceptor.close ();

  return result == -1 ? -1 : 0;
}

void
TAO_MCAST_Parser::assign_to_variables (char const * mcast_name)
{
  /*
   * The format now is "multicast_address:port:nicaddress:ttl/object_key"
   */
  ACE_CString mcast_name_cstring (mcast_name);

  ACE_CString::size_type pos_colon1 = mcast_name_cstring.find (':', 0);

#if defined (ACE_HAS_IPV6)
  // IPv6 numeric address in host string?
  bool ipv6_in_host = false;

  // Check if this is an mcast address containing a
  // decimal IPv6 address representation.
  if (mcast_name_cstring[0] == '[')
    {
      // In this case we have to find the end of the numeric address and
      // start looking for the port separator from there.
      ACE_CString::size_type const cp_pos = mcast_name_cstring.find (']', 0);
      if (cp_pos == 0)
        {
          // No valid IPv6 address specified.
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("\nTAO (%P|%t) MCAST_Parser: ")
                          ACE_TEXT ("Invalid IPv6 decimal address specified.\n")));
            }

          return;
        }
      else
        {
          if (mcast_name_cstring[cp_pos + 1] == ':')    // Look for a port
            pos_colon1 = cp_pos + 1;
          else
            pos_colon1 = cp_pos;
          ipv6_in_host = true; // host string contains full IPv6 numeric address
        }
    }
#endif  /* ACE_HAS_IPV6 */

  if (pos_colon1 == 0)
    {
#if defined (ACE_HAS_IPV6)
      const char default_addr[] = ACE_DEFAULT_MULTICASTV6_ADDR;
#else /* ACE_HAS_IPV6 */
      const char default_addr[] = ACE_DEFAULT_MULTICAST_ADDR;
#endif  /* !ACE_HAS_IPV6 */
      this->mcast_address_ = default_addr;
    }
  else
    {
#if defined (ACE_HAS_IPV6)
      if (ipv6_in_host)
        this->mcast_address_ =
          mcast_name_cstring.substring (1,
                                        pos_colon1 - 2).c_str ();
      else
#endif  /* ACE_HAS_IPV6 */
      this->mcast_address_ =
        mcast_name_cstring.substring (0,
                                      pos_colon1).c_str ();
    }
  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon1 + 1,
                                  mcast_name_cstring.length() -
                                  pos_colon1);

  ACE_CString::size_type const pos_colon2 = mcast_name_cstring.find (':', 0);

  if (pos_colon2 == 0)
    {
      if (mcast_name_cstring.find ("InterfaceRepository") != ACE_CString::npos)
        {
          this->mcast_port_ =
            TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT;
        }
      else if (mcast_name_cstring.find ("ImplRepoService") != ACE_CString::npos)
        {
          this->mcast_port_ =
            TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;
        }
      else if (mcast_name_cstring.find ("TradingService") != ACE_CString::npos)
        {
          this->mcast_port_ = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
        }
    }
  else
    {
      int const the_port =
        ACE_OS::atoi (mcast_name_cstring.substring (0, pos_colon2).c_str ());

      if (the_port >= 0 && the_port <= ACE_MAX_DEFAULT_PORT)
        this->mcast_port_ = the_port;
    }

  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon2 + 1,
                                  mcast_name_cstring.length() - pos_colon2);


  ACE_CString::size_type const pos_colon3 = mcast_name_cstring.find (':', 0);

  this->mcast_nic_ = mcast_name_cstring.substring (0, pos_colon3).c_str ();

  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon3 + 1,
                                  mcast_name_cstring.length() - pos_colon3);

  ACE_CString::size_type const pos_colon4 = mcast_name_cstring.find ('/', 0);

  if (pos_colon4 != 0)
    {
      // Change TTL to non-default value.
      int const the_ttl =
        ACE_OS::atoi (mcast_name_cstring.substring (0, pos_colon4).c_str ());

      if (the_ttl > 0 && the_ttl <= 255)  // Valid TTLs: (0, 255]
        this->mcast_ttl_ = the_ttl;
    }

  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon4,
                                  mcast_name_cstring.length() - pos_colon4);

  this->service_name_ =
    mcast_name_cstring.substring (1, mcast_name_cstring.length() - 1).c_str ();
}

ACE_STATIC_SVC_DEFINE (TAO_MCAST_Parser,
                       ACE_TEXT ("MCAST_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_MCAST_Parser),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_MCAST_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_MCAST_PARSER == 1 */

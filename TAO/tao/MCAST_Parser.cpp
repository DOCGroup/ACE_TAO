// $Id$

#include "MCAST_Parser.h"
#include "tao/Object_Loader.h"
#include "tao/Object.h"
#include "tao/ORB.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Stream.h"

#if !defined(__ACE_INLINE__)
#include "MCAST_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, MCAST_Parser, "$Id$")

TAO_MCAST_Parser::~TAO_MCAST_Parser (void)
{
}

static const char mcast_prefix[] = "mcast:";

int
TAO_MCAST_Parser::match_prefix (const char *ior_string) const
{
  return (ACE_OS::strncmp (ior_string,
                           ::mcast_prefix,
                           sizeof (::mcast_prefix) - 1) == 0);
}

CORBA::Object_ptr
TAO_MCAST_Parser::parse_string (const char *ior,
                                CORBA::ORB_ptr orb
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  const char *mcast_name =
    ior + sizeof (::mcast_prefix) + 1;

  assign_to_variables (mcast_name);

  /*
   * Now that we got the global variables.
   * we can invoke multicast_to_service and multicast_query
   */
  CORBA::Object_ptr object = CORBA::Object::_nil ();

  CORBA::UShort port = (CORBA::UShort) ACE_OS::atoi (this->mcast_port_.in
                                                     ());

  ACE_Time_Value *timeout = orb->get_timeout ();

  object = multicast_to_service (service_name_.in (),
                                 port,
                                 this->mcast_address_.in (),
                                 this->mcast_ttl_.in (),
                                 this->mcast_nic_.in (),
                                 orb,
                                 timeout
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return object;
}

CORBA_Object_ptr
TAO_MCAST_Parser::multicast_to_service (const char *service_name,
                                        u_short port,
                                        const char *mcast_address,
                                        const char *mcast_ttl,
                                        const char *mcast_nic,
                                        CORBA::ORB_ptr orb,
                                        ACE_Time_Value *timeout
                                        ACE_ENV_ARG_DECL)
{
  char buf[2048];
  char *ior = buf;

  CORBA::String_var cleaner;

  CORBA_Object_var return_value =
    CORBA_Object::_nil ();

  // Use UDP multicast to locate the  service.
  int result = this->multicast_query (ior,
                                      service_name,
                                      port,
                                      mcast_address,
                                      mcast_ttl,
                                      mcast_nic,
                                      timeout,
                                      orb);

  // If the IOR didn't fit into <buf>, memory for it was dynamically
  // allocated - make sure it gets deallocated.
  if (ior != buf)
    cleaner = ior;

  if (result == 0)
    {
      // Convert IOR to an object reference.
      return_value =
        orb->string_to_object (ior
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA_Object::_nil ());
    }

  // Return object reference.
  return return_value._retn ();
}

int
TAO_MCAST_Parser::multicast_query (char *&buf,
                                   const char *service_name,
                                   u_short port,
                                   const char *mcast_address,
                                   const char *mcast_ttl,
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
  if (acceptor.open (ACE_Addr::sap_any) == -1
      || acceptor.get_local_addr (my_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("acceptor.open () || ")
                  ACE_TEXT ("acceptor.get_local_addr () failed")));
      result = -1;
    }
  else
    {
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
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT("ORB.cpp: Multicast address setting failed\n")));
            stream.close ();
            dgram.close ();
            acceptor.close ();
            return -1;
          }

      // Open the datagram.
      if (dgram.open (ACE_Addr::sap_any) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to open the Datagram!\n")));
          result = -1;
        }
      else
        {
          // Set NIC
          dgram.set_nic (mcast_nic);

          // Set TTL
#if defined (ACE_WIN32)
          // MS Windows expects an int.
          int mcast_ttl_optval = ACE_OS::atoi (mcast_ttl);
#else
          // Apparently all other platforms expect an unsigned char
          // (0-255).
          unsigned char mcast_ttl_optval = ACE_OS::atoi (mcast_ttl);
#endif  /* ACE_WIN32 */
          if (dgram.ACE_SOCK::set_option (
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
          iovp[2].iov_len  = ACE_OS::strlen (service_name) + 1;

          // Send the multicast.
          result = dgram.send (iovp,
                               iovcnt,
                               multicast_addr);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\nsent multicast request.")));

          // Check for errors.
          if (result == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("error sending IIOP multicast")));
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("\n%s; Sent multicast.")
                            ACE_TEXT ("# of bytes sent is %d.\n"),
                            __FILE__,
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
                  ACE_ERROR ((LM_ERROR,
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
                      ACE_ERROR ((LM_ERROR,
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
                      if (ior_len > TAO_DEFAULT_IOR_SIZE)
                        {
                          buf = CORBA::string_alloc (ior_len);
                          if (buf == 0)
                            {
                              ACE_ERROR ((LM_ERROR,
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
                            ACE_ERROR ((LM_ERROR,
                                        ACE_TEXT ( "%p\n"),
                                        ACE_TEXT ("error reading ior")));
                          else if (TAO_debug_level > 0)
                            ACE_DEBUG ((LM_DEBUG,
                                        ACE_TEXT ("%s: service resolved to IOR <%s>\n"),
                                        __FILE__,
                                        buf));
                        }
                    }
                }
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
TAO_MCAST_Parser::assign_to_variables (const char * &mcast_name)
{
  /*
   * The format now is "multicast_address:port:nicaddress:ttl/object_key"
   */
  ACE_CString mcast_name_cstring (mcast_name);

  int pos_colon1 = mcast_name_cstring.find (':', 0);

  if (pos_colon1 == 0)
    {
          const char *default_addr = ACE_DEFAULT_MULTICAST_ADDR;
          this->mcast_address_ = CORBA::string_alloc (ACE_OS::strlen (default_addr));
          this->mcast_address_ = default_addr;

    }
  else
    {
      this->mcast_address_ = CORBA::string_alloc (pos_colon1);
      this->mcast_address_ =
        mcast_name_cstring.substring (0,
                                      pos_colon1).c_str ();
    }
  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon1 + 1,
                                  mcast_name_cstring.length() -
                                  pos_colon1);

  int pos_colon2 = mcast_name_cstring.find (':', 0);

  if (pos_colon2 == 0)
    {
      /*
       * If the port is not specified, use the default.
       * The default multicast port is the same as the default port
       * no. for Naming_Service, for now. But for other services,
       * check and modify the default values as needed.
       */
      char default_port[33];

      int trial_port = TAO_DEFAULT_NAME_SERVER_REQUEST_PORT;

      if (mcast_name_cstring.find ("InterfaceRepository") !=
          ACE_CString::npos)
        {
          trial_port = TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT;
        }
      else if (mcast_name_cstring.find ("ImplRepoService") !=
               ACE_CString::npos)
        {
           trial_port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;
        }
      else if (mcast_name_cstring.find ("TradingService") !=
               ACE_CString::npos)
        {
           trial_port = TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT;
        }


      ACE_OS_String::itoa (trial_port, default_port, 10);

      this->mcast_port_ =
        CORBA::string_alloc (ACE_OS::strlen ((const char *) default_port));
      this->mcast_port_ = (const char *) default_port;
    }
  else
    {
      this->mcast_port_ = CORBA::string_alloc (pos_colon2);
      this->mcast_port_ = mcast_name_cstring.substring (0,
                                                        pos_colon2).c_str ();
    }

  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon2 + 1,
                                  mcast_name_cstring.length() - pos_colon2);


  int pos_colon3 = mcast_name_cstring.find (':', 0);

  this->mcast_nic_ = CORBA::string_alloc (pos_colon3);
  this->mcast_nic_ =
    mcast_name_cstring.substring (0,
                                  pos_colon3).c_str ();

  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon3 + 1,
                                  mcast_name_cstring.length() - pos_colon3);

  int pos_colon4 = mcast_name_cstring.find ('/', 0);

  if (pos_colon4 == 0)
    {
      // And, the default TTL to be 1
          const char *default_ttl = "1";
          this->mcast_ttl_ = CORBA::string_alloc (ACE_OS::strlen (default_ttl));
      this->mcast_ttl_ = default_ttl;
    }
  else
    {
      this->mcast_ttl_ = CORBA::string_alloc (pos_colon4);
      this->mcast_ttl_ =
        mcast_name_cstring.substring (0,
                                      pos_colon4).c_str ();
    }
  mcast_name_cstring =
    mcast_name_cstring.substring (pos_colon4,
                                  mcast_name_cstring.length() - pos_colon4);

  this->service_name_ =
    CORBA::string_alloc (mcast_name_cstring.length ());

  this->service_name_ =
    mcast_name_cstring.substring (1,
                                  mcast_name_cstring.length()
                                  -1).c_str ();
}

ACE_STATIC_SVC_DEFINE (TAO_MCAST_Parser,
                       ACE_TEXT ("MCAST_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_MCAST_Parser),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_MCAST_Parser)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

// $Id$

#include "IOR_Multicast.h"

#include "tao/debug.h"

#include "ace/SOCK_Connector.h"

ACE_RCSID(orbsvcs, IOR_Multicast, "$Id$")

ACE_HANDLE
TAO_IOR_Multicast::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

TAO_IOR_Multicast::TAO_IOR_Multicast (void)
  : service_id_ ((TAO_Service_ID) 0),
    ior_ (0),
    mcast_nic_ (0)
{
}

TAO_IOR_Multicast::TAO_IOR_Multicast (const char *ior,
                                      u_short port,
                                      const char *mcast_addr,
                                      TAO_Service_ID service_id)
  : mcast_nic_ (0)
{
  if (this->init (ior,
                  port,
                  mcast_addr,
                  service_id) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("TAO_IOR_Multicast")));
}

// destructor

TAO_IOR_Multicast::~TAO_IOR_Multicast (void)
{
  this->mcast_dgram_.unsubscribe ();
}

int
TAO_IOR_Multicast::init (const char *ior,
                         u_short port,
                         const char *mcast_addr,
                         TAO_Service_ID service_id)
{
  if (this->mcast_addr_.set (port,
                             mcast_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "set"),
                      -1);

  return common_init (ior, service_id);
}

int
TAO_IOR_Multicast::init (const char *ior,
                         const char *mcast_addr,
                         TAO_Service_ID service_id)
{
  // Look for a '@' incase a nic is specified.
  this->mcast_nic_ = ACE_OS::strchr (mcast_addr, '@');

  CORBA::String_var actual_mcast_addr;
  CORBA::ULong length_addr;

  if (this->mcast_nic_ != 0)
    {
      // i.e. a nic name has been specified
      length_addr = this->mcast_nic_ - mcast_addr + 1;
      actual_mcast_addr = CORBA::string_alloc (length_addr);

      ACE_OS::strncpy (actual_mcast_addr,
                       mcast_addr,
                       length_addr - 1);

      actual_mcast_addr[length_addr - 1] = '\0';

      /// Save for use later.
      this->mcast_nic_ = this->mcast_nic_ + 1;
    }
  else
    {
      actual_mcast_addr =
        CORBA::string_alloc (ACE_OS::strlen (mcast_addr));

      actual_mcast_addr = mcast_addr;
    }

  if (this->mcast_addr_.set (actual_mcast_addr.in ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "set"),
                      -1);

  return common_init (ior, service_id);
}

int
TAO_IOR_Multicast::common_init (const char *ior,
                                TAO_Service_ID service_id)
{
  this->service_id_ = service_id;
  this->ior_ = ior;

  if (this->response_addr_.set ((u_short) 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "set"),
                      -1);
  else if (this->response_.open (this->response_addr_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "set"),
                        -1);
    }

  // Use ACE_SOCK_Dgram_Mcast factory to subscribe to multicast group.
  if (this->mcast_nic_ != 0)
    {
      if (this->mcast_dgram_.subscribe (this->mcast_addr_,
                                        1,
                                        this->mcast_nic_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "subscribe"),
                          -1);
    }
  else
    {
      if (this->mcast_dgram_.subscribe (this->mcast_addr_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "subscribe"),
                          -1);
    }
  return 0;
}

int
TAO_IOR_Multicast::handle_timeout (const ACE_Time_Value &,
                                   const void *)
{
  return 0;
}

int
TAO_IOR_Multicast::handle_input (ACE_HANDLE)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Entered TAO_IOR_Multicast::handle_input\n"));

  // The length of the service name string that follows.
  CORBA::Short header;
  // Port to which to reply.
  ACE_UINT16 remote_port;
  // Name of the service for which the client is looking.
  char service_name[BUFSIZ];

  ACE_INET_Addr remote_addr;

  // Take a peek at the header to find out how long is the service
  // name string we should receive.
  ssize_t n = this->mcast_dgram_.recv (&header,
                                       sizeof (header),
				       remote_addr,
                                       MSG_PEEK);
  if (n <= 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO_IOR_Multicast::handle_input - peek %d\n",
		       n),
		      0);
  else if (ACE_NTOHS (header) <= 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Header value < 1\n"),
                      0);

  // Receive full client multicast request.
  const int iovcnt = 3;
  iovec iov[iovcnt];

  iov[0].iov_base = (char *) &header;
  iov[0].iov_len  = sizeof (header);
  iov[1].iov_base = (char *) &remote_port;
  iov[1].iov_len  = sizeof (ACE_UINT16);
  iov[2].iov_base = (char *) service_name;
  iov[2].iov_len  = ACE_NTOHS (header);

  // Read the iovec.
  n = this->mcast_dgram_.recv (iov,
                               iovcnt,
                               remote_addr);
  if (n <= 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO_IOR_Multicast::handle_input recv = %d\n",
		       n),
		      0);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) Received multicast.\n"
		"Service Name received : %s\n"
		"Port received : %u\n",
		service_name,
		ACE_NTOHS (remote_port)));

  // Our reply data.
  ACE_CString ior (this->ior_);

  if (ACE_OS::strcmp (service_name,
                      "NameService") != 0
      && ACE_OS::strcmp (service_name,
                         "TradingService") != 0
      && ACE_OS::strcmp (service_name,
                         "ImplRepoService") != 0
      && ACE_OS::strcmp (service_name,
                         "InterfaceRepository") != 0
      && ACE_OS::strcmp (service_name,
                         "MCASTServer") != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unknown service requested.\n"),
                        0);
    }

  // Reply to the multicast message.
  ACE_SOCK_Connector connector;
  ACE_INET_Addr peer_addr (ACE_NTOHS (remote_port),
			   remote_addr.get_host_addr ());
  ACE_SOCK_Stream stream;

  // Connect.
  if (connector.connect (stream,
                         peer_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "IOR_Multicast::connect failed\n"),
		      0);
  // Send the IOR back to the client.  (Send iovec, which contains ior
  // length as the first element, and ior itself as the second.)

  // Length of ior to be sent.
  CORBA::Short data_len =
    ACE_HTONS (ior.length () + 1);

  // Vector to be sent.
  const int cnt = 2;
  iovec iovp[cnt];

  // The length of ior to be sent.
  iovp[0].iov_base = (char *) &data_len;
  iovp[0].iov_len  = sizeof (CORBA::Short);

  // The ior.
  iovp[1].iov_base = ACE_const_cast (char*, ior.c_str ());
  iovp[1].iov_len  = ior.length () + 1;

  ssize_t result = stream.sendv_n (iovp,
                                   cnt);
  // Close the stream.
  stream.close ();

  // Check for error.
  if (result == -1)
    return 0;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) ior_: <%s>\n"
                "sent to %s:%u.\n"
                "result from send = %d\n",
                ior.c_str (),
                peer_addr.get_host_name (),
		peer_addr.get_port_number (),
                result));
  return 0;
}

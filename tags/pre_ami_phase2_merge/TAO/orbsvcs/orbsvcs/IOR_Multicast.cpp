// $Id$

#include "IOR_Multicast.h"

#include "ace/SOCK_Connector.h"

ACE_RCSID(orbsvcs, IOR_Multicast, "$Id$")

ACE_HANDLE
TAO_IOR_Multicast::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

TAO_IOR_Multicast::TAO_IOR_Multicast (void)
  : service_id_ ((TAO_Service_ID) 0),
    ior_ (0)
{
}

TAO_IOR_Multicast::TAO_IOR_Multicast (const char *ior,
                                      u_short port,
                                      const char *mcast_addr,
                                      TAO_Service_ID service_id)
{
  if (this->init (ior,
                  port,
                  mcast_addr,
                  service_id) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("TAO_IOR_Multicast")));
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
  this->service_id_ = service_id;
  this->mcast_addr_.set (port, mcast_addr);
  this->ior_ = ior;
  this->response_addr_.set ((u_short) 0);
  this->response_.open (this->response_addr_);

  // Use ACE_SOCK_Dgram_Mcast factory to subscribe to multicast group.
  if (this->mcast_dgram_.subscribe (this->mcast_addr_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "subscribe"),
                      -1);

  // Initialise the IOR_LookupTable.

  //  this->ior_lookup_table_.add_ior ("ObjectID1","iioploc://tango.cs.wustl.edu:9999/ObjectID1");

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
  // The header has the length of the data following it.
  CORBA::Short header;
  ACE_UINT16 remote_port;

  char *name;
  ACE_NEW_RETURN (name,
                  char[BUFSIZ],
                  0);

  CORBA::String_var service_name (name);

  ACE_INET_Addr remote_addr;

  // Take a peek at the header and get the length of data in bytes.
  ssize_t n = this->mcast_dgram_.recv (&header,
                                       sizeof(header),
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

  // Construct an iovec to receive.
  const int iovcnt = 3;
  iovec iov[iovcnt];

  iov[0].iov_base = (char *) &header;
  iov[0].iov_len  = sizeof(header);
  iov[1].iov_base = (char *) &remote_port;
  iov[1].iov_len  = sizeof (ACE_UINT16);
  iov[2].iov_base = (char *) service_name;
  iov[2].iov_len  = ACE_NTOHS (header) - sizeof (ACE_UINT16);

  // Read the iovec.
  n = this->mcast_dgram_.recv (iov,
                               iovcnt,
                               remote_addr);
  if (n <= 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO_IOR_Multicast::handle_input recv = %d\n",
		       n),
		      0);
  // Null terminate.
  service_name [ACE_NTOHS (header) - sizeof (ACE_UINT16)] = 0;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) Received multicast.\n"
		"Service Name received : %s\n"
		"Port received : %u\n",
		service_name.in (),
		ACE_NTOHS (remote_port)));
  ACE_CString ior(this->ior_);

  if (ACE_OS::strcmp (service_name.in (),
                      "NameService") != 0
      && ACE_OS::strcmp (service_name.in (),
                         "TradingService") != 0)
    {
      // The client has requested an IOR other than for the
      // Name/Trading Service.  Lookup the table for the IOR. The call
      // to find_ior will fill the ior for us if the service name is
      // found in the table.

      ACE_CString service (service_name.in ());

      if (this->ior_lookup_table_.find_ior (service, ior) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "IOR_Multicast::find failed.\n"),
			  0);
    }

  ACE_SOCK_Connector connector;
  ACE_INET_Addr peer_addr (ACE_NTOHS (remote_port),
			   remote_addr.get_host_name ());
  ACE_SOCK_Stream stream;

  // Connect.
  if (connector.connect (stream,
                         peer_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "IOR_Multicast::connect failed\n"),
		      0);
  // Send the IOR back to the client.
  ssize_t result = stream.send_n (ior.c_str (),
                                  ACE_OS::strlen (ior.c_str ()) + 1);
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

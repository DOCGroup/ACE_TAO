//
// $Id$
//

#include "RMCast_UDP_Sender.h"
#include "RMCast.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_UDP_Sender.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_UDP_Sender, "$Id$")

ACE_RMCast_UDP_Sender::ACE_RMCast_UDP_Sender (const ACE_INET_Addr &mcast_addr)
  : ACE_RMCast_Module ()
  , mcast_addr_ (mcast_addr)
{
}

ACE_RMCast_UDP_Sender::~ACE_RMCast_UDP_Sender (void)
{
}

int
ACE_RMCast_UDP_Sender::open (void)
{
  return this->dgram_.open (ACE_Addr::sap_any);
}

int
ACE_RMCast_UDP_Sender::close (void)
{
  return this->dgram_.close ();
}

int
ACE_RMCast_UDP_Sender::put_data (ACE_RMCast::Data &data)
{
  // The first message block contains the header
  // @@ TODO: We could keep the header pre-initialized, and only
  // update the portions that do change...
  ACE_UINT32 tmp;
  char header[1 + 3 * sizeof(ACE_UINT32)];
  header[0] = ACE_RMCast::MT_DATA;

  tmp = ACE_HTONL (data.sequence_number);
  ACE_OS::memcpy (header + 1,
                  &tmp, sizeof(ACE_UINT32));
  tmp = ACE_HTONL (data.total_size);
  ACE_OS::memcpy (header + 1 + sizeof(ACE_UINT32),
                  &tmp, sizeof(ACE_UINT32));
  tmp = ACE_HTONL (data.fragment_offset);
  ACE_OS::memcpy (header + 1 + 2 * sizeof(ACE_UINT32),
                  &tmp, sizeof(ACE_UINT32));

  iovec iov[IOV_MAX];
  int iovcnt = 1;

  iov[0].iov_base = header;
  iov[0].iov_len = sizeof(header);

  ACE_Message_Block *mb = data.payload;

  for (const ACE_Message_Block *i = mb; i != 0; i = i->cont ())
    {
      iov[iovcnt].iov_base = i->rd_ptr ();
      iov[iovcnt].iov_len = i->length ();
      iovcnt++;
      if (iovcnt >= IOV_MAX)
        return -1;
    }

  ACE_Time_Value tv (0, 10000);
  ACE_OS::sleep (tv);
  if (this->dgram_.send (iov, iovcnt,
                         this->mcast_addr_) == -1)
    return -1;

#if 0
  ACE_HEX_DUMP ((LM_DEBUG,
                 (char*)iov[0].iov_base, iov[0].iov_len, "Sending"));
#endif

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

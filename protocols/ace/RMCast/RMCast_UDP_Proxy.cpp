// $Id$

#include "RMCast_UDP_Proxy.h"
#include "RMCast_Module.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_UDP_Proxy.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_UDP_Proxy, "$Id$")

ACE_RMCast_UDP_Proxy::ACE_RMCast_UDP_Proxy (ACE_RMCast_IO_UDP *io_udp,
                                            const ACE_INET_Addr &addr,
                                            ACE_RMCast_Module *module)
  : io_udp_ (io_udp)
  , peer_addr_ (addr)
  , module_ (module)
{
}

ACE_RMCast_UDP_Proxy::~ACE_RMCast_UDP_Proxy (void)
{
}

int
ACE_RMCast_UDP_Proxy::receive_message (char *buffer, size_t size)
{
  int type = buffer[0];

  // @@ What should we do with invalid messages like this?
  //
  if (type < 0 || type >= ACE_RMCast::MT_LAST)
    return 0;

  if (type == ACE_RMCast::MT_POLL)
    {
      ACE_RMCast::Poll poll;
      return this->module ()->poll (poll);
    }

  else if (type == ACE_RMCast::MT_ACK_JOIN)
    {
      ACE_RMCast::Ack_Join ack_join;
      const size_t header_size = 1 + sizeof(ACE_UINT32);
      if (size < header_size)
        {
          // The message is too small
          return 0;
        }

      ACE_UINT32 tmp;

      ACE_OS::memcpy (&tmp, buffer + 1,
                      sizeof(tmp));
      ack_join.next_sequence_number = ACE_NTOHL (tmp);
      return this->module ()->ack_join (ack_join);
    }

  else if (type == ACE_RMCast::MT_ACK_LEAVE)
    {
      ACE_RMCast::Ack_Leave ack_leave;
      return this->module ()->ack_leave (ack_leave);
    }

  else if (type == ACE_RMCast::MT_DATA)
    {
      ACE_RMCast::Data data;
      const size_t header_size = 1 + 3 * sizeof(ACE_UINT32);
      if (size < header_size)
        {
          // The message is too small
          return 0;
        }

      ACE_UINT32 tmp;

      ACE_OS::memcpy (&tmp, buffer + 1,
                      sizeof(tmp));
      data.sequence_number = ACE_NTOHL (tmp);

      ACE_OS::memcpy (&tmp, buffer + 1 + sizeof(tmp),
                      sizeof(tmp));
      data.total_size = ACE_NTOHL (tmp);

      ACE_OS::memcpy (&tmp, buffer + 1 + 2 * sizeof(tmp),
                      sizeof(tmp));
      data.fragment_offset = ACE_NTOHL (tmp);

      // Pass it up the module...
      ACE_Message_Block *mb;
      ACE_NEW_RETURN (mb, ACE_Message_Block, -1);
      mb->size (size - header_size);
      mb->copy (buffer + header_size, size - header_size);

      data.payload = mb;
      return this->module ()->data (data);
    }

  else if (type == ACE_RMCast::MT_JOIN)
    {
      ACE_RMCast::Join join;
      return this->module ()->join (join);
    }
      
  else if (type == ACE_RMCast::MT_LEAVE)
    {
      ACE_RMCast::Leave leave;
      return this->module ()->leave (leave);
    }
      
  else if (type == ACE_RMCast::MT_ACK)
    {
      ACE_RMCast::Ack ack;

      const size_t header_size = 1 + sizeof(ACE_UINT32);
      if (size < header_size)
        {
          // The message is too small
          return 0;
        }

      ACE_UINT32 tmp;

      ACE_OS::memcpy (&tmp, buffer + 1,
                      sizeof(tmp));
      ack.highest_in_sequence = ACE_NTOHL (tmp);
      ACE_OS::memcpy (&tmp, buffer + 1 + sizeof(ACE_UINT32),
                      sizeof(tmp));
      ack.highest_received = ACE_NTOHL (tmp);

      return this->module ()->ack (ack);
    }
      
  return 0;
}

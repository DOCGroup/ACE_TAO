// $Id$

#include "RMCast_Sender_Proxy_Best_Effort.h"
#include "RMCast_Module.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Sender_Proxy_Best_Effort.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Sender_Proxy_Best_Effort, "$Id$")

ACE_RMCast_Sender_Proxy_Best_Effort::
    ACE_RMCast_Sender_Proxy_Best_Effort (ACE_RMCast_Module *module)
  : ACE_RMCast_Sender_Proxy (module)
{
}

ACE_RMCast_Sender_Proxy_Best_Effort::
    ~ACE_RMCast_Sender_Proxy_Best_Effort (void)
{
}

int
ACE_RMCast_Sender_Proxy_Best_Effort::receive_message (char *buffer,
                                                      size_t size)
{
  int type = buffer[0];

  // All control messages are ignored...
  if (type != ACE_RMCast::MT_DATA)
    return 0;

  // @@ Push the event through the stack
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "Proxy(%x) - received data\n", long(this)));
  ACE_HEX_DUMP ((LM_DEBUG, buffer, header, "Proxy"));
#endif

  const size_t header_size = 1 + 3 * sizeof(ACE_UINT32);
  if (size < header_size)
    {
      // The message is too small
      return 0;
    }

  ACE_UINT32 tmp;
  
  ACE_RMCast::Data data;

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
  return this->module ()->put_data (data);
}

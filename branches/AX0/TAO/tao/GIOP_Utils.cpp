// $Id$

#include "tao/GIOP_Utils.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "Transport.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Utils.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Utils, "$Id$")

int
TAO_GIOP_Utils::
  read_bytes_input (TAO_Transport *transport,
                    TAO_InputCDR &input,
                    CORBA::ULong read_size,
                    ACE_Time_Value *value)
{
  // Grow the size of CDR stream
  if (input.grow (read_size) == -1)
    return -1;

  // Read until all the header is received.  There should be no
  // problems with locking, the header is only a few bytes so they
  // should all be available on the socket, otherwise there is a
  // problem with the underlying transport, in which case we have more
  // problems than just this small loop.
  char *buf = input.rd_ptr ();
  ssize_t n = 0;

  for (int t = read_size;
       t != 0;
       t -= n)
    {
      n = transport->recv (buf, t, value);
      if (n == -1)
        return -1;
      else if (n == 0) // @@ TODO && errno != EWOULDBLOCK)
        return -1;
      buf += n;
    }

  return 1;
}


ssize_t
TAO_GIOP_Utils::read_buffer (TAO_Transport *transport,
                             char *buf,
                             size_t len,
                             ACE_Time_Value *max_wait_time)
{
  ssize_t bytes_read = transport->recv (buf, len, max_wait_time);

  if (bytes_read <= 0 && TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t|%N|%l) - %p,\n")
                ACE_TEXT ("              transport = %d, ")
                ACE_TEXT ("bytes = %d, len = %d\n"),
                ACE_TEXT ("read_buffer"),
                transport->id (),
                bytes_read,
                len));

  if (bytes_read == -1 && errno == ECONNRESET)
    {
      // @@ Is this OK??

      // We got a connection reset (TCP RSET) from the other side,
      // i.e., they didn't initiate a proper shutdown.
      //
      // Make it look like things are OK to the upper layer.
      bytes_read = 0;
      errno = 0;
    }

  return bytes_read;
}

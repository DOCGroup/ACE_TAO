// $Id$

#ifndef COMPRESSOR_H
#define COMPRESSOR_h

#include "Protocol_Task.h"

/* A reallly dumb compression object.  (It actually adds 3 bytes to
   every message block.)
*/
class Compressor : public Protocol_Task
{
public:

  typedef Protocol_Task inherited;

  Compressor (void);

  ~Compressor (void);

protected:

  // This is called when the compressor is on the downstream side.
  // We'll take the message, compress it and move it along to the next
  // module.
  int send (ACE_Message_Block *message,
            ACE_Time_Value *timeout);

  // This one is called on the upstream side.  No surprise: we
  // decompress the data and send it on up the stream.
  int recv (ACE_Message_Block *message,
            ACE_Time_Value *timeout);
};

#endif /* COMPRESSOR_H */

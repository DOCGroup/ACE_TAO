// $Id$

#ifndef CRYPT_H
#define CRYPT_h

#include "Protocol_Task.h"

/* An interface (adaptor) between your favorite encryption method and
   an ACE_Stream.
*/
class Crypt : public Protocol_Task
{
public:

  typedef Protocol_Task inherited;

  // Again we have the option of multiple threads and again I
  // regret tempting folks to use it.
  Crypt (int thr_count = 0);

  ~Crypt (void);

protected:

  // Moving downstream will encrypt the data
  int send (ACE_Message_Block *message,
            ACE_Time_Value *timeout);

  // And moving upstream will decrypt it.
  int recv (ACE_Message_Block *message,
            ACE_Time_Value *timeout);
};

#endif /* CRYPT_H */

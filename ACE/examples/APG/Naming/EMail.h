#ifndef EMAIL_H
#define EMAIL_H

#include "ace/Log_Msg.h"

class EMail
{
public:
  EMail () = default;

  int send (char const *to,
            char const *from,
            char const *subject,
            char const *message) const
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("To:\t%C\n"), to));
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("From:\t%C\n"), from));
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Subject:\t%C\n"), subject));
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\n%C\n"), message));

    return 0;
  }
};

#endif /* EMAIL_H */

// $Id$

#ifndef EMAIL_H
#define EMAIL_H

#include "ace/Log_Msg.h"

class EMail
  {
  public:
    EMail()
    { }

    int send (const char *to,
              const char *from,
              const char *subject,
              const char *message)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("To:\t%s\n"), to));
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("From:\t%s\n"), from));
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Subject:\t%s\n"), subject));
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\n%s\n"), message));

      return 0;
    }
  };

#endif /* EMAIL_H */

// $Id$
#include "Messenger_i.h"

CORBA::Boolean
Messenger_i::send_message (const char *user_name, const char *subject,
                           char *& message)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Message from: %s\nSubject: %s\nMessage: %s"
                                  "\n"), user_name, subject, message));
  const char *reply = "A reply from the server.";
  message = CORBA::string_dup (reply);
  if (++this->count_ == 6)
    {
      this->orb_->shutdown (0);
    }
  return true;
}

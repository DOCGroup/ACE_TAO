#include "Messenger_i.h"

CORBA::Boolean
Messenger_i::send_message (char const *user_name, char const *subject,
                           char *& message)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message from: %C\nSubject: %C\nMessage: %C\n"),
              user_name, subject, message));
  char const *reply = "A reply from the server.";
  CORBA::string_free (message);
  message = CORBA::string_dup (reply);
  if (++this->count_ == 6)
    {
      this->orb_->shutdown (false);
    }
  return true;
}

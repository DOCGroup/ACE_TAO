/* -*- C++ -*- */
#include "ace/OS_NS_string.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SV_Message.h"

#define MSGSZ 128
#define SRV_KEY ACE_DEFAULT_SHM_KEY
#define SRV_ID 1

class Message_Data
{
public:
  explicit Message_Data (long p = -1,
                const char user[] = "",
                const char text[] = ""): pid_ (p)
  {
    ACE_OS::strncpy (this->username_, user, 9);
    ACE_OS::strncpy (this->mtext_, text, MSGSZ);
  }

  long pid () const { return this->pid_; }
  void pid (long p) { this->pid_ = p; }
  const char *user () const { return this->username_; }
  void user (const char user[]) { ACE_OS::strncpy (this->username_, user, 9); }
  const char *text () const { return this->mtext_; }
  void text (const char text[]) { ACE_OS::strncpy (this->mtext_, text, MSGSZ); }
  int length () const { return sizeof *this - sizeof this->mtext_ + ACE_OS::strlen (this->mtext_) + 1; }

protected:
  long  pid_;
  char  username_[9];
  char  mtext_[MSGSZ];
};

class Message_Block : public ACE_SV_Message, public Message_Data
{
  // = TITLE
  //   Stores message content.
  // = DESCRIPTION
  //   This may not be 100 percent portable on all C++ compilers since
  //   it relies on inheritance to be "concatenation."
  //
public:
  explicit Message_Block (long t,
                 long p = 0,
                 const char login[] = "",
                 const char message[] = "")
  : ACE_SV_Message (t),
    Message_Data (p, login, message)
  {}
};


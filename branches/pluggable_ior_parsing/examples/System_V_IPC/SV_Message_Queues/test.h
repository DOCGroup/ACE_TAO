/* -*- C++ -*- */
// $Id$

#include "ace/OS.h"

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
  Message_Data (long p = -1,
                const char user[] = "",
                const char text[] = ""): pid_ (p)
  {
    ::strncpy (this->username_, user, 9);
    ::strncpy (this->mtext_, text, MSGSZ);
  }

  long pid (void) { return this->pid_; }
  void pid (long p) { this->pid_ = p; }
  char *user (void) { return this->username_; }
  void user (char user[]) { ::strncpy (this->username_, user, 9); }
  char *text (void) { return this->mtext_; }
  void text (char text[]) { ::strncpy (this->mtext_, text, MSGSZ); }
  int length (void) { return sizeof *this - sizeof this->mtext_ + ::strlen (this->mtext_) + 1; }

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
  Message_Block (long t,
                 long p = 0,
                 const char login[] = "",
                 const char message[] = "")
  : ACE_SV_Message (t),
    Message_Data (p, login, message)
  {}
};


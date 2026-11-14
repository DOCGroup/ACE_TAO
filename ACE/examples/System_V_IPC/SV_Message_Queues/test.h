/* -*- C++ -*- */
#include <cstring>
#include "ace/SV_Message.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define MSGSZ 128
#define SRV_KEY ACE_DEFAULT_SHM_KEY
#define SRV_ID 1

class Message_Data
{
public:
  explicit Message_Data (long p = -1,
                         char const user[] = "",
                         char const text[] = "")
    : pid_ (p)
  {
    // These fixed buffers are the System V message layout.
    std::strncpy (this->username_, user, sizeof this->username_);
    this->username_[sizeof this->username_ - 1] = '\0';
    std::strncpy (this->mtext_, text, sizeof this->mtext_);
    this->mtext_[sizeof this->mtext_ - 1] = '\0';
  }

  long pid () const { return this->pid_; }
  void pid (long p) { this->pid_ = p; }
  char const *user () const { return this->username_; }
  void user (char const user[])
  {
    std::strncpy (this->username_, user, sizeof this->username_);
    this->username_[sizeof this->username_ - 1] = '\0';
  }
  char const *text () const { return this->mtext_; }
  void text (char const text[])
  {
    std::strncpy (this->mtext_, text, sizeof this->mtext_);
    this->mtext_[sizeof this->mtext_ - 1] = '\0';
  }
  int length () const { return sizeof *this - sizeof this->mtext_ + std::strlen (this->mtext_) + 1; }

protected:
  long pid_;
  char username_[9];
  char mtext_[MSGSZ];
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
                          char const login[] = "",
                          char const message[] = "")
    : ACE_SV_Message (t),
      Message_Data (p, login, message)
  {}
};


// -*- C++ -*-

//=============================================================================
/**
 * @file Message_Queue_Test_Ex.h
 *
 * $Id$
 *
 * Define class needed for generating templates. IBM C++ requires this
 * to be in its own file for auto template instantiation.
 *
 * @author Michael Vitlo <mvitalo@sprynet.com>
 * @author Irfan Pyarali <irfan@cs.wustl.edu>
 * @author David L. Levine <levine@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_TESTS_MESSAGE_QUEUE_TEST_EX_H
#define ACE_TESTS_MESSAGE_QUEUE_TEST_EX_H

#include "ace/OS.h"

// User-defined class used for queue data.
class User_Class
{
public:
  User_Class (const char inputMsg[])
    : message_ (0)
  {
    ACE_NEW (this->message_, char[ACE_OS::strlen (inputMsg) + 1]);
    ACE_OS::strcpy (this->message_, inputMsg);
  }

  ~User_Class (void) { delete [] this->message_; }
private:
  char *message_;
};

#endif /* ACE_TESTS_MESSAGE_QUEUE_TEST_EX_H */

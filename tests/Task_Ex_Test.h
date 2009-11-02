// -*- C++ -*-

//=============================================================================
/**
 * @file Task_Ex_Test.h
 *
 * $Id$
 *
 * Define class needed for generating templates. IBM C++ requires this
 * to be in its own file for auto template instantiation.
 *
 * @author Kobi Cohen-Arazi <kobi-co@barak-online.net>
 */
//=============================================================================

#ifndef ACE_TESTS_TASK_EX_TEST_H
#define ACE_TESTS_TASK_EX_TEST_H

#include "ace/Basic_Types.h"

class User_Defined_Msg
{
public:
  /// c'tor sets the msg id
  User_Defined_Msg (ACE_INT32 id) : msg_id_ (id)
    {}
  /// accessors to msg_id_
  ACE_INT32 msg_id () const {return msg_id_;}
private:
  /// keep the msg id here
  ACE_INT32 msg_id_;
};

#endif /* ACE_TESTS_TASK_EX_TEST_H */


//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_MSAE_TEST_I_H
#define TAO_MSAE_TEST_I_H

#include "testS.h"

/**
 * @class Test_i
 *
 * @brief Test implementation
 *
 * Implements the Test interface in test.idl
 */
class Test_i : public POA_A::Test
{
public:
  /// ctor
  Test_i (CORBA::ORB_ptr orb,
          A::AMI_TestHandler_ptr rh,
          CORBA::ULong max_count = 10,
          A::RunMode mode = A::RM_SLAVE);

  void set_opponent (A::Test_ptr opp);

  virtual void request (
    ::A::RequestMode mode,
    ::CORBA::ULong & counter,
    ::A::FollowUp_out follow_up);

  virtual void report (
    const char * msg);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;

  A::AMI_TestHandler_var rh_;

  A::Test_var opponent_;

  CORBA::ULong max_count_;

  A::RunMode mode_;

  time_t seed_;
};

class TestHandler
  : public ACE_Event_Handler
{
public:
  TestHandler (CORBA::ORB_ptr orb,
               CORBA::ULong max_count,
               A::RunMode mode);

  void set_counter (CORBA::ULong counter);

  void set_opponent (A::Test_ptr opp);

  void set_reply_handler (A::AMI_TestHandler_ptr rh);

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);

  void handle_followup (A::FollowUp fup, CORBA::ULong counter);

  void start ();

  CORBA::ORB_ptr orb () { return this->orb_.in (); }
  A::Test_ptr opponent () { return this->opponent_.in (); }
  CORBA::ULong max_count () { return this->max_count_; }
  A::RunMode mode () { return this->mode_; }

private:
  CORBA::ORB_var orb_;
  A::Test_var opponent_;
  A::AMI_TestHandler_var rh_;
  CORBA::ULong max_count_;
  A::RunMode mode_;
  CORBA::ULong counter_;
  time_t seed_;
};

class Test_Reply_i : public POA_A::AMI_TestHandler
{
public:
  Test_Reply_i (CORBA::ORB_ptr orb,
      CORBA::ULong max_count,
      A::RunMode mode);

  virtual void request (
    ::CORBA::ULong counter,
    ::A::FollowUp follow_up);

  virtual void request_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  virtual void report (
    void);

  virtual void report_excep (
    ::Messaging::ExceptionHolder * excep_holder);

  TestHandler& test_handler () { return this->evh_; }

private:
  TestHandler evh_;
};



#endif /* TAO_MSAE_TEST_I_H */


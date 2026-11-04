
//=============================================================================
/**
 *  @file   test_i.h
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
          A::RunMode mode = A::RM_SLAVE);
  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;

  void set_opponent (A::Test_ptr opp);

  void request (
    ::A::RequestMode mode,
    ::CORBA::ULong & counter,
    ::A::FollowUp_out follow_up) override;

  void report (char const *msg) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  A::AMI_TestHandler_var rh_;

  A::Test_var opponent_;

  A::RunMode const mode_;

  unsigned int seed_;
};

class TestHandler
  : public ACE_Event_Handler
{
public:
  TestHandler (CORBA::ORB_ptr orb,
               CORBA::ULong max_count,
               A::RunMode mode);
  TestHandler (TestHandler const &) = delete;
  TestHandler (TestHandler &&) = delete;
  TestHandler &operator= (TestHandler const &) = delete;
  TestHandler &operator= (TestHandler &&) = delete;

  void set_counter (CORBA::ULong counter);

  void set_opponent (A::Test_ptr opp);

  void set_reply_handler (A::AMI_TestHandler_ptr rh);

  int handle_timeout (ACE_Time_Value const &tv,
                      void const *arg) override;

  int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE) override;

  void handle_followup (A::FollowUp fup, CORBA::ULong counter);

  void start ();

  CORBA::ORB_ptr orb () const { return this->orb_.in (); }
  A::Test_ptr opponent () const { return this->opponent_.in (); }
  CORBA::ULong max_count () const { return this->max_count_; }
  A::RunMode mode () const { return this->mode_; }

private:
  CORBA::ORB_var orb_;
  A::Test_var opponent_;
  A::AMI_TestHandler_var rh_;
  CORBA::ULong const max_count_;
  A::RunMode const mode_;
  CORBA::ULong counter_ {};
  unsigned int seed_;
};

class Test_Reply_i : public POA_A::AMI_TestHandler
{
public:
  Test_Reply_i (CORBA::ORB_ptr orb,
                CORBA::ULong max_count,
                A::RunMode mode);
  Test_Reply_i (Test_Reply_i const &) = delete;
  Test_Reply_i (Test_Reply_i &&) = delete;
  Test_Reply_i &operator= (Test_Reply_i const &) = delete;
  Test_Reply_i &operator= (Test_Reply_i &&) = delete;

  void request (
    ::CORBA::ULong counter,
    ::A::FollowUp follow_up) override;

  void request_excep (
    ::Messaging::ExceptionHolder *excep_holder) override;

  void report () override;

  void report_excep (
    ::Messaging::ExceptionHolder *excep_holder) override;

  TestHandler &test_handler () { return this->evh_; }

private:
  TestHandler evh_;
};


#endif /* TAO_MSAE_TEST_I_H */


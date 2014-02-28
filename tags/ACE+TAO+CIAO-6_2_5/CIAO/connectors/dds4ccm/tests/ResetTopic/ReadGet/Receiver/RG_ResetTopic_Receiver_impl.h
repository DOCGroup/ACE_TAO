// $Id$

#ifndef RG_RESETTOPIC_RECEIVER_IMPL_H_
#define RG_RESETTOPIC_RECEIVER_IMPL_H_

#include "RG_ResetTopic_ReceiverEC.h"

namespace CIAO_RG_ResetTopic_Receiver_Impl
{
  class RG_ResetTopic_Receiver_impl;

  /**
   * LastSampleChecker
   */
  class LastSampleChecker
    : public ACE_Event_Handler
  {
  public:
    LastSampleChecker (RG_ResetTopic_Receiver_impl &callback,
                       const ::CORBA::UShort &iterations);

    virtual ~LastSampleChecker (void);

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    RG_ResetTopic_Receiver_impl &callback_;
    const ::CORBA::UShort iterations_;
  };

  /**
   * RG_ResetTopic_Receiver_impl
   */
  class RG_ResetTopic_Receiver_impl
  {
  public:
    RG_ResetTopic_Receiver_impl (
      ::RG_ResetTopic::CCM_Receiver_Context_ptr ctx,
      const ::CORBA::UShort & iterations,
      const ::CORBA::UShort & keys);
    ~RG_ResetTopic_Receiver_impl (void);

    void start (const char * topic_name);
    bool check_last (void);
    void start_read (void);


    void iterations (::CORBA::UShort iterations);
    void keys (::CORBA::UShort keys);

  private:
    ::RG_ResetTopic::CCM_Receiver_Context_var ciao_context_;
    ::CORBA::UShort iterations_;
    ::CORBA::UShort keys_;
    ::CORBA::UShort expected_per_run_;

    LastSampleChecker * checker_;

    ACE_CString topic_name_;

    void set_topic_name_reader (const char * topic_name);
    void set_topic_name_getter (const char * topic_name);

    void start_reading (void);
    void start_getting (void);
    void test_exception (void);

    void check_samples (
      const char * test,
      const RG_ResetTopicSampleSeq& samples,
      const ::CORBA::UShort& expected=0);

    ACE_Reactor* reactor (void);
  };
};

#endif /* RG_RESETTOPIC_RECEIVER_IMPL_H_ */

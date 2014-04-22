// $Id$

#ifndef RG_LATEBINDING_RECEIVER_IMPL_H_
#define RG_LATEBINDING_RECEIVER_IMPL_H_

#include "RG_LateBinding_ReceiverEC.h"

namespace CIAO_RG_LateBinding_Receiver_Impl
{
  class RG_LateBinding_Receiver_impl;

  /**
   * LastSampleChecker
   */
  class LastSampleChecker
    : public ACE_Event_Handler
  {
  public:
    LastSampleChecker (RG_LateBinding_Receiver_impl &callback,
                       const ::CORBA::UShort &iterations);

    virtual ~LastSampleChecker (void);

    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    RG_LateBinding_Receiver_impl &callback_;
    const ::CORBA::UShort iterations_;

  };

  /**
   * RG_LateBinding_Receiver_impl
   */
  class RG_LateBinding_Receiver_impl
  {
  public:
    RG_LateBinding_Receiver_impl (
      ::RG_LateBinding::CCM_Receiver_Context_ptr ctx,
      const ::CORBA::UShort & iterations,
      const ::CORBA::UShort & keys);
    ~RG_LateBinding_Receiver_impl (void);

    void start (void);
    bool check_last (void);
    void start_read (void);

  private:
    ::RG_LateBinding::CCM_Receiver_Context_var ciao_context_;
    ::CORBA::UShort iterations_;
    ::CORBA::UShort keys_;

    LastSampleChecker * checker_;

    void set_topic_name_reader ();
    void set_topic_name_getter ();

    void start_reading (void);
    void start_getting (void);
    void test_exception (void);

    void check_samples (
      const char * test,
      const RG_LateBindingTestSeq& samples,
      const ::CORBA::UShort& expected=0);

    ACE_Reactor* reactor (void);
  };
};

#endif /* RG_LATEBINDING_RECEIVER_IMPL_H_ */

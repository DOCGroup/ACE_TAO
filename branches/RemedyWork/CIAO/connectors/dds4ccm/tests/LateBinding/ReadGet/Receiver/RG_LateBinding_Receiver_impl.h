// $Id$

#ifndef RG_LATEBINDING_RECEIVER_IMPL_H_
#define RG_LATEBINDING_RECEIVER_IMPL_H_

#include "RG_LateBinding_ReceiverEC.h"

namespace CIAO_RG_LateBinding_Receiver_Impl
{
  class RG_LateBinding_Receiver_impl;

  /**
   * Timeout_Handler
   */
  class Timeout_Handler :
    public ACE_Event_Handler
  {
  public:
    Timeout_Handler (RG_LateBinding_Receiver_impl &callback);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
  private:
    RG_LateBinding_Receiver_impl &callback_;
  };

  /**
   * RG_LateBinding_Receiver_impl
   */
  class RG_LateBinding_Receiver_impl
  {
  public:
    RG_LateBinding_Receiver_impl (
      ::RG_LateBinding::CCM_Receiver_Context_ptr ctx,
      ::CORBA::UShort expected);
    ~RG_LateBinding_Receiver_impl (void);

    void start (
      ACE_Reactor *reactor);

    void set_topic_name_reader (bool test_non_changeable=false);
    void set_topic_name_getter (bool test_non_changeable=false);

    void start_reading (void);
    void start_getting (void);
    void test_exception (void);
  private:
    ::RG_LateBinding::CCM_Receiver_Context_var ciao_context_;
    CORBA::UShort expected_;

    Timeout_Handler *to_handler_read_;
    Timeout_Handler *to_handler_get_;

    void check_samples (
      const char * test,
      const RG_LateBindingTestSeq& samples,
      const ::CORBA::UShort& expected);
  };
};

#endif /* RG_LATEBINDING_RECEIVER_IMPL_H_ */

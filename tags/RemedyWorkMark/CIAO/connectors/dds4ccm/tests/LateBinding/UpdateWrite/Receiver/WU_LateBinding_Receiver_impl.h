// $Id$

#ifndef WU_LATEBINDING_RECEIVER_IMPL_H_
#define WU_LATEBINDING_RECEIVER_IMPL_H_

#include "WU_LateBinding_ReceiverEC.h"

class WU_LateBinding_Receiver_impl
{
public:
  WU_LateBinding_Receiver_impl (void);
  ~WU_LateBinding_Receiver_impl (void);

  static void start (
    ::WU_LateBinding::CCM_Receiver_Context_ptr ctx);

  static void on_one_data (const ::WU_LateBindingTest & datum);
  static void on_many_data (void);

  static void on_creation (const ::WU_LateBindingTest & datum);
  static void on_one_update (const ::WU_LateBindingTest & datum);
};

#endif /* WU_LATEBINDING_RECEIVER_IMPL_H_ */

/* $Id$ */
#ifndef EC_EVENT_LIMIT_H_
#define EC_EVENT_LIMIT_H_

#include "ace/Event_Handler.h"
#include "tao/ORB_Core.h"
#include <dsui.h>

class EC_Event_Limit : public ACE_Event_Handler
{

 public:
  EC_Event_Limit (TAO_ORB_Core* orb_ptr, ds_control* ds_ptr);
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);

 private:
  TAO_ORB_Core* m_orb_ptr;
  ds_control* m_ds_ptr;
};


#endif /* EC_EVENT_LIMIT_H_ */

/* $Id$ */
#ifndef EC_EVENT_LIMIT_H_
#define EC_EVENT_LIMIT_H_

#include "ace/Event_Handler.h"
#include "tao/ORB.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#endif //ACE_HAS_DSUI

class EC_Event_Limit : public ACE_Event_Handler
{

 public:
#ifdef ACE_HAS_DSUI
  EC_Event_Limit (CORBA::ORB_var orb, ds_control* ds_ptr);
#else
  EC_Event_Limit (CORBA::ORB_var orb);
#endif //ACE_HAS_DSUI

  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);

 private:
  CORBA::ORB_var m_orb_;

#ifdef ACE_HAS_DSUI
  ds_control* m_ds_ptr;
#endif //ACE_HAS_DSUI
};

#endif /* EC_EVENT_LIMIT_H_ */

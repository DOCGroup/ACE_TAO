
// $Id$

#include "ace/Service_Config.h"
#include "Notification_Receiver_i.h"

#if defined (ACE_HAS_ORBIX) && (ACE_HAS_ORBIX != 0)

Notification_Receiver_i::Notification_Receiver_i (void)
{
}

Notification_Receiver_i::~Notification_Receiver_i (void)
{
}

// Inform the <Event_Comm::Notification_Receiver> that <event> has occurred.

void
Notification_Receiver_i::receive_notification
  (const Event_Comm::Notification &notification,
   CORBA::Environment &IT_env)
{
  const char *tmpstr = notification.tag_;

  ACE_DEBUG ((LM_DEBUG, "**** got notification = %s\n", tmpstr));
}

// Disconnect the <Event_Comm::Notification_Receiver> from the <Event_Comm::Notifier>.

void 
Notification_Receiver_i::disconnect (const char *reason,
				     CORBA::Environment &IT_env)
{
  ACE_DEBUG ((LM_DEBUG, "**** got disconnected due to %s\n", reason));
  ACE_Reactor::end_event_loop();
}

#endif /* ACE_HAS_ORBIX */

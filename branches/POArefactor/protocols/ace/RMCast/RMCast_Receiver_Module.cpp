//
// $Id$
//

#include "RMCast_Receiver_Module.h"
#include "RMCast_Proxy.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Receiver_Module.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Receiver_Module, "$Id$")

ACE_RMCast_Receiver_Module::~ACE_RMCast_Receiver_Module (void)
{
}

int
ACE_RMCast_Receiver_Module::data (ACE_RMCast::Data &data)
{
  switch (this->state_)
    {
    case RS_NON_EXISTENT:
    case RS_JOINING:
      if (data.source != 0)
        {
          //ACE_DEBUG ((LM_DEBUG,
          //            "Receiver_Module::data - joining\n"));

          this->state_ = RS_JOINING;
          ACE_RMCast::Join join;
          if (data.source->reply_join (join) != 0)
            this->leaving (data.source);
        }
      break;

    case RS_JOINED:
      // ACE_DEBUG ((LM_DEBUG,
      //             "Receiver_Module::data - joined\n"));
      if (this->ACE_RMCast_Module::data (data) != 0)
        this->leaving (data.source);
      break;

    default:
    case RS_LEAVING:
      this->leaving (data.source);
      break;
    }
  return 0;
}

int
ACE_RMCast_Receiver_Module::poll (ACE_RMCast::Poll &poll)
{
  switch (this->state_)
    {
    case RS_NON_EXISTENT:
    case RS_JOINING:
      if (poll.source != 0)
        {
          this->state_ = RS_JOINING;
          ACE_RMCast::Join join;
          if (poll.source->reply_join (join) != 0)
            this->leaving (poll.source);
        }
      break;

    case RS_JOINED:
      if (this->ACE_RMCast_Module::poll (poll) != 0)
        this->leaving (poll.source);

    default:
    case RS_LEAVING:
      this->leaving (poll.source);
      break;
    }
  return 0;
}

int
ACE_RMCast_Receiver_Module::ack_join (ACE_RMCast::Ack_Join &ack_join)
{
  switch (this->state_)
    {
    case RS_NON_EXISTENT:
      if (ack_join.source != 0)
        {
          //ACE_DEBUG ((LM_DEBUG,
          //            "Receiver_Module::ack_join - joining\n"));

          this->state_ = RS_JOINING;
          ACE_RMCast::Join join;
          if (ack_join.source->reply_join (join) != 0)
            this->leaving (ack_join.source);
        }
      break;

    case RS_JOINING:
    case RS_JOINED:
      //ACE_DEBUG ((LM_DEBUG,
      //            "Receiver_Module::ack_join - joined\n"));

      this->state_ = RS_JOINED;
      if (this->ACE_RMCast_Module::ack_join (ack_join) != 0)
        this->leaving (ack_join.source);
      break;

    default:
    case RS_LEAVING:
      this->leaving (ack_join.source);
      break;
    }
  return 0;
}

int
ACE_RMCast_Receiver_Module::ack_leave (ACE_RMCast::Ack_Leave &ack_leave)
{
  //ACE_DEBUG ((LM_DEBUG,
  //            "Receiver_Module::ack_leave\n"));

  (void) this->ACE_RMCast_Module::ack_leave (ack_leave);

  // Returning -1 should remove the Proxy from the local set
  return -1;
}

int
ACE_RMCast_Receiver_Module::ack (ACE_RMCast::Ack &)
{
  return -1;
}

int
ACE_RMCast_Receiver_Module::join (ACE_RMCast::Join &)
{
  return -1;
}

int
ACE_RMCast_Receiver_Module::leave (ACE_RMCast::Leave &)
{
  return -1;
}

void
ACE_RMCast_Receiver_Module::leaving (ACE_RMCast_Proxy *proxy)
{
  this->state_ = RS_LEAVING;
  if (proxy != 0)
    {
      ACE_RMCast::Leave leave;
      (void) proxy->reply_leave (leave);
    }
  (void) this->close ();
}

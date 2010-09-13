// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
  const char* name,
  unsigned int expected,
  Notify_Test_Client& client)
  : name_ (name),
  expected_ (expected),
  count_ (0),
  delay_count_ (0),
  delay_period_ (5),
  client_ (client)
{
  this->client_.consumer_start (this);
}

void
Notify_Structured_Push_Consumer::set_delay_parameters (unsigned int delay_count, unsigned long delay_period)
{
  this->delay_count_ = delay_count;
  this->delay_period_ = delay_period;
}

void
Notify_Structured_Push_Consumer::push_structured_event (
  const CosNotification::StructuredEvent& /*event*/)
{
  ACE_DEBUG((LM_DEBUG, "-"));

  ++count_;

  if (this->delay_count_ != 0 && this->count_ % this->delay_count_ == 0)
    {
      ACE_OS::sleep (this->delay_period_);
    }

  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: too many events received.\n")));
  }

  if (this->count_ >= this->expected_)
  {
    this->client_.consumer_done (this);
  }
}


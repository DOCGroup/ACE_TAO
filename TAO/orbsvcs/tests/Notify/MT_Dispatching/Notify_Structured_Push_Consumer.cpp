#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
  unsigned int expected,
  Notify_Test_Client& client)
  : expected_ (expected),
  client_ (client)
{
  this->client_.consumer_start (this);
}

void
Notify_Structured_Push_Consumer::push_structured_event (
  CosNotification::StructuredEvent const& /*event*/)
{
  ACE_DEBUG((LM_DEBUG, "-"));

  unsigned int const count = this->count_.fetch_add (1, std::memory_order_relaxed) + 1;

  if (count > this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: too many events received.\n")));
  }

  if (count >= this->expected_)
  {
    this->client_.consumer_done (this);
  }
}


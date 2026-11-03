#include "Sender_i.h"

void
Sender_i::active_objects (CORBA::ULong no_threads)
{
  this->active_objects_ = no_threads;
}


void
Sender_i::send_ready_message ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Received a call ...\n"));

  this->number_received_.fetch_add (1, std::memory_order_relaxed);
}

CORBA::ULong
Sender_i::get_number_received () const
{
  return this->number_received_.load (std::memory_order_relaxed);
}

CORBA::ULong
Sender_i::get_active_objects () const
{
  return active_objects_;
}

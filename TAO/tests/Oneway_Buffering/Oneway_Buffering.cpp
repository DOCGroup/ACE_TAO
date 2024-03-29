#include "Oneway_Buffering.h"

Oneway_Buffering::Oneway_Buffering (CORBA::ORB_ptr orb,
                                    Test::Oneway_Buffering_Admin_ptr admin)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , admin_ (Test::Oneway_Buffering_Admin::_duplicate (admin))
{
}

void
Oneway_Buffering::receive_data (const Test::Payload &the_payload)
{
  this->admin_->request_received (the_payload.length ());
}

void
Oneway_Buffering::flush ()
{
}

void
Oneway_Buffering::sync ()
{
  this->admin_->flush ();
}

void
Oneway_Buffering::shutdown ()
{
  this->orb_->shutdown (false);
}

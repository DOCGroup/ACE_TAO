//
// $Id$
//
#include "AMI_Buffering.h"

AMI_Buffering::AMI_Buffering (CORBA::ORB_ptr orb,
                              Test::AMI_Buffering_Admin_ptr admin)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    admin_ (Test::AMI_Buffering_Admin::_duplicate (admin)),
    nest_ (0),
    max_nest_ (0),
    must_shutdown_ (false)
{
}

AMI_Buffering::Nest_Guard::Nest_Guard (AMI_Buffering &a)
  :target_(a)
{
  ++target_.nest_;
  if (target_.nest_ > target_.max_nest_)
    target_.max_nest_ = target_.nest_;
}

AMI_Buffering::Nest_Guard::~Nest_Guard (void)
{
  --target_.nest_;
}

void
AMI_Buffering::receive_data (const Test::Payload &the_payload)
{
  try
    {
      AMI_Buffering::Nest_Guard ng(*this);
      this->admin_->request_received (the_payload.length ());
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) DEBUG: AMI_Buffering::receive_data"));
    }

  this->try_shutdown();
}

void
AMI_Buffering::flush (void)
{
}

void
AMI_Buffering::sync (void)
{
  this->admin_->flush ();
}

void
AMI_Buffering::shutdown (void)
{
  this->must_shutdown_ = true;
  this->try_shutdown();
}

void
AMI_Buffering::try_shutdown(void)
{
  if (!this->must_shutdown_ || this->nest_ > 0)
    return;
  if (this->max_nest_ > 1)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) max nesting level: %d\n", max_nest_));
  this->admin_->shutdown();

  this->orb_->shutdown (0);
}

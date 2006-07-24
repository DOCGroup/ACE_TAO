//
// $Id$
//
#include "AMI_Buffering.h"

ACE_RCSID(AMI_Buffering, AMI_Buffering, "$Id$")

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
AMI_Buffering::receive_data (const Test::Payload &the_payload
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      AMI_Buffering::Nest_Guard ng(*this);
      this->admin_->request_received (the_payload.length () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) DEBUG: AMI_Buffering::receive_data"));
    }
  ACE_ENDTRY;

  this->try_shutdown(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
AMI_Buffering::flush (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
AMI_Buffering::sync (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->flush (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
AMI_Buffering::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->must_shutdown_ = true;
  this->try_shutdown(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
AMI_Buffering::try_shutdown(ACE_ENV_SINGLE_ARG_DECL)
{
  if (!this->must_shutdown_ || this->nest_ > 0)
    return;
  if (this->max_nest_ > 1)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) max nesting level: %d\n", max_nest_));
  this->admin_->shutdown(ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

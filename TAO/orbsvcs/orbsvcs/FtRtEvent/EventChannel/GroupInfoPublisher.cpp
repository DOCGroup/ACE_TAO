// $Id$

#include "GroupInfoPublisher.h"
#include "../Utils/resolve_init.h"
#include "IOGR_Maker.h"
#include "Identification_Service.h"
#include "FTEC_Become_Primary_Listener.h"

ACE_RCSID (EventChannel,
           GroupInfoPublisher,
           "$Id$")



GroupInfoPublisherBase::GroupInfoPublisherBase()
: primary_(false)
{
}



void
GroupInfoPublisherBase::subscribe(TAO_FTEC_Become_Primary_Listener* listener)
{
  subscribers_.push_back(listener);
}

void GroupInfoPublisherBase::set_naming_context(CosNaming::NamingContext_var naming_context)
{
  naming_context_ = naming_context;
}

bool
GroupInfoPublisherBase::is_primary() const
{
  return primary_;
}

CORBA::Object_var
GroupInfoPublisherBase::group_reference() const
{
  return iogr_;
}

FtRtecEventChannelAdmin::EventChannel_var
GroupInfoPublisherBase::successor() const
{
  return successor_;
}



const GroupInfoPublisherBase::BackupList&
GroupInfoPublisherBase::backups() const
{
  return backups_;
}

void
GroupInfoPublisherBase::update(const FTRT::ManagerInfoList & info_list,
                           int my_position
                           ACE_ENV_ARG_DECL)
{
  bool become_primary = (my_position == 0 && !primary_);
  primary_ = (my_position == 0);

  /// create the object group
  size_t len = info_list.length();

  TAO_IOP::TAO_IOR_Manipulation::IORList iors;
  iors.length(len);

  size_t i;
  for (i = 0; i < len; ++i) {
    iors[i] = CORBA::Object::_duplicate(info_list[i].ior.in());
  }

  CORBA::Object_var obj =
    IOGR_Maker::instance()->make_iogr(iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  iogr_ =
    ::FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (primary_ && !CORBA::is_nil(naming_context_.in())) {
    ACE_DEBUG((LM_DEBUG, "Registering to the Name Service\n"));
    naming_context_->rebind(FTRTEC::Identification_Service::instance()->name(),
                            iogr_.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  /// check if sucessor changed
  size_t successors_length = info_list.length() - my_position -1;

  if (successors_length != backups_.length())  {
    // successor changed, update successor
    iors.length(successors_length);
    for (i = 0; i < successors_length; ++i) {
      iors[i] = CORBA::Object::_duplicate(info_list[i+ my_position+1].ior.in());

    }

    obj =  IOGR_Maker::instance()->merge_iors(iors
                                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    successor_ = FtRtecEventChannelAdmin::EventChannel::_narrow(obj.in()
                                                                ACE_ENV_ARG_PARAMETER);
    FtRtecEventChannelAdmin::EventChannel_var t = successor_;
    ACE_CHECK;

    // update backups
    backups_.length(successors_length);
    for (i = 0; i < successors_length; ++i)  {
      backups_[i] =
        FtRtecEventChannelAdmin::EventChannel::_narrow(
                      info_list[i+ my_position+1].ior.in()
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }

  if (become_primary) {
    // notify the subscribers
    for (i = 0; i < subscribers_.size(); ++i)
      subscribers_[i]->become_primary();
  }
}

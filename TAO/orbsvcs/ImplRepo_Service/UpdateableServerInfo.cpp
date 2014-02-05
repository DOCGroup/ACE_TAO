// $Id$

#include "orbsvcs/Log_Macros.h"
#include "Locator_Repository.h"
#include "UpdateableServerInfo.h"

UpdateableServerInfo::UpdateableServerInfo (Locator_Repository* repo,
                                            const ACE_CString& name,
                                            int pid)
: repo_(repo),
  si_(repo->get_active_server (name,pid)),
  needs_update_(false)
{
}

UpdateableServerInfo::UpdateableServerInfo (Locator_Repository* repo,
                                            const Server_Info_Ptr& si,
                                            bool reset_start_count)
: repo_(repo),
  si_(si),
  needs_update_(false)
{
  if (reset_start_count)
    {
      needs_update_ = repo_ != 0;
      si_->start_count = 0;
    }
}

UpdateableServerInfo::UpdateableServerInfo (const Server_Info& si)
: repo_(0),
  si_(new Server_Info(si)),
  needs_update_(false)
{
}

UpdateableServerInfo::~UpdateableServerInfo ()
{
  update_repo();
}

void
UpdateableServerInfo::update_repo ()
{
  if (!needs_update_)
    return;

  needs_update_ = false;
  int err = repo_->update_server (si_);
  ACE_ASSERT (err == 0);
  ACE_UNUSED_ARG (err);
}

const Server_Info*
UpdateableServerInfo::operator-> () const
{
  return si_.get();
}

const Server_Info&
UpdateableServerInfo::operator* () const
{
  return *(si_.get());
}


const Server_Info_Ptr&
UpdateableServerInfo::edit ()
{
  needs_update_ = repo_ != 0;
  return si_;
}

void
UpdateableServerInfo::needs_update ()
{
  needs_update_ = true;
}

bool
UpdateableServerInfo::null() const
{
  return si_.null();
}

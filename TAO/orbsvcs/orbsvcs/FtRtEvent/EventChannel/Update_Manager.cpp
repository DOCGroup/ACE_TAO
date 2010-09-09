// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/Update_Manager.h"
#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Update_Manager::Update_Manager(ACE_Auto_Event& evt,
                               int num_backups,
                               int transaction_depth,
                               bool& success)
  : evt_(evt)
  , replied_(num_backups)
  , suicide_condition_(num_backups)
  , num_backups_(num_backups)
  , transaction_level_(transaction_depth)
  , success_(success)
{
  suicide_condition_.flip();
  signal_condition_.resize(transaction_depth, true);
  signal_condition_.resize(num_backups);
}

Update_Manager::~Update_Manager()
{
}

void Update_Manager::handle_reply(int id)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  replied_[id] = true;

  if ((replied_ & signal_condition_) == signal_condition_) {
    success_ = true;
    evt_.signal();
  }
  if (replied_ == suicide_condition_)
    delete this;
}

void Update_Manager::handle_exception(int id)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  replied_[id] = true;
  ++transaction_level_;

  if (num_backups_ > transaction_level_) {
    signal_condition_[transaction_level_]=true;
  }
  else {
    success_ = false;
    evt_.signal();
  }
  if (replied_ == suicide_condition_)
    delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL

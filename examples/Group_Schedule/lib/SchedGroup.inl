// $Id$

inline
SchedThread::SchedThread(
  SchedGroup* grp, const SchedQoS& qos, const char* thread_name)
: grp_(grp)
, member_id_(grp->add_current_thread(qos, thread_name))
{
}

inline
SchedThread::~SchedThread()
{
  if (member_id_ != -1)
    grp_->remove_current_thread(member_id_);
}

inline bool
SchedThread::is_valid() const
{
  return member_id_ != -1;
}

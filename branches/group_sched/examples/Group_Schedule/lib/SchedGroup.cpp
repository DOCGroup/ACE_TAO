// $Id$
#include "SchedGroup.h"

extern "C" {
#include "include/middleware/group_sched_api.h"
#include "include/middleware/group_sched_shared.h"
#include "include/group_sched/group_sched_rr_abi.h"
#include "include/group_sched/group_sched_static_priority_abi.h"
#include "include/group_sched/group_sched_seq_abi.h"
#include "include/group_sched/group_sched_frame_progress_abi.h"
}

#include <pthread.h>

namespace {
  int fd;
  pthread_once_t once_control = PTHREAD_ONCE_INIT;

  void init_group()
  {
    attach_shared_mem();
    fd = grp_open();
  }

  struct SharedMemoryDettacher
  {
    ~SharedMemoryDettacher() {
      detach_shared_mem();
    }
  } dettacher;
}

FrameProgressQoS::FrameProgressQoS()
{
  scheduler_name_ = FRAME_PROGRESS_SCHEDULER;
  param_len_ = sizeof(int);
  param_ = &progress;
}

RR_QoS::RR_QoS()
{
  scheduler_name_ = RR_SCHEDULER;
  param_len_ = 0;
  param_ = 0;
}

RR2_QoS::RR2_QoS()
{
  scheduler_name_ = RR2_SCHEDULER;
  param_len_ = sizeof(sched_rr2_data);
  param_ = static_cast<sched_rr2_data*>(this);
}

Seq_QoS::Seq_QoS()
{
  scheduler_name_ = SEQ_SCHEDULER;
  param_len_ = 0;
  param_ = 0;
}

Static_Priority_QoS::Static_Priority_QoS()
{
  scheduler_name_ = STATIC_PRIORITY_SCHEDULER;
  param_len_ = sizeof(int);
  param_ = &priority;
}


SchedGroup::SchedGroup(const char* name,
                       const SchedQoS& qos)
: parent_name_(0)
{
  pthread_once(&once_control, init_group);
  name_ = new char[strlen(name)+1];
  strcpy(name_, name);
  id_ = grp_create_group(fd, name_, qos.scheduler_name_, 0);
  if (id_ != -1) {
    grp_set_named_group_parameters(fd,
      (char*)name_,
      (void*)qos.param_,
      qos.param_len_);
  }
}

SchedGroup::SchedGroup(const char* name)
: id_ (-1),parent_name_(0)
{
  //pthread_once(&once_control, init_group);
  name_ = new char[strlen(name)+1];
  strcpy(name_, name);
}

SchedGroup::~SchedGroup()
{
  if (parent_name_) {
    if (parent_name_[0] == 0) {
      grp_remove_topsdf_group(fd);
    }
    else {
      grp_leave_group_by_name(fd, (char*)parent_name_, id_);
    }
  }
  if (id_ != -1) {
    grp_destroy_named_group(fd, (char*)name_);
  }
  delete name_;
}

int
SchedGroup::set_as_top()
{
  if (fd == 0) return -1;
  parent_name_ = "";
  return grp_set_topsdf_group_by_name(fd, (char*)name_);
}

int
SchedGroup::add_member(SchedGroup& member)
{
  if (fd == 0) return -1;
  int ret =  grp_join_group_by_name(fd,
    (char*)name_,
    (char*)member.name_,
    GRP_COMP_TYPE_GROUP,
    member.id_);
  if (ret != -1)
    member.parent_name_ = name_;
  return ret;
}

int
SchedGroup::add_current_thread(const SchedQoS& qos,
                               const char* thread_name)
{
  if (fd == 0) return -1;
  member_id_t member_id =  grp_join_group_by_name(fd,
    (char*)name_,
    (char*)thread_name,
    GRP_COMP_TYPE_PROCESS,
    ::getpid() );
  if ( (int)member_id == -1)
    return -1;
  if (qos.param_len_ > 0)
  {
    if (grp_set_named_group_member_parameters(
      fd, (char*)name_, (char*)thread_name, (void*)qos.param_, qos.param_len_
      ) == -1)
      return -1;
  }
  return member_id;
}


int
SchedGroup::remove_current_thread(int member_id)
{
  if (fd == 0) return -1;
  return grp_leave_group_by_name(fd, (char*)name_, member_id);
}










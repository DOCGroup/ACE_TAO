// $Id$

#ifndef ACE_ADAPTER_TASK_T_H
#define ACE_ADAPTER_TASK_T_H
#include "ace/pre.h"

#include "../../Task_T.h"

template <ACE_SYNCH_DECL>
class ACE_Task_W : public ACE_Task<ACE_SYNCH_DECL>
{
  ACE_Task_W (ACE_Thread_Manager *thr_mgr = 0,
              ACE_Message_Queue<ACE_SYNCH_USE> *mq = 0)
    : ACE_Task<ACE_SYNCH_DECL> (thr_mgr, mq)
  {}

  virtual ~ACE_Task_W (void)
  {}

  const wchar_t *name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_Task<ACE_SYNCH_DECL>::name ())); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_TASK_T_H */

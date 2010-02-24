// -*- C++ -*-

//=============================================================================
/**
 *  @file   TP_Task.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef TP_TASK_H
#define TP_TASK_H

#include "ace/Task.h"
#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TP_Task : public ACE_Task_Base {
public:
  TP_Task(CORBA::ORB_var& orb)
    : orb_(orb)
  {
  }

  ~TP_Task(){}
  virtual int svc() {
    this->orb_->run();
    return 0;
  }
private:
  CORBA::ORB_var& orb_;
};

#endif

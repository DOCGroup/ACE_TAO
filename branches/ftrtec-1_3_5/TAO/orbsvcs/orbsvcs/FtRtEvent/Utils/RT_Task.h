// -*- C++ -*-
//=============================================================================
/**
 *  @file   RT_Task.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef RT_TASK_H
#define RT_TASK_H
#include "ftrtevent_export.h"

class ACE_Task_Base;

class TAO_FtRtEvent_Export RT_Task 
{
public:
  static void set_current();
  static int activate(ACE_Task_Base*);
};

#endif

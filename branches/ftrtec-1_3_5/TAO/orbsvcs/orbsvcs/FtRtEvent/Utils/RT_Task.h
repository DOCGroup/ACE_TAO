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

class ACE_Task_Base;

class RT_Task 
{
public:
  static void set_current();
  static int activate(ACE_Task_Base*);
};

#endif

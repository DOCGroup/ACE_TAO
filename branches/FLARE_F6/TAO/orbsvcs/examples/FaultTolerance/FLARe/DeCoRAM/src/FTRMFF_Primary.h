// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Primary.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_PRIMARY_H_
#define FTRMFF_PRIMARY_H_

#include "Schedule.h"
#include "CTT_Basic.h"

class FTRMFF_Primary : public FTRMFF_Algorithm
{
public:
  virtual ~FTRMFF_Primary ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
};

class FTRMFF_Primary_Algorithm : public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Primary_Algorithm (const PROCESSOR_LIST & processors);

  virtual ~FTRMFF_Primary_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  SCHEDULE schedule_;
  CTT_Basic ctt_;
};

#endif /* FTRMFF_PRIMARY_H_ */

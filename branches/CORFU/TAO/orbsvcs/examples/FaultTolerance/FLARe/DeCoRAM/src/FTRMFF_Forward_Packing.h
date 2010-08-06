// -*- C++ -*-

//=============================================================================
/**
 *  @file     FTRMFF_Forward_Packing.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FTRMFF_FORWARD_PACKING_ALGORITHM_H_
#define FTRMFF_FORWARD_PACKING_ALGORITHM_H_

#include "Scheduler.h"
#include <memory>

class FTRMFF_Forward_Packing : public FTRMFF_Algorithm
{
public:
  FTRMFF_Forward_Packing (const std::string & algorithm = "Forward_Packing");

  virtual ~FTRMFF_Forward_Packing ();

  virtual FTRMFF_Output operator () (const FTRMFF_Input & input);
private:
  std::string algorithm_;
};

class FTRMFF_Forward_Packing_Algorithm : 
  public FTRMFF_Algorithm_Impl
{
public:
  FTRMFF_Forward_Packing_Algorithm (const PROCESSOR_LIST & processors,
                                    unsigned int consistency_level,
                                    const std::string & scheduler);

  virtual ~FTRMFF_Forward_Packing_Algorithm ();

  virtual SCHEDULING_MAP operator () (const TASK_LIST & tasks);

  virtual const SCHEDULE & schedule () const;

private:
  std::auto_ptr<Scheduler> scheduler_;
};

#endif /* FTRMFF_FORWARD_PACKING_ALGORITHM_H_ */

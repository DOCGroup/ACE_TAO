/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Job_i.h
 *
 *  This file defines the servant for the Job.idl interface
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef JOB_I_H
#define JOB_I_H

#include "JobS.h"
#include "ace/SString.h"
#include "ace/Arg_Shifter.h"

/**
 * @class Job_i
 *
 * @brief Implements a Job that performs some cpu bound work.
 */
class Job_i : public POA_Job
{
 public:
  /// Constructor
  Job_i (void);

  /// Init the state of this object.
  int init (ACE_Arg_Shifter& arg_shifter);

  /// = Accessors
  const ACE_CString& name (void);
  const ACE_CString& poa (void);

  /// = inteface Job method implementation.
  virtual void work (CORBA::ULong work);

  virtual void shutdown (void);
 protected:
  /// The name of this Job
  ACE_CString job_name_;

  /// The name of the POA that we live in.
  ACE_CString POA_name_;
};

#endif /* JOB_I_H */

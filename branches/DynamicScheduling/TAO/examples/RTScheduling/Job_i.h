/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Job_i.h
 *
 *  $Id$
 *
 *  This file defines the servant for the Job.idl interface
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef JOB_I_H
#define JOB_I_H

#include "JobS.h"
#include "Task_Stats.h"

class ACE_Arg_Shifter;
class DT_Creator;

/**
 * @class Job_i
 *
 * @brief Implements a Job that performs some cpu bound work.
 *
 */
class Job_i : public POA_Job, public virtual PortableServer::RefCountServantBase
{
 public:
  /// Constructor
  Job_i (DT_Creator* dt_creator);

  ~Job_i (void);

  /// Init the state of this object.
  int init (ACE_Arg_Shifter& arg_shifter);

  /// = Accessors
  const ACE_CString& name (void);
  const ACE_CString& poa (void);

  /// = inteface Job method implementation.
  virtual void work (CORBA::ULong work,
		     CORBA::Short importance
		     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void post_work (int guid,
			  int importance);

  void dump_stats (void);

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  int guid (void);

 protected:
  /// The name of this Job
  ACE_CString job_name_;

  /// The name of the POA that we live in.
  ACE_CString POA_name_;

  DT_Creator* dt_creator_;

  Task_Stats *task_stats_;

  int guid_;
};

#endif /* JOB_I_H */

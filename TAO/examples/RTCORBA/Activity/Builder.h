/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Builder.h
 *
 *  The builder creates Tasks, Jobs, POAs.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef BUILDER_H
#define BUILDER_H

#include "orbsvcs/CosNamingC.h"
#include "tao/RTPortableServer/RTPortableServer.h"

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Arg_Shifter.h"

class Activity;
class Periodic_Task;
class Job_i;
class POA_Holder;

typedef POA_Holder** POA_LIST;
typedef Periodic_Task** TASK_LIST;
typedef Job_i** JOB_LIST;

/**
 * @class Builder
 *
 * @brief A service object that creates Tasks, Jobs, POAs.
 */
class Builder : public ACE_Service_Object
{
 public:
  /// = Initialization and termination
  Builder ();
  ~Builder ();

  /// = Service_Object virtual method overloads.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini (void);

  /// = List access
  int poa_list (POA_LIST& poa_list);
  int task_list (TASK_LIST& task_list);
  int job_list (JOB_LIST &job_list);

 protected:

  /// Number of POA's declared
  int poa_count_;

  /// The list of POA's.
  POA_LIST poa_list_;

  /// Number of Tasks's declared
  int task_count_;

  /// The list of Tasks.
  TASK_LIST task_list_;

  /// Number of Jobs's declared
  int job_count_;

  /// The list of Jobs
  JOB_LIST job_list_;
};

ACE_STATIC_SVC_DECLARE (Builder)

ACE_FACTORY_DECLARE (ACE_Local_Service, Builder)

#endif /* BUILDER_H */

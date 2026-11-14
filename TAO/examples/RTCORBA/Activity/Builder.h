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

#include <memory>
#include <vector>

class Activity;
class Periodic_Task;
class Job_i;
class POA_Holder;

using POA_LIST = std::vector<std::unique_ptr<POA_Holder>>;
using TASK_LIST = std::vector<std::unique_ptr<Periodic_Task>>;
using JOB_LIST = std::vector<Job_i *>;

/**
 * @class Builder
 *
 * @brief A service object that creates Tasks, Jobs, POAs.
 */
class Builder : public ACE_Service_Object
{
 public:
  Builder ();
  ~Builder () override;

  Builder (Builder const &) = delete;
  Builder (Builder &&) = delete;
  Builder &operator= (Builder const &) = delete;
  Builder &operator= (Builder &&) = delete;

  /// = Service_Object virtual method overloads.
  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;

  /// = List access
  POA_LIST const &poa_list () const;
  TASK_LIST const &task_list () const;
  JOB_LIST const &job_list () const;

 protected:
  /// Number of POA's declared
  int poa_count_;

  /// Owns the POA_Holder objects.
  POA_LIST poa_list_;

  /// Number of Tasks's declared
  int task_count_;

  /// Owns the task objects.
  TASK_LIST task_list_;

  /// Number of Jobs's declared
  int job_count_;

  /// Borrowed job pointers; servants are activated and owned by their POAs.
  JOB_LIST job_list_;
};

ACE_STATIC_SVC_DECLARE (Builder)

ACE_FACTORY_DECLARE (ACE_Local_Service, Builder)

#endif /* BUILDER_H */

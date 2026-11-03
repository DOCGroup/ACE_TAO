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

class Activity;
class Periodic_Task;
class Job_i;
class POA_Holder;

using POA_LIST = POA_Holder **;
using TASK_LIST = Periodic_Task **;
using JOB_LIST = Job_i **;

/**
 * @class Builder
 *
 * @brief A service object that creates Tasks, Jobs, POAs.
 */
class Builder : public ACE_Service_Object
{
 public:
  Builder ();
  ~Builder () override = default;

  Builder (Builder const &) = delete;
  Builder (Builder &&) = delete;
  Builder &operator= (Builder const &) = delete;
  Builder &operator= (Builder &&) = delete;

  /// = Service_Object virtual method overloads.
  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;

  /// = List access
  int poa_list (POA_LIST& poa_list) const;
  int task_list (TASK_LIST& task_list) const;
  int job_list (JOB_LIST& job_list) const;

 protected:
  /// Number of POA's declared
  int poa_count_;

  /// Owns the array storage; fini() deletes the POA_Holder objects in its slots.
  std::unique_ptr<POA_Holder *[]> poa_list_;

  /// Number of Tasks's declared
  int task_count_;

  /// Owns the array storage and task objects; Activity borrows the raw view.
  std::unique_ptr<Periodic_Task *[]> task_list_;

  /// Number of Jobs's declared
  int job_count_;

  /// Owns only the array storage; servants are activated and owned by their POAs.
  std::unique_ptr<Job_i *[]> job_list_;
};

ACE_STATIC_SVC_DECLARE (Builder)

ACE_FACTORY_DECLARE (ACE_Local_Service, Builder)

#endif /* BUILDER_H */

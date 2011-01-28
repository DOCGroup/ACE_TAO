// $Id$

/**
 * @file Exec_Dest_Status.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 */

#ifndef EXEC_STATUS_H_
#define EXEC_STATUS_H_

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#include <map>
#include <string>

#include "Exec_Dest_export.h"

namespace Deployment
{
  struct DeploymentPlan;
}

namespace dance_test
{
  class Executor_Destructor_Interceptor_Export Exec_Dest_Status
  {
  public:
    Exec_Dest_Status (void);

    ~Exec_Dest_Status (void);

    void plan (const ::Deployment::DeploymentPlan *plan);

    void destructor_called (const std::string &id);

    bool check_destructor (const std::string &id);
  private:

    struct Instance_Status
    {
      Instance_Status (void);

      bool destructor_;
    };

    typedef std::map <std::string, Instance_Status> Inst_Map;
    Inst_Map instances_;
  };

  typedef ACE_Singleton <Exec_Dest_Status,
                         ACE_Null_Mutex> EXEC_DEST_STATUS;
}

EXECUTOR_DESTRUCTOR_INTERCEPTOR_SINGLETON_DECLARE (ACE_Singleton,
                                                   dance_test::Exec_Dest_Status,
                                                   ACE_Null_Mutex)

#endif

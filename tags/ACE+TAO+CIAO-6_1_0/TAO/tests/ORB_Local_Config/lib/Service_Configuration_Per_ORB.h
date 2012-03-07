// $Id$

#ifndef SERVICE_CONFIGURATION_PER_ORB_H
#  define SERVICE_CONFIGURATION_PER_ORB_H


#include "ace/Trace.h"
#include "ace/Service_Config.h"

/// We need this wrapper to "break" the encapsulation and test
/// the internals of the class. The class also does a complete
/// teardown on the service repository used. So, do not expect
/// to find *any*, even static services in the global repo afer
/// this class desructor is done.

class ACE_Service_Gestalt_Test : public ACE_Service_Gestalt
{
 public:
  ACE_Service_Gestalt_Test (size_t n)
    : ACE_Service_Gestalt (n)
    , teardown_ (false)
    {
    };

  ACE_Service_Gestalt_Test ()
    : ACE_Service_Gestalt (ACE_Service_Repository::DEFAULT_SIZE)
    , teardown_ (true)
    {
    };

  ~ACE_Service_Gestalt_Test (void)
    {
      if (this->teardown_)
        {
          // Close and possibly delete all service instances in the Service
          // Repository.
          ACE_Service_Config::fini_svcs ();

          // Unlink all services in the Service Repository and close/delete
          // all ACE library services and singletons.
          ACE_Service_Config::close ();
        }
    };

  size_t command_line_directives_count (void) const
  {
    return this->svc_queue_->size ();
  };

  size_t service_config_files_count (void) const
  {
    return this->svc_conf_file_queue_->size ();
  };

  size_t services_count (void) const
  {
    return this->repo_->current_size ();
  };

  bool has_same_service_repository ( ACE_Service_Gestalt_Test const * psg)
  {
    return (this->repo_ == psg->repo_);
  }

private:
  bool teardown_;
};


#endif /* SERVICE_CONFIGURATION_PER_ORB_H */

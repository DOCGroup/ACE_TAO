// $Id$

#include "tao/TAO.h"
#include "Time_Date_i.h"

class ACE_Svc_Export DLL_ORB : public ACE_Task_Base
{
public:
  virtual int init (int argc, char *argv[]);
  // Initialize the <TAO_ORB_Manager>.

  virtual int fini (void);
  // Shutdown the <TAO_ORB_Manager>.
  
  virtual int svc (void);
  // Concurrency hook.

  TAO_ORB_Manager orb_manager_;
  // ORB manager.
};

class ACE_Svc_Export Time_Date_Servant : public ACE_Service_Object
{
public:
  virtual int init (int argc, char *argv[]);
  // Initialize the <Time_Date> servant.

  int parse_args (int argc, char *argv[]);

private:
   Time_Date_i servant_;
  // Servant for the <Time_Date> object.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  const char *orb_;
  // Name of the ORB we're linking dynamically.
};

// The following Factory is used by the <ACE_Service_Config> and
// dll_orb.conf file to dynamically initialize the state of the 
// Time_Date service.
ACE_SVC_FACTORY_DECLARE (DLL_ORB)
ACE_SVC_FACTORY_DECLARE (Time_Date_Servant)

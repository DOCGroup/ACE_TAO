// $Id$

#include "ace/Task.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "tao/default_resource.h"
#include "Time_Date_i.h"
#include "Alt_Resource_Factory.h"

class Alt_Resource_Factory_Export My_Resource_Factory : public TAO_Default_Resource_Factory
{
  // = TITLE
  //   Illustrates how to select the appropriate <ACE_Reactor>.
  //
  // = DESCRIPTION
  //   If we're running in a single-threaded configuration this will
  //   be the <ACE_Reactor::instance> singleton.  Otherwise, it'll be
  //   the ORB's own default <ACE_Reactor>, which is defined by the
  //   <TAO_Default_Resource_Factory>.
public:
  virtual ACE_Reactor *get_reactor (void);
  // Return the <ACE_Reactor> that will be utilized by the ORB.
};

ACE_FACTORY_DECLARE (Alt_Resource_Factory, My_Resource_Factory)

class ACE_Svc_Export DLL_ORB : public ACE_Task_Base
{
  // = TITLE
  //   Define a class that dynamically links an ORB into a server
  //   process via the <ACE_Service_Configurator>.
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
  // = TITLE
  //   Define a class that dynamically links the Time_Date service
  //   into a server process via the <ACE_Service_Configurator>.  Note
  //   that this assumes the ORB has been linked previously.
public:
  virtual int init (int argc, char *argv[]);
  // Initialize the <Time_Date> servant.

  int parse_args (int argc, char *argv[]);
  // Parse the "command-line" arguments.

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

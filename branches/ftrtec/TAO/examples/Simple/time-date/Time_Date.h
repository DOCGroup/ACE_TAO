// -*- C++ -*-
//
// $Id$

#include "ace/Task.h"
#include "ace/svc_export.h"
#include "tao/default_resource.h"
#include "Alt_Resource_Factory.h"

class Time_Date_i;

/**
 * @class My_Resource_Factory
 *
 * @brief Illustrates how to select the appropriate @c ACE_Reactor.
 *
 * If we're running in a single-threaded configuration this will be
 * the @c ACE_Reactor::instance singleton.  Otherwise, it'll be the
 * ORB's own default @c ACE_Reactor, which is defined by the
 * @c TAO_Default_Resource_Factory.
 */
class Alt_Resource_Factory_Export My_Resource_Factory
  : public TAO_Default_Resource_Factory
{
public:

  /// Return the @c ACE_Reactor that will be utilized by the ORB.
  virtual ACE_Reactor *get_reactor (void);

};

ACE_FACTORY_DECLARE (Alt_Resource_Factory, My_Resource_Factory)

/**
 * @class DLL_ORB
 *
 * @brief Define a class that dynamically links an ORB into a server
 *        process via the @c ACE_Service_Configurator.
 */
class ACE_Svc_Export DLL_ORB : public ACE_Task_Base
{
public:

  /// Initialize the @c TAO_ORB_Manager.
  virtual int init (int argc, char *argv[]);

  /// Shutdown the @c TAO_ORB_Manager.
  virtual int fini (void);

  /// Concurrency hook.
  virtual int svc (void);

public:

  /// ORB pseudo-reference.
  CORBA::ORB_var orb_;

  /// Reference to the POA.
  PortableServer::POA_var poa_;

  /// Reference to the POA Manager.
  PortableServer::POAManager_var poa_manager_;

};

/**
 * @class Time_Date_Servant
 *
 * @brief Define a class that dynamically links the Time_Date service
 *        into a server process via the @c ACE_Service_Configurator.
 *        Note that this assumes the ORB has been linked previously.
 */
class ACE_Svc_Export Time_Date_Servant : public ACE_Service_Object
{
public:

  /// Initialize the @c Time_Date servant.
  virtual int init (int argc, char *argv[]);

  /// Parse the "command-line" arguments.
  int parse_args (int argc, char *argv[]);

private:

  /// Servant for the @c Time_Date object.
  Time_Date_i * servant_;

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_;

  /// Name of the ORB we're linking dynamically.
  const char *orb_;

};

// The following Factory is used by the <ACE_Service_Config> and
// dll_orb.conf file to dynamically initialize the state of the
// Time_Date service.
ACE_SVC_FACTORY_DECLARE (DLL_ORB)
ACE_SVC_FACTORY_DECLARE (Time_Date_Servant)

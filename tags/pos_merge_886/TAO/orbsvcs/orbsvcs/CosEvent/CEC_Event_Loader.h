// $Id$

// ================================================================
// FILENAME
//   CEC_Event_Loader.h
//
// DESCRIPTION
//   This class loads the Event Service dynamically.
//
// AUTHOR
//   Priyanka Gontla <pgontla@ece.uci.edu>
// ================================================================

#ifndef TAO_CEC_EVENT_LOADER_H
#define TAO_CEC_EVENT_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CEC_EventChannel.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Service_Config.h"

class TAO_Event_Export TAO_CEC_Event_Loader : public TAO_Object_Loader
{
 public:

  // Constructor
  TAO_CEC_Event_Loader (void);

  // Destructor
  ~TAO_CEC_Event_Loader (void);

  // Called by the Service Configurator framework to initialize the
  // Event Service. Defined in <ace/Service_Config.h>
  virtual int init (int argc, char *argv[]);

  // Called by the Service Configurator framework to remove the
  // Event Service. Defined in <ace/Service_Config.h>
  virtual int fini (void);

  // This function call creates the Event Channel given a reference to the
  // ORB and the command line parameters.
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc, char *argv[],
                                   CORBA::Environment
                                   &ACE_TRY_ENV)
     ACE_THROW_SPEC ((CORBA::SystemException));

 protected:

  // ORB
  CORBA::ORB_var orb_;

  // Flag to control the event loop
  int terminate_flag_;

  // Create and activate the event service
  TAO_CEC_EventChannel_Attributes *attributes_;

  // TAO_CEC_Factory
  TAO_CEC_Factory *factory_;

  // TAO_CEC_EventChannel
  TAO_CEC_EventChannel *ec_impl_;

  // Naming Context needed if '-x' option is passed
  CosNaming::NamingContext_var naming_context_;

  // Flag to check if '-x' option is passed
  int bind_to_naming_service_;

  // CosNaming::Name
  CosNaming::Name channel_name_;

 private:
  ACE_UNIMPLEMENTED_FUNC (TAO_CEC_Event_Loader (const TAO_CEC_Event_Loader &))
  ACE_UNIMPLEMENTED_FUNC (TAO_CEC_Event_Loader &operator= (const TAO_CEC_Event_Loader &))

};

ACE_FACTORY_DECLARE (TAO_Event, TAO_CEC_Event_Loader)

#endif /* TAO_CEC_EVENT_LOADER_H */

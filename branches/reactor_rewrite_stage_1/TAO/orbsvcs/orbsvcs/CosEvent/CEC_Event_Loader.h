/**
 * @file CEC_Event_Loader.h
 *
 * $Id$
 *
 * Define a class to dynamically load the COS Event Service.
 *
 * @author Priyanka Gontla <pgontla@ece.uci.edu>
 */

#ifndef TAO_CEC_EVENT_LOADER_H
#define TAO_CEC_EVENT_LOADER_H

#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CEC_EventChannel.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Service_Config.h"

/**
 * @class TAO_CEC_Event_Loader
 *
 * @brief Dynamically load an instance of the COS Event Service.
 */
class TAO_Event_Export TAO_CEC_Event_Loader : public TAO_Object_Loader
{
public:
  /// Constructor
  TAO_CEC_Event_Loader (void);

  /// Destructor
  ~TAO_CEC_Event_Loader (void);

  //@{
  /**
   * @name Derived from ACE_Service_Object
   */
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);
  //@}

  //@{
  /**
   * @name Derived from TAO_Object_Loader
   */
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc, char *argv[]
                                   TAO_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

protected:
  /// Keep a pointer to the underlying ORB.
  CORBA::ORB_var orb_;

  /// Flag to control the event loop
  int terminate_flag_;

  /// Attributes used to configure the Event Service properties.
  TAO_CEC_EventChannel_Attributes *attributes_;

  /// Factory used to configure the Event Service strategies.
  TAO_CEC_Factory *factory_;

  /// The Event Service implementation class.
  TAO_CEC_EventChannel *ec_impl_;

  /// Naming Context needed if '-x' option is passed
  CosNaming::NamingContext_var naming_context_;

  /// Flag to check if '-x' option is passed
  int bind_to_naming_service_;

  /// The name used when binding to the NamingService.
  CosNaming::Name channel_name_;

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_CEC_Event_Loader (const TAO_CEC_Event_Loader &))
  ACE_UNIMPLEMENTED_FUNC (TAO_CEC_Event_Loader &operator= (const TAO_CEC_Event_Loader &))
};

ACE_FACTORY_DECLARE (TAO_Event, TAO_CEC_Event_Loader)

#endif /* TAO_CEC_EVENT_LOADER_H */

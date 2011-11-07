/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *  Test client for the Property Service.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef PROPERTY_CLIENT_H
#define PROPERTY_CLIENT_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "orbsvcs/Naming/Naming_Client.h"

/**
 * @class Client
 *
 * @brief Client for A/V Streams demo.
 *
 * Encapsulates client functionality.
 */
class Client
{
public:
  /// Constructor.
  Client (void);

  /// Initialize the ORB etc, and bind the MMDevices.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Run the client.  This will call StreamCtrl::bind_devs.
  int run (void);

  /// Testing  the property service methods.
  int property_tester (void);

protected:
  /// Bind to the remote MMDevice.
  int bind_to_remote_mmdevice (int argc,
                               ACE_TCHAR *argv[]);

  // = Property Testing.

  /// Testing the define property method.
  int test_define_property (void);

  /// Testing the number of properties, currently in the Property Set.
  int test_get_number_of_properties (void);

  ///Testing get_all_property_names.
  int test_get_all_property_names (void);

  /// Testing get_property_value.
  int test_get_property_value (void);

  ///Testing get_properties.  Give the names and get their properties.
  int test_get_properties (void);

  /// Testing get_all_properties.
  int test_get_all_properties (void);

  /// Testing delete_property.
  int test_delete_property (const char *property_name);

  /// Testing delete_properties.
  int test_delete_properties (void);

  /// Defining a sequence of properties.
  int test_define_properties (void);

  /// Defines properties with various modes.
  int test_define_property_with_mode (void);

  /// The ORB manager, handles ORB initialization etc.
  TAO_ORB_Manager manager_;

  /// The Naming Client handles Naming Service work
  TAO_Naming_Client my_name_client_;

  /// To test propertySetDef interface.
  CosPropertyService::PropertySetDef_var propsetdef_;
};

#endif /* COSPROPERTYSERVICE_CLIENT_H */

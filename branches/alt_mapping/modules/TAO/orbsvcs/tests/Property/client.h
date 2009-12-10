/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/Property
//
// = FILENAME
//    client.h
//
// = DESCRIPTION
//    Test client for the Property Service.
//
// = AUTHORS
//   Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

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

class Client
{
  // = TITLE
  //    Client for A/V Streams demo.
  //
  // = DESCRIPTION
  //    Encapsulates client functionality.
public:
  Client (void);
  // Constructor.

  int init (int argc,
            ACE_TCHAR **argv);
  // Initialize the ORB etc, and bind the MMDevices.

  int run (void);
  // Run the client.  This will call StreamCtrl::bind_devs.

  int property_tester (void);
  // Testing  the property service methods.

protected:
  int bind_to_remote_mmdevice (int argc,
                               ACE_TCHAR *argv[]);
  // Bind to the remote MMDevice.

  // = Property Testing.

  int test_define_property (void);
  // Testing the define property method.

  int test_get_number_of_properties (void);
  // Testing the number of properties, currently in the Property Set.

  int test_get_all_property_names (void);
  //Testing get_all_property_names.

  int test_get_property_value (void);
  // Testing get_property_value.

  int test_get_properties (void);
  //Testing get_properties.  Give the names and get their properties.

  int test_get_all_properties (void);
  // Testing get_all_properties.

  int test_delete_property (const char *property_name);
  // Testing delete_property.

  int test_delete_properties (void);
  // Testing delete_properties.

  int test_define_properties (void);
  // Defining a sequence of properties.

  int test_define_property_with_mode (void);
  // Defines properties with various modes.

  TAO_ORB_Manager manager_;
  // The ORB manager, handles ORB initialization etc.

  TAO_Naming_Client my_name_client_;
  // The Naming Client handles Naming Service work

  CosPropertyService::PropertySetDef_var propsetdef_;
  // To test propertySetDef interface.
};

#endif /* COSPROPERTYSERVICE_CLIENT_H */

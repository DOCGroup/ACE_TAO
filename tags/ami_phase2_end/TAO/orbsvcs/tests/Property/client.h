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

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "orbsvcs/Naming/Naming_Utils.h"

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
            char **argv,
            CORBA::Environment &ACE_TRY_ENV);
  // Initialize the ORB etc, and bind the MMDevices.

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Run the client.  This will call StreamCtrl::bind_devs.

  int property_tester (CORBA::Environment &ACE_TRY_ENV);
  // Testing  the property service methods.

protected:
  int bind_to_remote_mmdevice (int argc,
                               char *argv[],
                               CORBA::Environment &ACE_TRY_ENV);
  // Bind to the remote MMDevice.

  // = Property Testing.

  int test_define_property (CORBA::Environment &ACE_TRY_ENV);
  // Testing the define property method.

  int test_get_number_of_properties (CORBA::Environment &ACE_TRY_ENV);
  // Testing the number of properties, currently in the Property Set.

  int test_get_all_property_names (CORBA::Environment &ACE_TRY_ENV);
  //Testing get_all_property_names.

  int test_get_property_value (CORBA::Environment &ACE_TRY_ENV);
  // Testing get_property_value.

  int test_get_properties (CORBA::Environment &ACE_TRY_ENV);
  //Testing get_properties.  Give the names and get their properties.

  int test_get_all_properties (CORBA::Environment &ACE_TRY_ENV);
  // Testing get_all_properties.

  int test_delete_property (CORBA::String property_name,
                            CORBA::Environment &ACE_TRY_ENV);
  // Testing delete_property.

  int test_delete_properties (CORBA::Environment &ACE_TRY_ENV);
  // Testing delete_properties.

  int test_define_properties (CORBA::Environment &ACE_TRY_ENV);
  // Defining a sequence of properties.

  int test_define_property_with_mode (CORBA::Environment &ACE_TRY_ENV);
  // Defines properties with various modes.

  TAO_ORB_Manager manager_;
  // The ORB manager, handles ORB initialization etc.

  TAO_Naming_Client my_name_client_;
  // The Naming Client handles Naming Service work

  CosPropertyService::PropertySetDef_var propsetdef_;
  // To test propertySetDef interface.
};

#endif /* COSPROPERTYSERVICE_CLIENT_H */

// -*- C++ -*-

//=============================================================================
/**
 * @file PropertyManagerTest.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_PROPERTY_MANAGER_TEST_H
#define TAO_PROPERTY_MANAGER_TEST_H

#include "orbsvcs/LoadBalancingC.h"
#include "HasherFactory.h"
/**
 * @class PropertyManagerTest
 *
 * @brief Class that provides methods that set up properties in the
 *        LoadBalancer in addition to testing the PropertyManager
 *        functionality in the Load Balancer.
 */
class PropertyManagerTest
{
public:

  /// Constructor.
  PropertyManagerTest (TAO_LoadBalancer::PropertyManager_ptr p);

  /// Run the PropertManager test.
  /**
   * Running the PropertyManager test also sets up the Load Balancer
   * for the remainder of the test, not just the PropertyManager
   * test.
   */
  int run (void);

private:

  /// Set up the default properties in the Load Balancer.
  /**
   * This sets up the default number of initial and minimum number of
   * replicas, in addition to factory information.
   */
  int set_default_properties (void);

  /// Verify that the Load Balancer detects invalid properties.
  int test_invalid_property (void);

  /// Verify that the Load Balancer detects unsupported properties.
  int test_unsupported_property (void);

  /// Test the ability to obtain the default properties from the Load
  /// Balancer.
  int get_default_properties (void);

  /// Test the ability to remove the default properties set by this
  /// test from the Load Balancer.
  int remove_default_properties (void);

  /// Set properties specific to the different types of object groups.
  int set_type_properties (void);

  /// Test the ability to obtain the properties from the Load Balancer
  /// specific to a given type of object group.
  int get_type_properties (void);

  /// Test the ability to remove the properties specific to a given
  /// type of object group set by this test from the Load Balancer.
  int remove_type_properties (void);

  /// Test the ability of the Load Balancer to change properties
  /// "on-the-fly," such as when load balancing is actually being
  /// performed.
  int set_properties_dynamically (void);

  /// Verify that the Load Balancer is able to determine that a given
  /// object group is not available.
  int test_object_group_not_found (void);

  /// Test the ability to obtain properties applied to a given object
  /// group.
  /**
   * The returned properties should include those set by default,
   * those that are type specific, and those set dynamically.  This
   * also verifies that properties were properly overridden
   * (e.g. type-specific overrides default, and dynamic overrides
   * type-specific and default).
   */
  int get_properties (void);

private:

  /// Reference to the Load Balancer down cast to its
  /// PropertyManager interface.
  TAO_LoadBalancer::PropertyManager_var property_manager_;

  /// The HasherFactory servant.
  HasherFactory hasher_factory_;

};

#endif  /* TAO_PROPERTY_MANAGER_TEST_H */

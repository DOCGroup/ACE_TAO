// -*- C++ -*-
#ifndef BUG_2247_REGRESSION_MANAGER_H
#define BUG_2247_REGRESSION_MANAGER_H

#include "tao/ORB.h"
#include "tao/IORManipulation/IORManip_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"

struct Test_Options
{
  ACE_TCHAR const *first_ior {};
  ACE_TCHAR const *first_key {};
  ACE_TCHAR const *second_ior {};
  ACE_TCHAR const *second_key {};
  ACE_TCHAR const *ior_output_file {};
  bool shutdown_test {};
  bool merged_test {};
};

class Manager
{
public:
  Manager () = default;

  Manager (Manager const &) = delete;
  Manager (Manager &&) = delete;
  Manager &operator= (Manager const &) = delete;
  Manager &operator= (Manager &&) = delete;

  /// Initializes the ORB and POA.
  void init (int argc,
             ACE_TCHAR *argv[]);

  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Merges the IORs from the configured servers.
  int make_merged_iors ();

  /// Sets the properties for the profiles.
  int set_properties ();

  /// Writes the merged IOR to a file.
  int write_to_file ();

  /// Returns a pointer to the manager's ORB.
  CORBA::ORB_ptr orb () const;

  Test_Options const &options () const;

private:
  /// The manager's ORB.
  CORBA::ORB_var orb_ {};

  /// Merged IOR set.
  CORBA::Object_var merged_set_ {};

  CORBA::Object_var object_primary_ {};
  CORBA::Object_var object_secondary_ {};
  TAO_IOP::TAO_IOR_Manipulation_var iorm_ {};
  Test_Options options_ {};
};

#endif /* BUG_2247_REGRESSION_MANAGER_H */

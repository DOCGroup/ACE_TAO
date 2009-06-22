// $Id$

#ifndef BUG_2248_REGRESSION_CLIENT_H
#define BUG_2248_REGRESSION_CLIENT_H

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"
#include "tao/IORManipulation/IORManipulation.h"
class Manager
{
public:

  Manager (void);
  // Ctor

  void init (int& argc,
             ACE_TCHAR *argv[]);

  // Initialize the ORB, POA etc.

  int make_merged_iors (void);
  // Merges the different IORS

  /// Run the  ORB event loop..
  int run (void);

  /// Write the merged IOGRs to a file
  int write_to_files (void);

  /// Return the pointer to the copy of our ORB
  CORBA::ORB_ptr orb (void);

private:

  /// Write the merged IOGRs to a file
  int write_to_file (CORBA::Object_ptr ior, const ACE_TCHAR* ior_output_file);

  /// Sets the properties for the profiles
  int set_properties (TAO_IOP::TAO_IOR_Manipulation_ptr iorm,
                      CORBA::Object_ptr merged_set,
                      CORBA::Object_ptr primary, CORBA::ULong version);

  /// Our ORB
  CORBA::ORB_var orb_;

  /// The merged IOR set
  CORBA::Object_var merged_set_10_;
  CORBA::Object_var merged_set_11_;
};

#endif /* BUG_2248_REGRESSION_CLIENT_H */

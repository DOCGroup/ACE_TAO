// -*- C++ -*-
#ifndef BUG_2247_REGRESSION_MANAGER_H
#define BUG_2247_REGRESSION_MANAGER_H

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object.h"

class Manager
{
public:
  Manager ();
  // Ctor

  void init (int argc,
             ACE_TCHAR *argv[]);

  // Initialize the ORB, POA etc.

  int make_merged_iors ();
  // Merges the different IORS

  int set_properties ();
  // Sets the properties for the profiles

  int run ();
  // Run the  ORB event loop..

  int write_to_file ();
  // Write the merged IOR to a file

  CORBA::ORB_ptr orb ();
  // Return the pointer to the copy of our ORB
private:
  CORBA::ORB_var orb_;
  // Our ORB

  CORBA::Object_var merged_set_;
  // The merged IOR set
};

#endif /* BUG_2247_REGRESSION_MANAGER_H */

// -*- C++ -*-
// $Id$

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

  Manager (void);
  // Ctor

  void init (int argc,
             char *argv[]);

  // Initialize the ORB, POA etc.

  int make_merged_iors (void);
  // Merges the different IORS

  int set_properties (void);
  // Sets the properties for the profiles

  int run (void);
  // Run the  ORB event loop..

  int write_to_file (void);
  // Write the merged IOR to a file

  CORBA::ORB_ptr orb (void);
  // Return the pointer to the copy of our ORB
private:
  CORBA::ORB_var orb_;
  // Our ORB

  CORBA::Object_var merged_set_;
  // The merged IOR set
};

#endif /* BUG_2247_REGRESSION_MANAGER_H */

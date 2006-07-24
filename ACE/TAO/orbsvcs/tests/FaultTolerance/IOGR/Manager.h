//$Id$
// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   test
//
// = FILENAME
//    Manager.h
//
// = DESCRIPTION
//    A manager class that merger IORS and designates primary
//
// = AUTHOR
//     Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef TEST_FT_IOGR_MANAGER_H
#define TEST_FT_IOGR_MANAGER_H

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
             char *argv[]
             ACE_ENV_ARG_DECL);

  // Initialize the ORB, POA etc.

  int make_merged_iors (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Merges the different IORS

  int set_properties (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Sets the properties for the profiles

  int run (ACE_ENV_SINGLE_ARG_DECL);
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

#endif /*TEST_FT_IOGR_MANAGER_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file    Manager.h
 *
 *  $Id$
 *
 *  A manager class that merger IORS and designates primary
 *
 *
 *  @author  Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

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
  void init (int argc,
             ACE_TCHAR *argv[]);

  // Initialize the ORB, POA etc.

  /// Merges the different IORS
  int make_merged_iors (void);

  /// Sets the properties for the profiles
  int set_properties (void);

  /// Run the  ORB event loop..
  int run (void);

  /// Write the merged IOR to a file
  int write_to_file (void);

  /// Return the pointer to the copy of our ORB
  CORBA::ORB_ptr orb (void);
private:
  CORBA::Object_var object_primary_;
  CORBA::Object_var object_secondary_;

  /// The merged IOR set
  CORBA::Object_var merged_set_;

  /// Our ORB
  CORBA::ORB_var orb_;
};

#endif /*TEST_FT_IOGR_MANAGER_H */

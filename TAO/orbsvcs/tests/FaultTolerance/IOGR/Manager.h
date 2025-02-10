// -*- C++ -*-

//=============================================================================
/**
 *  @file    Manager.h
 *
 *  A manager class that merger IORS and designates primary
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
  int make_merged_iors ();

  /// Sets the properties for the profiles
  int set_properties ();

  /// Run the  ORB event loop..
  int run ();

  /// Write the merged IOR to a file
  int write_to_file ();

  /// Return the pointer to the copy of our ORB
  CORBA::ORB_ptr orb ();
private:
  CORBA::Object_var object_primary_;
  CORBA::Object_var object_secondary_;

  /// The merged IOR set
  CORBA::Object_var merged_set_;

  /// Our ORB
  CORBA::ORB_var orb_;
};

#endif /*TEST_FT_IOGR_MANAGER_H */

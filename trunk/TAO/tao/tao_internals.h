// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    tao_internals.h
//
// = DESCRIPTION
//    Structures and methods completely internal to TAO.
//
// = AUTHOR
//    Chris Cleeland
// 
// ============================================================================

#if !defined (TAO_INTERNALS_H)
#  define TAO_INTERNALS_H

#  include "ace/Synch.h"

class ACE_Svc_Export TAO_Internal
// = TITLE
//    This class encapsulates some private behaviors and global data
//    structures used internal to TAO.  No application should EVER see
//    this class.
{
public:
  static int open_services (int& argc, char** argv);
  // Initialize the ACE Service Configurator.  This is a one-shot
  // method, i.e., it can be called multiple times but it will only do
  // its work once.  It does, however, track the number of times it's
  // called (see <open_services>).  It is fully thread-safe.  Return 0
  // if successful, -1 with errno set if failure.

  static int close_services (void);
  // The complement to <open_services>, this will perform appropriate
  // ACE Service Configurator closure operations.  It should be called
  // as many times as <open_services>, and will only actually close
  // things down on the last call.  It is fully thread-safe.  Return 0
  // if successful, -1 with errno set if failure.

  static ACE_Thread_Mutex orbinit_lock_;
  // Mutex used as a monitor in CORBA::ORB_init ()
  
protected:
  static int fake_service_entries_i (void);
  // If your platform can't support the Service Configurator's model
  // of loading services found in a file, then you should change the
  // code in here in order to get service entries into the service
  // repository.
  
private:
  TAO_Internal ();
  // Private CTOR prevents this class from being instantiated.

  static ACE_Thread_Mutex service_lock_;
  // Monitor for service config

  static int service_open_count_;
};

#if defined(__ACE_INLINE__)
#  include "tao_internals.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_INTERNALS_H */

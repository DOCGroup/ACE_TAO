// This may look like C, but it's really -*- C++ -*-
// $Id$

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

#  include "tao/corba.h"

class TAO_Export TAO_Internal
  // = TITLE
  //    This class encapsulates some private behaviors and global data
  //    structures used internal to TAO.  No application should EVER
  //    see this class.
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

protected:
  static int fake_service_entries_i (void);
  // If your platform can't support the Service Configurator's model
  // of loading services found in a file, then you should change the
  // code in here in order to get service entries into the service
  // repository.
  
private:
  TAO_Internal (void);
  // Private CTOR prevents this class from being instantiated.

  static int service_open_count_;
  // Number of times <open_services> has been called.  Incremented by
  // <open_services>, and decremented by <close_services>.  Access to
  // this is protected via the <ACE_Static_Object_Lock>.
};

#endif /* TAO_INTERNALS_H */

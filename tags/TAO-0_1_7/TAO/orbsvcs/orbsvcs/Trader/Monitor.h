/* -*- C++ -*- */

// ============================================================================
// $Id$
//
// = LIBRARY
//    orsvcs
// 
// = FILENAME
//    Monitor.h
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//   
// ============================================================================

#ifndef TAO_MONITOR_H
#define TAO_MONITOR_H

template <class TYPE, class TAO_LOCK>
class TAO_Monitor : public TYPE
{
  //
  // = TITLE
  //
  //     A utility class for writing applications where access to an
  //     object must be synchronized in a multithreaded environment.
  //     
  //
  // = DESCRIPTION
  //
  //     A monitor associates a lock of type TAO_LOCK with an object
  //     of type TYPE.  The user is fully responsible for invoking all
  //     necessary acquire/release operations on that lock (i.e. the
  //     monitor does not automate any lock operations - all
  //     operations are invoked explicitly by the user).  The
  //     usefullness of a monitor is in associating one lock with one
  //     object without the need for inheritance or much code.
  //  
  //
public:
  
  typedef TAO_LOCK LOCK_TYPE;
  
  // Return a reference to the lock that I use.
  TAO_LOCK &lock (void)
  {
    return this->lock_;
  }

  // Return a reference to the lock that I use.
  const TAO_LOCK &lock (void) const
  {
    return this->lock_;
  }

protected:

  TAO_LOCK lock_;
  // Lock used to monitor the object.
};

#endif /* #define TAO_MONITOR_H */

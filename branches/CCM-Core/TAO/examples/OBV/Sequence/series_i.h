// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/OBV/Sequence
//
// = FILENAME
//    series_i.h
//
// = DESCRIPTION
//    This class implements the "series" IDL interface.
//
// = AUTHOR
//    Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef SERIES_I_H
#define SERIES_I_H

#include "seriesS.h"

class series_i: public POA_series
{
  // = TITLE
  //    Numerial Series Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that performs various
  //    arithmatic computation using OBV.
public:
  // = Initialization and termination methods.
  series_i (void);
  // Constructor

  series_i (series_i &);
  // Copy Constructor added because g++ generates a
  // bad one if not defined

  ~series_i (void);
  // Destructor

  virtual registry* cube (registry *v, CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the current time/date on the server

  virtual registries *sum (const registries &x, CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const series_i &))
  // Keeping g++2.7.2
};

#endif /* SERIES_I_H */

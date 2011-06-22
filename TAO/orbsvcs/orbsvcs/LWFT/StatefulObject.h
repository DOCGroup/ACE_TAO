// -*- C++ -*-

//=============================================================================
/**
 *  @file    StatefulObject.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _STATEFUL_OBJECT_H_
#define _STATEFUL_OBJECT_H_

#include "ssa_export.h"

// forward declaration
namespace CORBA
{
  class Any;
};

/**
 * @class StatefulObject
 *
 * @brief Abstract Base class for objects that have a set_state method.
 * 
 * This class shields differences in how to disseminate state throughout
 * the system (e.g. either by standard CORBA calls, AMI or DDS) from
 * the state synchronization agent
 */
class SSA_Export StatefulObject
{
public:

  /// ctor
  StatefulObject ();

  /// dtor
  virtual ~StatefulObject ();

  /// method to set the 
  virtual void set_state (const ::CORBA::Any & state_value) = 0;
};

#endif /* _STATEFUL_OBJECT_H_ */

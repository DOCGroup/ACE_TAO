// -*- C++ -*-

//=============================================================================
/**
 *  @file    CorbaStateUpdate.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _CORBA_STATE_UPDATE_H_
#define _CORBA_STATE_UPDATE_H_

#include "StatefulObject.h"
#include "tao/Object.h"

/**
 * @class CorbaStateUpdate
 *
 * @brief Implementation of the StatefulObject interface that uses
 *        a ReplicatedApplication CORBA object to disseminate state information
 */
class CorbaStateUpdate : public StatefulObject
{
public:

  /// ctor
  CorbaStateUpdate (CORBA::Object_ptr application);

  /// dtor
  virtual ~CorbaStateUpdate ();

  /// method to set the 
  virtual void set_state (const ::CORBA::Any & state_value);  

private:
  CORBA::Object_var application_;
};

#endif /* _CORBA_STATE_UPDATE_H_ */

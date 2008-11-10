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
#include "ReplicatedApplicationC.h"

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
  CorbaStateUpdate (ReplicatedApplication_ptr application);

  /// dtor
  virtual ~CorbaStateUpdate ();

  /// method to set the 
  virtual void set_state (const ::CORBA::Any & state_value);  

private:
  ReplicatedApplication_var application_;
};

#endif /* _CORBA_STATE_UPDATE_H_ */

// -*- C++ -*-

//=============================================================================
/**
 * @file    Port_Measurement_Pool.h
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_PORT_MEASUREMENT_POOL_H_
#define _CUTS_PORT_MEASUREMENT_POOL_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "cuts/config.h"
#include "cuts/CUTS_export.h"

#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/RW_Thread_Mutex.h"

class CUTS_Port_Measurement;

//=============================================================================
/**
 * @typedef CUTS_Port_Measurement_Map
 */
//=============================================================================

typedef
ACE_Hash_Map_Manager <long,
                      CUTS_Port_Measurement *,
                      ACE_Null_Mutex> CUTS_Port_Measurement_Map;

//=============================================================================
/**
 * @struct CUTS_Port_Measurement_Node
 */
//=============================================================================

struct CUTS_Port_Measurement_Node
{
  /// The item contained in the node.
  CUTS_Port_Measurement_Map item_;

  /// Pointer to the previous node.
  CUTS_Port_Measurement_Node * prev_;

  /// Pointer to the next node.
  CUTS_Port_Measurement_Node * next_;
};

//=============================================================================
/**
 * @class CUTS_Port_Measurement_Pool
 */
//=============================================================================

class CUTS_Export CUTS_Port_Measurement_Pool :
  protected ACE_Double_Linked_List <CUTS_Port_Measurement_Node>
{
public:
  /// Constructor.
  CUTS_Port_Measurement_Pool (void);

  /**
   * Initializing constructor. This will set the size of port
   * measurement pool.
   */
  CUTS_Port_Measurement_Pool (size_t size);

  /// Destructor.
  ~CUTS_Port_Measurement_Pool (void);

  /**
   * Sets the current CUTS_Port_Measurement object to the next
   * one available in the pool and returns a pointer to the
   * previous current one.
   *
   * @return    Pointer to the previous CUTS_Port_Measurement object.
   */
  CUTS_Port_Measurement_Map & advance (void);

  /**
   * Get a pointer to the port measurement container that is
   * active.
   *
   * @return    Pointer to the current CUTS_Port_Measurement object.
   */
  CUTS_Port_Measurement_Map & current (void);

  /// Get the size of the pool.
  size_t size (void) const;

private:
  /// Pointer to the current node.
  CUTS_Port_Measurement_Node * curr_;

  /// Locking mechanism for thread-safe access.
  ACE_RW_Thread_Mutex lock_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Port_Measurement_Pool.inl"
#endif

#endif  // !defined _CUTS_PORT_MEASUREMENT_POOL_H_

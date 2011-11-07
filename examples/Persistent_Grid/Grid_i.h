// -*- C++ -*-

//=============================================================================
/**
 *  @file    Grid_i.h
 *
 *  $Id$
 *
 *  This class implements the Grid IDL interface.
 *
 *
 */
//=============================================================================


#ifndef GRID_I_H
#define GRID_I_H

#include "GridS.h"
#include "ace/Memory_Pool.h"
#include "ace/Null_Mutex.h"
#include "ace/Malloc_T.h"

typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> pool_t;
// Memory pool for the persistent stuff

//class Grid_Factory_i;
/**
 * @class Grid_Factory_i
 *
 * Create a <Grid>.
 */
class Grid_Factory_i : public POA_Grid_Factory
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Grid_Factory_i (void);

  /// Destructor.
  ~Grid_Factory_i (void);

  /// This function creates and returns a <Grid>.
  virtual Grid_ptr make_grid (CORBA::Short,
                              CORBA::Short);

  /// Shutdown the server.
  virtual void shutdown (void);

  /// Do a clean up of the memory map
  virtual void cleanup ( );

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

  /// Set the pool pointer
  void pool_name (const ACE_TCHAR *name);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// Name of the pool
  ACE_TCHAR *pool_name_;

  /// Hold the pool of name pool_name_
  pool_t *pool_t_;

  void operator= (const Grid_Factory_i &);
};


/**
 * @class Grid_i:
 *
 * @brief Grid object implementation.
 *
 * Grid object implementation
 */
class Grid_i: public POA_Grid
{
public:
  // = Initialization and termination methods.

  /// Constructor
  Grid_i (void);

  /// Constructor.
  Grid_i (CORBA::Short, CORBA::Short, pool_t *);

  /// Destructor
  ~Grid_i (void);

  /// Returns the width of the grid
  virtual CORBA::Short width (void);

  /// Returns the height of the grid
  virtual CORBA::Short height (void);

  /// Sets the width of the grid.
  virtual void width (CORBA::Short);

  /// Sets the height of the grid.
  virtual void height (CORBA::Short);

  /// Sets the grid value.
  virtual void set (CORBA::Short,
                    CORBA::Short,
                    CORBA::Long);

  /// Gets the grid value.
  virtual CORBA::Long get (CORBA::Short,
                           CORBA::Short);

  /// Destroy the grid.
  virtual void destroy (void);

  /// Set a pointer to the pool
  void set_pool (pool_t *);

private:
  /// Width of the grid.
  CORBA::Short width_;

  /// Height of the grid.
  CORBA::Short height_;

  /// Pointer to the matrix.  This is organized as an "array of arrays."
  CORBA::Long **array_;

  ///Pointer to the memory pool..
  pool_t *pool_t_;
};


#endif /* GRID_I_H */

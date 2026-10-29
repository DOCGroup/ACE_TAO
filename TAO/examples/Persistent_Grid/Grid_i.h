// -*- C++ -*-

//=============================================================================
/**
 *  @file    Grid_i.h
 *
 *  This class implements the Grid IDL interface.
 */
//=============================================================================


#ifndef GRID_I_H
#define GRID_I_H

#include "GridS.h"
#include "ace/Memory_Pool.h"
#include "ace/Null_Mutex.h"
#include "ace/Malloc_T.h"

using pool_t = ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
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
  /// Constructor.
  Grid_Factory_i () = default;

  /// Destructor.
  ~Grid_Factory_i ();

  Grid_Factory_i (Grid_Factory_i const&) = delete;
  Grid_Factory_i (Grid_Factory_i&&) = delete;
  Grid_Factory_i& operator= (Grid_Factory_i const&) = delete;
  Grid_Factory_i& operator= (Grid_Factory_i&&) = delete;

  /// This function creates and returns a <Grid>.
  Grid_ptr make_grid (CORBA::Short,
                      CORBA::Short) override;

  /// Shutdown the server.
  void shutdown () override;

  /// Do a clean up of the memory map
  void cleanup () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

  /// Set the pool pointer
  void pool_name (ACE_TCHAR const *name);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// Name of the pool
  ACE_TCHAR *pool_name_ {};

  /// Hold the pool of name pool_name_
  pool_t *pool_t_ {};
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
  /// Constructor
  Grid_i () = default;

  /// Constructor.
  Grid_i (CORBA::Short, CORBA::Short, pool_t *);

  /// Destructor
  ~Grid_i () override = default;

  Grid_i (Grid_i const&) = delete;
  Grid_i (Grid_i&&) = delete;
  Grid_i& operator= (Grid_i const&) = delete;
  Grid_i& operator= (Grid_i&&) = delete;

  /// Returns the width of the grid
  CORBA::Short width () override;

  /// Returns the height of the grid
  CORBA::Short height () override;

  /// Sets the width of the grid.
  void width (CORBA::Short) override;

  /// Sets the height of the grid.
  void height (CORBA::Short) override;

  /// Sets the grid value.
  void set (CORBA::Short,
            CORBA::Short,
            CORBA::Long) override;

  /// Gets the grid value.
  CORBA::Long get (CORBA::Short,
                   CORBA::Short) override;

  /// Destroy the grid.
  void destroy () override;

  /// Set a pointer to the pool
  void set_pool (pool_t *);

private:
  /// Width of the grid.
  CORBA::Short width_ {};

  /// Height of the grid.
  CORBA::Short height_ {};

  /// Pointer to the matrix.  This is organized as an "array of arrays."
  CORBA::Long **array_ {};

  ///Pointer to the memory pool..
  pool_t *pool_t_ {};
};


#endif /* GRID_I_H */

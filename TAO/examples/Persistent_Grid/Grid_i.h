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

#include <memory>
#include <vector>

using pool_t = ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
// Memory pool for the persistent stuff

class Grid_i;
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

  /// Out-of-line because grid_owners_ owns the forward-declared Grid_i.
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
  /// Borrowed from the server's command-line arguments.
  ACE_TCHAR const *pool_name_ {};

  /// Borrowed pointer to the latest pool wrapper, used by cleanup().
  pool_t *pool_t_ {};

  /// Owns pool wrappers while Grid_i servants borrow pointers to them.
  std::vector<std::unique_ptr<pool_t>> pool_owners_;

  /// Keeps activated Grid_i servants alive for the factory's lifetime.
  std::vector<std::unique_ptr<Grid_i>> grid_owners_;
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

private:
  /// Width of the grid.
  CORBA::Short width_ {};

  /// Height of the grid.
  CORBA::Short height_ {};

  /// Pointers into persistent mapped storage, organized as an array of arrays.
  /// Keep this representation because it is persisted in the memory pool.
  CORBA::Long **array_ {};

  /// Borrowed pointer to a pool wrapper kept alive by Grid_Factory_i.
  pool_t *pool_t_ {};
};


#endif /* GRID_I_H */

// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/grid
//
// = FILENAME
//    Grid_i.h
//
// = DESCRIPTION
//    This class implements the Grid IDL interface.
//
// = AUTHOR
//
// ============================================================================

#ifndef GRID_I_H
#define GRID_I_H

#include "GridS.h"
#include "ace/Memory_Pool.h"
#include "ace/Null_Mutex.h"
#include "ace/Malloc_T.h"

typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> pool_t;
// Memory pool for the persistent stuff

//class Grid_Factory_i;
class Grid_Factory_i : public POA_Grid_Factory
{
  // =TITLE
  //   Create a <Grid>.
public:
  // = Initialization and termination methods.
  Grid_Factory_i (void);
  // Constructor.

  ~Grid_Factory_i (void);
  // Destructor.

  virtual Grid_ptr make_grid (CORBA::Short,
                              CORBA::Short);
  // This function creates and returns a <Grid>.

  virtual void shutdown (void);
  // Shutdown the server.

  virtual void cleanup ( );
  // Do a clean up of the memory map

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

  void pool_name (const ACE_TCHAR *name);
  // Set the pool pointer

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  ACE_TCHAR *pool_name_;
  // Name of the pool

  pool_t *pool_t_;
  // Hold the pool of name pool_name_

  ACE_UNIMPLEMENTED_FUNC (void operator= (const Grid_Factory_i &))
  // Keeping g++2.7.2
};


class Grid_i: public POA_Grid
{
  // = TITLE
  //    Grid object implementation.
  //
  // = DESCRIPTION
  //    Grid object implementation
public:
  // = Initialization and termination methods.

  Grid_i (void);
  // Constructor

  Grid_i (CORBA::Short,
          CORBA::Short,
          pool_t *);

  // Constructor.

  ~Grid_i (void);
  // Destructor

  virtual CORBA::Short width (void);
  // Returns the width of the grid

  virtual CORBA::Short height (void);
  // Returns the height of the grid

  virtual void width (CORBA::Short);
  // Sets the width of the grid.

  virtual void height (CORBA::Short);
  // Sets the height of the grid.

  virtual void set (CORBA::Short,
                    CORBA::Short,
                    CORBA::Long);
  // Sets the grid value.

  virtual CORBA::Long get (CORBA::Short,
                           CORBA::Short);
  // Gets the grid value.

  virtual void destroy (void);
  // Destroy the grid.

  void set_pool (pool_t *);
  // Set a pointer to the pool

private:
  CORBA::Short width_;
  // Width of the grid.

  CORBA::Short height_;
  // Height of the grid.

  CORBA::Long **array_;
  // Pointer to the matrix.  This is organized as an "array of arrays."

  pool_t *pool_t_;
  //Pointer to the memory pool..
};


#endif /* GRID_I_H */

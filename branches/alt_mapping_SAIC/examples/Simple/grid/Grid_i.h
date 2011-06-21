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
#include <ace/Vector_T.h>

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
  Grid_i (CORBA::Short,
          CORBA::Short);

  /// Destructor
  virtual ~Grid_i (void);

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

private:
  /// Allocates array
  static CORBA::Long *allocate_array (CORBA::Short x, CORBA::Short y);

  /// Width of the grid.
  CORBA::Short width_;

  /// Height of the grid.
  CORBA::Short height_;

  /// Pointer to the matrix.  This is organized as an "array of arrays."
  typedef ACE_Auto_Array_Ptr<CORBA::Long> GridArray;
  GridArray array_;

  /// Solaris and some Windows compilers don't have min in std namespaces
  static CORBA::UShort ushort_min (CORBA::UShort, CORBA::UShort);
};

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
  virtual ~Grid_Factory_i (void);

  /// This function creates and returns a <Grid>.
  virtual Grid_ptr make_grid (CORBA::Short,
                              CORBA::Short);

  /// Shutdown the server.
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// This container is here only for proper clean up.
  typedef ACE_Vector<PortableServer::ServantBase_var> GridsHolder;
  GridsHolder grids_holder_;

  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// Keeping g++2.7.2
  ACE_UNIMPLEMENTED_FUNC (void operator= (const Grid_Factory_i &))
};

#endif /* GRID_I_H */

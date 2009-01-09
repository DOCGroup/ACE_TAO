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
#include <ace/Vector_T.h>

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
          CORBA::Short);
  // Constructor.

  virtual ~Grid_i (void);
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

private:
  static CORBA::Long *allocate_array (CORBA::Short x, CORBA::Short y);
  // Allocates array

  CORBA::Short width_;
  // Width of the grid.

  CORBA::Short height_;
  // Height of the grid.

  typedef ACE_Auto_Array_Ptr<CORBA::Long> GridArray;
  GridArray array_;
  // Pointer to the matrix.  This is organized as an "array of arrays."

  static CORBA::UShort ushort_min (CORBA::UShort, CORBA::UShort);
  // Solaris and some Windows compilers don't have min in std namespaces
};

class Grid_Factory_i : public POA_Grid_Factory
{
  // =TITLE
  //   Create a <Grid>.
public:
  // = Initialization and termination methods.
  Grid_Factory_i (void);
  // Constructor.

  virtual ~Grid_Factory_i (void);
  // Destructor.

  virtual Grid_ptr make_grid (CORBA::Short,
                              CORBA::Short);
  // This function creates and returns a <Grid>.

  virtual void shutdown (void);
  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  typedef ACE_Vector<PortableServer::ServantBase_var> GridsHolder;
  GridsHolder grids_holder_;
  // This container is here only for proper clean up.

  CORBA::ORB_var orb_;
  // ORB pointer.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const Grid_Factory_i &))
  // Keeping g++2.7.2
};

#endif /* GRID_I_H */

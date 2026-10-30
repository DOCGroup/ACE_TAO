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
#include "ace/Vector_T.h"
#include <memory>

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
  Grid_i (CORBA::Short,
          CORBA::Short);

  /// Destructor
  ~Grid_i () override = default;

  Grid_i (Grid_i const &) = delete;
  Grid_i &operator= (Grid_i const &) = delete;
  Grid_i (Grid_i &&) = delete;
  Grid_i &operator= (Grid_i &&) = delete;

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
  /// Allocates array
  static CORBA::Long *allocate_array (CORBA::Short x, CORBA::Short y);

  /// Width of the grid.
  CORBA::Short width_ {};

  /// Height of the grid.
  CORBA::Short height_ {};

  /// Pointer to the matrix.  This is organized as an "array of arrays."
  using GridArray = std::unique_ptr<CORBA::Long[]>;
  GridArray array_;

  /// Some Windows compilers don't have min in std namespaces
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
  /// Constructor.
  Grid_Factory_i () = default;

  /// Destructor.
  ~Grid_Factory_i () override = default;

  Grid_Factory_i (Grid_Factory_i const &) = delete;
  Grid_Factory_i &operator= (Grid_Factory_i const &) = delete;
  Grid_Factory_i (Grid_Factory_i &&) = delete;
  Grid_Factory_i &operator= (Grid_Factory_i &&) = delete;

  /// This function creates and returns a <Grid>.
  Grid_ptr make_grid (CORBA::Short,
                      CORBA::Short) override;

  /// Shutdown the server.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// This container is here only for proper clean up.
  using GridsHolder = ACE_Vector<PortableServer::ServantBase_var>;
  GridsHolder grids_holder_;

  /// ORB pointer.
  CORBA::ORB_var orb_ {};
};

#endif /* GRID_I_H */

// -*- C++ -*-
// $Id$
#include "Grid_i.h"
#include "tao/corba.h"

// Default constructor.

Grid_i::Grid_i (void)
  : width_ (0),
    height_ (0),
    array_ (0)
{
  //no-op
}

//  Constructor.

Grid_i::Grid_i (CORBA::Short x,
                CORBA::Short y
                ACE_ENV_ARG_DECL)
  : width_ (x),
    height_ (y)
{
  ACE_NEW_THROW_EX (array_,
                    CORBA::Long *[y],
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Allocate memory for the matrix.

  for (int ctr = 0; ctr < y; ctr++)
    {
      ACE_NEW_THROW_EX (array_[ctr],
                        CORBA::Long[x],
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }

}

// Default destructor.

Grid_i::~Grid_i (void)
{
  // no-op.
}

//  Set a value in the grid.

void
Grid_i::set (CORBA::Short x,
             CORBA::Short y,
             CORBA::Long value
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Grid::RANGE_ERROR))
{
  if (x < 0
      || y < 0
      || x >= width_
      || y >= height_)
    ACE_THROW (Grid::RANGE_ERROR ());
  else
    array_[x][y] = value;
}

//  Get a value from  the grid.

CORBA::Long
Grid_i::get (CORBA::Short x,
             CORBA::Short y
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Grid::RANGE_ERROR))
{
  if (x < 0
      || y < 0
      || x >= width_
      || y >= height_)
    ACE_THROW_RETURN (Grid::RANGE_ERROR (), -1);
  else
    return array_[x][y];
}

// Access methods.

CORBA::Short
Grid_i::width (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->width_;
}

CORBA::Short
Grid_i::height (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->height_;
}

void
Grid_i::width (CORBA::Short x
               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->width_ = x;
}

void
Grid_i::height (CORBA::Short y
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->height_ = y;
}

// Destroy the grid

void
Grid_i::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Delete the array.

  for (int i = 0; i < height_; i++)
      delete [] array_[i];

  delete [] array_;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) %s\n",
              "Grid has been destroyed"));
}

// Set the ORB pointer.

void
Grid_Factory_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Shutdown.

void
Grid_Factory_i::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) %s\n",
              "Grid Factory is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

// Constructor

Grid_Factory_i::Grid_Factory_i (void)
{
  // no-op
}

// Copy Constructor

Grid_Factory_i::Grid_Factory_i (Grid_Factory_i &grid)
  :POA_Grid_Factory (grid)
{
  // no-op
}

// Destructor

Grid_Factory_i::~Grid_Factory_i (void)
{
  // no-op
}

// Make a <Grid>.

Grid_ptr
Grid_Factory_i::make_grid (CORBA::Short width,
                           CORBA::Short height
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Grid_i *grid_ptr = 0;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Making a new Grid\n"));

  // Set a default value for width.
  if (width <= 0)
    width = Grid_Factory::DEFAULT_WIDTH;

  // Set a default value for height.
  if (height <= 0)
    height = Grid_Factory::DEFAULT_HEIGHT;

  // This attempts to create a new Grid_i and throws an exception and
  // returns a null value if it fails
  ACE_NEW_THROW_EX (grid_ptr,
                    Grid_i (width, height ACE_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Grid::_nil ());

  // Register the Grid pointer.
  Grid_ptr gptr = grid_ptr->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  return gptr;
}

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
                CORBA::Short y,
                CORBA::Environment &ACE_TRY_ENV)
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
	     CORBA::Long value,
	     CORBA::Environment &ACE_TRY_ENV)
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
             CORBA::Short y,
             CORBA::Environment &ACE_TRY_ENV)
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
Grid_i::width (CORBA::Environment &A)
{
  return this->width_;
}

CORBA::Short
Grid_i::height (CORBA::Environment &)
{
  return this->height_;
}

void
Grid_i::width (CORBA::Short x,
	       CORBA::Environment &)
{
  this->width_ = x;
}

void
Grid_i::height (CORBA::Short y,
		CORBA::Environment &)
{
  this->height_ = y;
}

// Destroy the grid

void
Grid_i::destroy (CORBA::Environment &)
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
Grid_Factory_i::shutdown (CORBA::Environment &)
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

Grid_Factory_i::Grid_Factory_i (Grid_Factory_i &)
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
                           CORBA::Short height,
                           CORBA::Environment &ACE_TRY_ENV)
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
                    Grid_i (width, height, ACE_TRY_ENV),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Grid::_nil ());

  // Register the Grid pointer.
  return grid_ptr->_this (ACE_TRY_ENV);
}

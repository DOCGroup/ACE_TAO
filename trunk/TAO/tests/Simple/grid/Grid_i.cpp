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
                CORBA::Environment &_env)
  : width_ (x),
    height_ (y)
{
  ACE_NEW (array_,
	   CORBA::Long *[y]);

  // Allocate memory for the matrix.

  for (int ctr = 0; ctr < y; ctr++)
    ACE_NEW (array_[ctr],
	     CORBA::Long[x]);
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
	     CORBA::Environment &_env)
{
  if (x < 0
      || y < 0
      || x >= width_
      || y >= height_)
    {
      TAO_THROW (RANGE_ERROR);
    }
  else 
    array_[x][y] = value;
}

//  Get a value from  the grid.

CORBA::Long
Grid_i::get (CORBA::Short x,
             CORBA::Short y,
             CORBA::Environment &_env)
{
  if (x < 0
      || y < 0
      || x >= width_
      || y >= height_)
  {
    TAO_THROW_RETURN (RANGE_ERROR, 0);
  }
  else 
    return array_[x][y];
}

// Access methods.

CORBA::Short 
Grid_i::width (CORBA::Environment &_env)
{
  return this->width_;
}

CORBA::Short 
Grid_i::height (CORBA::Environment &_env)
{
  return this->height_;
}

void 
Grid_i::width (CORBA::Short x,
	       CORBA::Environment &_env)
{
  this->width_ = x;
}

void 
Grid_i::height (CORBA::Short y,
		CORBA::Environment &_env)
{
  this->height_ = y;
}

// Destroy the grid

void 
Grid_i::destroy (CORBA::Environment &_env)
{
  // Delete the array.
  for (int i = 0; i < height_; i++)
      delete [] array_[i];

  delete [] array_;

  ACE_DEBUG ((LM_DEBUG,
              "%s\n",
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
              "%s\n",
              "Grid Factory  is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

// Constructor
Grid_Factory_i::Grid_Factory_i (void)
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
                           CORBA::Environment &_env)
{
  Grid_i *grid_ptr = 0;
 
  ACE_DEBUG ((LM_DEBUG,
              "\n\tMaking a new Grid"));

  // Set a default value for width.
  if (width <= 0)
    width = Grid_Factory::DEFAULT_WIDTH;
 
  // Set a default value for height.
  if (height <= 0)
    height = Grid_Factory::DEFAULT_HEIGHT;

  // This attempts to create a new Grid_i and throws an exception and
  // returns a null value if it fails
  ACE_NEW_THROW_RETURN (grid_ptr,
			Grid_i (width, height, _env),
			CORBA::NO_MEMORY (CORBA::COMPLETED_NO),
			Grid::_nil ());

  // Register the Grid pointer.
  return grid_ptr->_this (_env);
}

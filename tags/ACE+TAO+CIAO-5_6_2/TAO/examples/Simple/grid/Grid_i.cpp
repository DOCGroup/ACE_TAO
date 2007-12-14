// -*- C++ -*-
// $Id$

#include "Grid_i.h"

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
                CORBA::Short y)
  : width_ (x),
    height_ (y)
{
  ACE_NEW_THROW_EX (this->array_,
                    CORBA::Long *[y],
                    CORBA::NO_MEMORY ());

  // Allocate memory for the matrix.

  for (int ctr = 0; ctr < y; ctr++)
    {
      ACE_NEW_THROW_EX (this->array_[ctr],
                        CORBA::Long[x],
                        CORBA::NO_MEMORY ());
    }

}

// Default destructor.

Grid_i::~Grid_i (void)
{
  for (int ctr = 0; ctr < this->height_; ctr++)
    {
      delete [] this->array_[ctr];
    }

  delete [] this->array_;
}

//  Set a value in the grid.

void
Grid_i::set (CORBA::Short x,
             CORBA::Short y,
             CORBA::Long value)
{
  if (x < 0
      || y < 0
      || x >= width_
      || y >= height_)
    throw Grid::RANGE_ERROR ();
  else
    array_[x][y] = value;
}

//  Get a value from  the grid.

CORBA::Long
Grid_i::get (CORBA::Short x,
             CORBA::Short y)
{
  if (x < 0
      || y < 0
      || x >= width_
      || y >= height_)
    throw Grid::RANGE_ERROR ();
  else
    return array_[x][y];
}

// Access methods.

CORBA::Short
Grid_i::width (void)
{
  return this->width_;
}

CORBA::Short
Grid_i::height (void)
{
  return this->height_;
}

void
Grid_i::width (CORBA::Short x)
{
  this->width_ = x;
}

void
Grid_i::height (CORBA::Short y)
{
  this->height_ = y;
}

// Destroy the grid

void
Grid_i::destroy (void)
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
Grid_Factory_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) %s\n",
              "Grid Factory is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

// Constructor

Grid_Factory_i::Grid_Factory_i (void)
  : grid_ (0)
{
  // no-op
}

// Destructor

Grid_Factory_i::~Grid_Factory_i (void)
{
  delete this->grid_;
}

// Make a <Grid>.

Grid_ptr
Grid_Factory_i::make_grid (CORBA::Short width,
                           CORBA::Short height)
{
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
  ACE_NEW_THROW_EX (this->grid_,
                    Grid_i (width, height),
                    CORBA::NO_MEMORY ());

  // Register the Grid pointer.
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id =
    root_poa->activate_object (this->grid_);

  CORBA::Object_var object = root_poa->id_to_reference (id.in ());

  Grid_ptr gptr = Grid::_narrow (object.in ());
  return gptr;
}

// -*- C++ -*-
#include "Grid_i.h"

#include <new>
#include <utility>

//  Constructor.

Grid_i::Grid_i (CORBA::Short x,
                CORBA::Short y,
                pool_t *mem_pool)
  : width_ (x),
    height_ (y),
    pool_t_ (mem_pool)
{
  // First try to locate the matrix in the pool. If it is there then
  // it has already been created. In such a case we just get that
  // memory and assign it to array_

  void *tmp_array {};

  if (mem_pool->find ("Array", tmp_array) != -1)
    {
      array_ = static_cast<CORBA::Long **> (tmp_array);
    }
  else
    {
      // Allocate memory for the matrix.
      ACE_ALLOCATOR (array_,
                     static_cast<CORBA::Long **> (mem_pool->malloc (y * sizeof (CORBA::Long *))));
      if (array_)
        {
          for (int ctr = 0; ctr < y; ctr++)
            {
              ACE_ALLOCATOR (array_[ctr],
                             static_cast<CORBA::Long *> (mem_pool->malloc (x *
                                                                sizeof (CORBA::Long))));
            }

          mem_pool->bind ("Array", array_);
        }
    }

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
Grid_i::width ()
{
  return this->width_;
}

CORBA::Short
Grid_i::height ()
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
Grid_i::destroy ( )
{
  // Delete the array.
  for (int i = 0; i < height_; i++)
    this->pool_t_->free (array_[i]);

  this->pool_t_->free (array_);

  ACE_DEBUG ((LM_DEBUG,
              " (%P|%t) %s\n",
              "Grid has been destroyed"));
}

Grid_Factory_i::~Grid_Factory_i () = default;

// Make a <Grid>.

Grid_ptr
Grid_Factory_i::make_grid (CORBA::Short width,
                           CORBA::Short height)
{
  ACE_DEBUG ((LM_DEBUG,
              " (%P|%t) Making a new Grid\n"));

  // Set a default value for width.
  if (width <= 0)
    width = Grid_Factory::DEFAULT_WIDTH;

  // Set a default value for height.
  if (height <= 0)
    height = Grid_Factory::DEFAULT_HEIGHT;

  // Get a memory pool
  std::unique_ptr<pool_t> pool_owner;
  try
    {
      pool_owner = std::make_unique<pool_t> (pool_name_);
      this->pool_owners_.push_back (std::move (pool_owner));
    }
  catch (std::bad_alloc const &)
    {
      throw CORBA::NO_MEMORY ();
    }
  this->pool_t_ = this->pool_owners_.back ().get ();

  std::unique_ptr<Grid_i> grid_owner;
  try
    {
      grid_owner = std::make_unique<Grid_i> (width, height, this->pool_t_);
      this->grid_owners_.push_back (std::move (grid_owner));
    }
  catch (std::bad_alloc const &)
    {
      throw CORBA::NO_MEMORY ();
    }

  // Register the Grid pointer.
  return this->grid_owners_.back ()->_this ();
}

// Set the ORB pointer.

void
Grid_Factory_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

// Shutdown.
void
Grid_Factory_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              " (%P|%t) %s\n",
              "Grid Factory is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

void
Grid_Factory_i::cleanup ()
{
  char const * const name = "Array";

  if (this->pool_t_->unbind (name) == -1)
    ACE_DEBUG ((LM_DEBUG,
                "\n Failed to unbind "));
}

void
Grid_Factory_i::pool_name (ACE_TCHAR const *name)
{
  this->pool_name_ = name;
}

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
                CORBA::Short y,
                pool_t *mem_pool)
  : width_ (x),
    height_ (y),
    array_ (0),
    pool_t_ (0)
{
  // First try to locate the matrix in the pool. If it is there then
  // it has already been created. In such a case we just get that
  // memory and assign it to array_

  void *tmp_array (0);

  if (mem_pool->find ("Array", tmp_array) != -1)
    {
      array_ = reinterpret_cast <CORBA::Long **> (tmp_array);
    }
  else
    {
      // Allocate memory for the matrix.
      ACE_ALLOCATOR (array_,
                     static_cast<CORBA::Long **> (mem_pool->malloc (y * sizeof (CORBA::Long *))));
      //array_ = (CORBA::Long **) mem_pool->malloc (y * sizeof (CORBA::Long *));

      if (array_ != 0)
        {
          for (int ctr = 0; ctr < y; ctr++)
            {
              ACE_ALLOCATOR (array_[ctr],
                             static_cast<CORBA::Long *> (mem_pool->malloc (x *
                                                                sizeof (CORBA::Long ))));

              //array_[ctr] = (CORBA::Long *)mem_pool->malloc (x *
              //                                            sizeof (CORBA::Long));
            }

          mem_pool->bind ("Array", array_);
        }
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
Grid_i::destroy ( )
{
  // Delete the array.
  for (int i = 0; i < height_; i++)
    this->pool_t_->free (array_[i]);

  //  delete [] array_;
  this->pool_t_->free (array_);

  ACE_DEBUG ((LM_DEBUG,
              " (%P|%t) %s\n",
              "Grid has been destroyed"));
}

void
Grid_i::set_pool (pool_t *pool)
{
  this->pool_t_ = pool;
}
// Constructor

Grid_Factory_i::Grid_Factory_i (void)
  : orb_ (0),
    pool_name_ (0),
    pool_t_ (0)
{
  // no-op
}

// Destructor

Grid_Factory_i::~Grid_Factory_i (void)
{
  delete this->pool_t_;
  // no-op
}

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
  ACE_NEW_THROW_EX (pool_t_,
                    pool_t (pool_name_),
                    CORBA::NO_MEMORY ());

  //  pool_t_ = new pool_t (pool_name_);

  // This attempts to create a new Grid_i and throws an exception and
  // returns a null value if it fails
  int prev_no = errno;
  Grid_i *grid_ptr = new Grid_i (width,
                                 height,
                                 pool_t_);
  if (errno == ENOMEM)
    throw CORBA::NO_MEMORY ();

  errno = prev_no;

  grid_ptr->set_pool (pool_t_);

  // Register the Grid pointer.
  return grid_ptr->_this ();
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
              " (%P|%t) %s\n",
              "Grid Factory is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

void
Grid_Factory_i::cleanup (void)
{
  const char *name = "Array";

  if (this->pool_t_->unbind (name) == -1)
    ACE_DEBUG ((LM_DEBUG,
                "\n Failed to unbind "));
}

void
Grid_Factory_i::pool_name (const ACE_TCHAR *name)
{
  this->pool_name_ = ACE_OS::strdup (name);
}

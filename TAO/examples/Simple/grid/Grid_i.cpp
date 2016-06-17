// -*- C++ -*-
#include "Grid_i.h"

// Solaris and some Windows compilers don't have min in std namespaces
// moreover on Windows 'min' is a macro, so we have to avoid using it literally.
CORBA::UShort
Grid_i::ushort_min (CORBA::UShort a, CORBA::UShort b)
{
  return a > b ? b : a;
}

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
    height_ (y),
    array_ (0)
{
}

// Default destructor.
Grid_i::~Grid_i (void)
{
}

//  Set a value in the grid.
void
Grid_i::set (CORBA::Short x,
             CORBA::Short y,
             CORBA::Long value)
{
  if (x < 0
      || y < 0
      || x >= this->width_
      || y >= this->height_)
    throw Grid::RANGE_ERROR ();
  else
    {
      if (this->array_.get () == 0)
        this->array_.reset (allocate_array (this->width_, this->height_));
      *(this->array_.get () + this->width_ * y + x) = value;
    }
}

//  Get a value from  the grid.
CORBA::Long
Grid_i::get (CORBA::Short x,
             CORBA::Short y)
{
  if (x < 0
      || y < 0
      || x >= this->width_
      || y >= this->height_)
    throw Grid::RANGE_ERROR ();
  else
    {
      if (this->array_.get () == 0)
        this->array_.reset (allocate_array (this->width_, this->height_));
      return *(this->array_.get () + this->width_ * y + x);
    }
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
  if (x > 0 && x != this->width_)
    {
      GridArray array (allocate_array (x, this->height_));
      for (CORBA::Short ctr = 0; ctr < this->height_; ++ctr)
        {
          ACE_OS::memcpy (array.get () + x * ctr, this->array_.get () + this->width_ * ctr,
                          Grid_i::ushort_min (this->width_, x) * sizeof (CORBA::Long));
        }
      this->array_ = array;
      array.release ();
      this->width_ = x;
    }
}

void
Grid_i::height (CORBA::Short y)
{
  if (y > 0 && y != this->height_)
    {
      GridArray array (allocate_array (this->width_, y));
      for (CORBA::Short ctr = 0; ctr < Grid_i::ushort_min (this->height_, y); ++ctr)
        {
          ACE_OS::memcpy (array.get () + this->width_ * ctr, this->array_.get () + this->width_ * ctr,
                          this->width_ * sizeof (CORBA::Long));
        }
      this->array_ = array;
      array.release ();
      this->height_ = y;
    }
}

// Destroy the grid
void
Grid_i::destroy (void)
{
  // Delete the array.
  ACE_Auto_Array_Ptr<CORBA::Long> tmp (this->array_.release ());
  this->width_ = 0;
  this->height_ = 0;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Grid has been destroyed\n")));
}

// Allocates a new array; even if NO_MEMORY is thrown there should be no memory leak
CORBA::Long *
Grid_i::allocate_array (CORBA::Short x, CORBA::Short y)
{
  CORBA::Long *array = 0;
  ACE_NEW_THROW_EX (array,
                    CORBA::Long[x * y],
                    CORBA::NO_MEMORY ());

  return array;
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
              ACE_TEXT ("(%P|%t) Grid Factory is shutting down\n")));

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
}

// Make a <Grid>.
Grid_ptr
Grid_Factory_i::make_grid (CORBA::Short width,
                           CORBA::Short height)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Making a new Grid\n")));

  // Set a default value for width.
  if (width <= 0)
    width = Grid_Factory::DEFAULT_WIDTH;

  // Set a default value for height.
  if (height <= 0)
    height = Grid_Factory::DEFAULT_HEIGHT;

  // This attempts to create a new Grid_i and throws an exception and
  // returns a null value if it fails
  Grid_i *grid = 0;
  ACE_NEW_THROW_EX (grid,
                    Grid_i (width, height),
                    CORBA::NO_MEMORY ());
  this->grids_holder_.push_back (PortableServer::ServantBase_var (grid));

  // Register the Grid pointer.
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::ObjectId_var id =
    root_poa->activate_object (grid);

  CORBA::Object_var object = root_poa->id_to_reference (id.in ());

  return Grid::_narrow (object.in ());
}

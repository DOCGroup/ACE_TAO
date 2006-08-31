// $Id$

#include "cuts/Worker_Library.h"

//
// CUTS_Worker_Library
//
CUTS_Worker_Library::CUTS_Worker_Library (const char * name)
: name_ (name)
{

}

//
// ~CUTS_Worker_Library
//
CUTS_Worker_Library::~CUTS_Worker_Library (void)
{

}

//
// name
//
const char * CUTS_Worker_Library::name (void) const
{
  return this->name_.c_str ();
}

//
// create_worker
//
CUTS_Worker * CUTS_Worker_Library::create_worker (const char * type)
{
  CUTS_Worker * worker = 0;
  WORKER_CREATE_FUNC create_worker_func = 0;

  int result = this->worker_map_.find (type, create_worker_func);

  if (result != -1 && create_worker_func != 0)
  {
    worker = create_worker_func ();
  }

  return worker;
}

//
// release
//
void CUTS_Worker_Library::release (void)
{
  delete this;
}

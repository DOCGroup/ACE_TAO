// $Id$

#include "cuts/Worker.h"

#if !defined (__CUTS_INLINE__)
# include "cuts/Worker.inl"
#endif

#include "cuts/Action_Iterator.h"

//
// CUTS_Worker
//
CUTS_Worker::CUTS_Worker (void)
: parent_ (-1)
{

}

//
// ~CUTS_Worker
//
CUTS_Worker::~CUTS_Worker (void)
{

}

//
// begin
//
CUTS_Action_Iterator * CUTS_Worker::begin (void)
{
  CUTS_Action_Iterator * iter = 0;
  ACE_NEW_RETURN (iter, CUTS_Action_Iterator (), 0);
  return iter;
}

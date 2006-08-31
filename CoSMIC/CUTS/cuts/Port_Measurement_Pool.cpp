// $Id$

#include "cuts/Port_Measurement_Pool.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Port_Measurement_Pool.inl"
#endif

#include "ace/Guard_T.h"

//
// CUTS_Port_Measurement_Pool
//
CUTS_Port_Measurement_Pool::CUTS_Port_Measurement_Pool (void)
{
  this->curr_ = this->head_;
}

//
// CUTS_Port_Measurement_Pool
//
CUTS_Port_Measurement_Pool::CUTS_Port_Measurement_Pool (size_t size)
{
  // Initialize the <curr_> pointer to the <head_> of the
  // list and treat its dummy element as a real one.
  this->curr_ = this->head_;

  // Initialize the size of the pool.
  CUTS_Port_Measurement_Node * node = 0;

  for (size_t i = 1; i < size; i ++)
  {
    ACE_NEW (node, CUTS_Port_Measurement_Node);

    if (node != 0)
    {
      this->insert_tail (node);
    }
  }
}

//
// ~CUTS_Port_Measurement_Pool
//
CUTS_Port_Measurement_Pool::~CUTS_Port_Measurement_Pool (void)
{

}

//
// advance
//
CUTS_Port_Measurement_Map & CUTS_Port_Measurement_Pool::advance (void)
{
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex,
                          guard,
                          this->lock_,
                          this->curr_->item_);

  // Advance the <curr_> pointer to the next node.
  CUTS_Port_Measurement_Node * curr = this->curr_;
  this->curr_ = this->curr_->next_;

  // Return the item of the previously active node.
  return curr->item_;
}

//
// current
//
CUTS_Port_Measurement_Map & CUTS_Port_Measurement_Pool::current (void)
{
  ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex,
                         guard,
                         this->lock_,
                         this->curr_->item_);

  return this->curr_->item_;
}

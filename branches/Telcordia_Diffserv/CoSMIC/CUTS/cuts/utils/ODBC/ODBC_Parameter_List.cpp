// $Id$

#include "ODBC_Parameter_List.h"

#if !defined (__CUTS_INLINE__)
#include "ODBC_Parameter_List.inl"
#endif

#include "ODBC_Parameter.h"
#include "ace/Log_Msg.h"

//
// ODBC_Parameter_List
//
ODBC_Parameter_List::ODBC_Parameter_List (void)
{

}

//
// ODBC_Parameter_List
//
ODBC_Parameter_List::ODBC_Parameter_List (HSTMT handle, size_t n)
: Parameter_Array (n)
{
  if (this->array_ != 0)
  {
    // We need to initialize all the parameters of the array
    // since the list is being constructed for the very first
    // time.
    ODBC_Parameter ** params = this->array_;

    for (size_t i = 0; i < n; )
    {
      ODBC_Parameter * param = 0;
      ACE_NEW (param, ODBC_Parameter (handle, ++ i));

      *params ++ = param;
    }
  }
}

//
// ~ODBC_Parameter_List
//
ODBC_Parameter_List::~ODBC_Parameter_List (void)
{
  if (this->array_ != 0)
  {
    // We have to delete all the parameters in the lists. The number
    // of parameters is not <cur_size_>, but <max_size_> in our case
    // since we save old/unused parameters.
    ODBC_Parameter ** param = this->array_;

    for (size_t i = 0; i < this->max_size_; i ++)
    {
      delete (*param ++);
    }
  }
}

//
// get
//
ODBC_Parameter * ODBC_Parameter_List::get (size_t index)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  if (!this->in_range (index))
  {
    throw CUTS_DB_Exception ("ODBC_Parameter_List::get () : "
                             "index is out of range");
  }

  return this->array_[index];
}

//
// resize
//
void ODBC_Parameter_List::resize (HSTMT handle, size_t n)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  // Get the current <max_size_> of the array and then resize the array
  // to its right size.
  size_t curr_max = this->max_size_;
  this->size (n);

  // If we are adding new parameters to the array then we need to
  // initialize them appropriately, e.g., w/ the correct index.
  if (n > curr_max)
  {
    ODBC_Parameter ** params = this->array_ + curr_max;

    for (size_t i = curr_max; i < n; )
    {
      ODBC_Parameter * param = 0;
      ACE_NEW (param, ODBC_Parameter (handle, ++ i));

      *params ++ = param;
    }
  }
}

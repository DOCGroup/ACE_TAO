// -*- C++ -*-
// $Id$

namespace ADBC
{
//
// message
//
ADBC_INLINE
const ACE_CString & Exception::message (void) const
{
  return this->message_;
}

//
// native
//
ADBC_INLINE
long Exception::native (void) const
{
  return this->native_;
}

//
// state
//
ADBC_INLINE
const ACE_CString & Exception::state (void) const
{
  return this->state_;
}
}

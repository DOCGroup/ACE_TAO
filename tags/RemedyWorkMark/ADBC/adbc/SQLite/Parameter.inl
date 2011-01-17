// -*- C++ -*-
// $Id$

namespace ADBC
{
namespace SQLite
{
//
// Parameter
//
ADBC_INLINE
Parameter::Parameter (void)
: owner_ (0),
  is_null_ (true)
{

}

//
// Parameter
//
ADBC_INLINE
Parameter::
Parameter (const Parameter & p)
: ADBC::Parameter (p),
  owner_ (p.owner_),
  is_null_ (p.is_null_)
{

}

//
// ~Parameter
//
ADBC_INLINE
Parameter::~Parameter (void)
{

}

//
// is_null
//
ADBC_INLINE
bool Parameter::is_null (void) const
{
  return this->is_null_;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (float * value)
{
  this->bind_double (static_cast <double> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (double * value)
{
  this->bind_double (*value);
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT16 * value)
{
  this->bind_int (static_cast <int> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT16 * value)
{
  this->bind_int (static_cast <int> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT32 * value)
{
  this->bind_int (static_cast <int> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT32 * value)
{
  this->bind_int (static_cast <int> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT64 * value)
{
  this->bind_int (static_cast <int> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT64 * value)
{
  this->bind_int (static_cast <int> (*value));
  ::ADBC::Parameter::bind (value);
}

//
// length
//
ADBC_INLINE
long Parameter::length (void)
{
  return 0;
}

//
// reset
//
ADBC_INLINE
void Parameter::
reset (const Parameter_List * owner, int index)
{
  this->owner_ = owner;
  this->index_ = index;
}

}
}

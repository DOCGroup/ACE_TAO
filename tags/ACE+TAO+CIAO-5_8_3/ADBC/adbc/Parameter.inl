// -*- C++ -*-
// $Id$

namespace ADBC
{
//
// Parameter
//
ADBC_INLINE
Parameter::Parameter (void)
: index_ (-1),
  type_ (PT_UNKNOWN)
{

}

//
// Parameter
//
ADBC_INLINE
Parameter::Parameter (const Parameter & p)
: index_ (p.index_),
  type_ (p.type_)
{

}

//
// Parameter
//
ADBC_INLINE
Parameter::~Parameter (void)
{

}

//
// type
//
ADBC_INLINE
Parameter::Parameter_Type Parameter::type (void) const
{
  return this->type_;
}

//
// direction
//
ADBC_INLINE
Parameter::Direction_Type Parameter::direction (void) const
{
  return this->direction_;
}

//
// index
//
ADBC_INLINE
size_t Parameter::index (void) const
{
  return this->index_;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (char *, size_t)
{
  this->type_ = Parameter::PT_CHAR;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (const char *, size_t)
{
  this->type_ = Parameter::PT_CHAR;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT16 *)
{
  this->type_ = Parameter::PT_SHORT;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT16 *)
{
  this->type_ = Parameter::PT_USHORT;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT32 *)
{
  this->type_ = Parameter::PT_LONG;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT32 *)
{
  this->type_ = Parameter::PT_ULONG;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT64 *)
{
  this->type_ = Parameter::PT_LONGLONG;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT64 *)
{
  this->type_ = Parameter::PT_ULONGLONG;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (double *)
{
  this->type_ = Parameter::PT_DOUBLE;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (float *)
{
  this->type_ = Parameter::PT_FLOAT;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (Date_Time *)
{
  this->type_ = Parameter::PT_DATETIME;
}

}

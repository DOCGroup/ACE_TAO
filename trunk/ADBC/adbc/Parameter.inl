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
  type_ (PT_UNKNOWN),
  is_null_ (true)
{

}

//
// Parameter
//
ADBC_INLINE
Parameter::Parameter (const Parameter & p)
: index_ (p.index_),
  type_ (p.type_),
  is_null_ (p.is_null_)
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
Parameter::Parameter_Type
Parameter::type (void) const
{
  return this->type_;
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
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (const char *, size_t)
{
  this->type_ = Parameter::PT_CHAR;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT16 *)
{
  this->type_ = Parameter::PT_SHORT;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT16 *)
{
  this->type_ = Parameter::PT_USHORT;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT32 *)
{
  this->type_ = Parameter::PT_LONG;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT32 *)
{
  this->type_ = Parameter::PT_ULONG;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_INT64 *)
{
  this->type_ = Parameter::PT_LONGLONG;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (ACE_UINT64 *)
{
  this->type_ = Parameter::PT_ULONGLONG;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (double *)
{
  this->type_ = Parameter::PT_DOUBLE;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (float *)
{
  this->type_ = Parameter::PT_FLOAT;
  this->is_null_ = false;
}

//
// bind
//
ADBC_INLINE
void Parameter::bind (Date_Time *)
{
  this->type_ = Parameter::PT_DATETIME;
  this->is_null_ = false;
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
// null
//
ADBC_INLINE
void Parameter::null (void)
{
  this->is_null_ = true;
}

//
// direction
//
ADBC_INLINE
Parameter::Direction_Type
Parameter::direction (void) const
{
  return this->direction_;
}
}

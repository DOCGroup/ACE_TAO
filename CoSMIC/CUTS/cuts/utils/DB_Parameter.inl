// $Id$

//
// type
//
CUTS_INLINE
CUTS_DB_Parameter::Parameter_Type
CUTS_DB_Parameter::type (void) const
{
  return this->type_;
}

//
// index
//
CUTS_INLINE
size_t CUTS_DB_Parameter::index (void) const
{
  return this->index_;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (char *, size_t)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_CHAR;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (short *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_SHORT;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (u_short *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_USHORT;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (long *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_LONG;
}

//
// bind
//
void CUTS_DB_Parameter::bind (u_long *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_ULONG;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (double *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_DOUBLE;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (float *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_FLOAT;
}

//
// bind
//
CUTS_INLINE
void CUTS_DB_Parameter::bind (CUTS_DB_Date_Time_Impl *)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->type_ = CUTS_DB_Parameter::PT_DATETIME;
}

//
// is_null
//
CUTS_INLINE
int CUTS_DB_Parameter::is_null (void) const
{
  return this->null_;
}

//
// null
//
CUTS_INLINE
void CUTS_DB_Parameter::null (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  this->null_ = 1;
}

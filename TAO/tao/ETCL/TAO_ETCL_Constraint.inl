// -*- C++ -*-
// ****************************************************************

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint ()
  : any_ (nullptr)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  ETCL_Literal_Constraint const* lit)
  : ETCL_Literal_Constraint (*lit),
    any_ (nullptr)
{}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::ULong uinteger)
  : ETCL_Literal_Constraint (uinteger),
    any_ (nullptr)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Long integer)
  : ETCL_Literal_Constraint (integer),
    any_ (nullptr)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Boolean boolean)
  : ETCL_Literal_Constraint (boolean),
    any_ (nullptr)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Double doub)
  : ETCL_Literal_Constraint (doub),
    any_ (nullptr)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  char const* str)
  : ETCL_Literal_Constraint (str),
    any_ (nullptr)
{
}


// -*- C++ -*-
// ****************************************************************

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint () = default;

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  ETCL_Literal_Constraint const* lit)
  : ETCL_Literal_Constraint (*lit)
{}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::ULong uinteger)
  : ETCL_Literal_Constraint (uinteger)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Long integer)
  : ETCL_Literal_Constraint (integer)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Boolean boolean)
  : ETCL_Literal_Constraint (boolean)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Double doub)
  : ETCL_Literal_Constraint (doub)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  char const* str)
  : ETCL_Literal_Constraint (str)
{
}


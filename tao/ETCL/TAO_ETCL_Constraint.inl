// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (void)
  : any_ (0)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  const ETCL_Literal_Constraint *lit)
  : ETCL_Literal_Constraint (*lit),
    any_ (0)
{}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::ULong uinteger)
  : ETCL_Literal_Constraint (uinteger),
    any_ (0)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Long integer)
  : ETCL_Literal_Constraint (integer),
    any_ (0)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Boolean boolean)
  : ETCL_Literal_Constraint (boolean),
    any_ (0)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  CORBA::Double doub)
  : ETCL_Literal_Constraint (doub),
    any_ (0)
{
}

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (
  const char* str)
  : ETCL_Literal_Constraint (str),
    any_ (0)
{
}


// $Id$

ACE_INLINE TAO::BufferingConstraint
TAO_Buffering_Constraint_Policy::buffering_constraint (CORBA::Environment &)
{
  return this->buffering_constraint_;
}

ACE_INLINE TAO::BufferingConstraint
TAO_Buffering_Constraint_Policy::buffering_constraint (void)
{
  return this->buffering_constraint_;
}


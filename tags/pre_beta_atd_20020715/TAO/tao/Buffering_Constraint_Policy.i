// $Id$

ACE_INLINE void
TAO_Buffering_Constraint_Policy::get_buffering_constraint (TAO::BufferingConstraint &bc) const
{
  bc = this->buffering_constraint_;
}

// $Id$

ACE_INLINE TAO_EC_Conjunction_Filter::ChildrenIterator
TAO_EC_Conjunction_Filter::begin (void) const
{
  return this->children_;
}

ACE_INLINE TAO_EC_Conjunction_Filter::ChildrenIterator
TAO_EC_Conjunction_Filter::end (void) const
{
  return this->children_ + this->n_;
}

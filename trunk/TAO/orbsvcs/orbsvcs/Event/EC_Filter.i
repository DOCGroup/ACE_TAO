// $Id$

ACE_INLINE
TAO_EC_Filter::TAO_EC_Filter (void)
  :  parent_ (0)
{
}

ACE_INLINE TAO_EC_Filter*
TAO_EC_Filter::parent (void) const
{
  return this->parent_;
}

ACE_INLINE int
TAO_EC_Filter::matches (const RtecEventComm::EventHeader& rhs,
                        const RtecEventComm::EventHeader& lhs)
{
  if ((rhs.source == 0 && rhs.type == 0)
      || (lhs.source == 0 && lhs.type == 0))
    return 1;

  if (rhs.source == 0 || lhs.source == 0)
    return rhs.type == lhs.type;

  if (rhs.type == 0 || lhs.type == 0)
    return rhs.source == lhs.source;

  return (rhs.type == lhs.type && rhs.source == lhs.type);
}

// ****************************************************************

ACE_INLINE
TAO_EC_Null_Filter::TAO_EC_Null_Filter (void)
{
}

// ****************************************************************

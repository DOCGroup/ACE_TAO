// $Id$

ACE_INLINE
TAO_NS_Refcountable_Guard::TAO_NS_Refcountable_Guard (TAO_NS_Refcountable& refcountable)
  :refcountable_ (refcountable)
{
  this->refcountable_._incr_refcnt ();
}

ACE_INLINE
TAO_NS_Refcountable_Guard::~TAO_NS_Refcountable_Guard ()
{
  this->refcountable_._decr_refcnt ();
}

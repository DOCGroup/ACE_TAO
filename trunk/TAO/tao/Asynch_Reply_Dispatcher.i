// $Id$

#if (TAO_HAS_MINIMUM_CORBA == 0)

ACE_INLINE void
TAO_Asynch_Reply_Dispatcher_Base::transport (TAO_Transport *t)
{
  this->transport_ = t;
}

#endif /* (TAO_HAS_MINIMUM_CORBA == 0) */


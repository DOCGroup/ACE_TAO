// $Id$

template<class TYPE, class INTERFACE, class INTERFACE_PTR, class EXCEPTION> ACE_INLINE void
TAO_Notify_Find_Worker_T<TYPE,INTERFACE,INTERFACE_PTR,EXCEPTION>::work (TYPE* type ACE_ENV_ARG_DECL_NOT_USED)
{
  if (type->id () == this->id_)
    this->result_ = type;
}

// $Id$

/*************************************************************************************************/

template<class OBJECT> ACE_INLINE OBJECT*
TAO_NS_Object_Find_Worker_T<OBJECT>::find ( ACE_TYPENAME TAO_NS_Object_Find_Worker_T::COLLECTION* collection ACE_ENV_ARG_DECL)
{
  collection->for_each (this ACE_ENV_ARG_PARAMETER);
  return this->result_;
}

template<class OBJECT> ACE_INLINE void
TAO_NS_Object_Find_Worker_T<OBJECT>::work (OBJECT* object ACE_ENV_ARG_DECL_NOT_USED)
{
  if (object->id () == this->id_)
    this->result_ = object;
}

/*************************************************************************************************/

template<class TYPE, class OBJECT, class PARENT>  ACE_INLINE TYPE*
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::find (TAO_NS_Object_Id id ACE_ENV_ARG_DECL)
{
  FIND_WORKER find_worker (id);

  return find_worker.find (this->collection_ ACE_ENV_ARG_PARAMETER);
}

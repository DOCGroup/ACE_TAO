/* -*- C++ -*- */
// $Id$

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_TCHAR *name)
{
  ACE_Service_Object *svc_obj = 
    ACE_static_cast (ACE_Service_Object *, ACE_Dynamic_Service_Base::instance (name));
  return ACE_dynamic_cast (TYPE *, svc_obj);
}

/*
 *  XercesSystem inline methods
 */

inline xercesc::DOMImplementation*
XercesSystem::impl()
{
  return impl_;
}

inline xercesc::DOMImplementationLS*
XercesSystem::impl_ls()
{
  return impl_ls_;
}

inline xercesc::DOMBuilder*
XercesSystem::new_parser()
{
  return
    impl_ls_->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS,
                               0);
}

/*
 *  XercesAutoPtr inline methods
 */

template <typename T> inline
XercesAutoPtr<T>::XercesAutoPtr(typename XercesAutoPtr<T>::element_type* t) throw()
  : t_(t)
{
}

template <typename T> inline
XercesAutoPtr<T>::XercesAutoPtr(XercesAutoPtr<T>& p) throw()
  : t_(t)
{
  p->t_ = 0;
}

template <typename T> inline XercesAutoPtr<T>&
XercesAutoPtr<T>::operator=(XercesAutoPtr<T>& p)
  throw()
{
  if (t_)
    t_->release();
  t_ = p.t_;
  p.t_ = 0;
  return *this;
}

template <typename T> inline
XercesAutoPtr<T>::~XercesAutoPtr()
{
  if (t_)
    t_->release();
}

template <typename T> inline typename XercesAutoPtr<T>::element_type&
XercesAutoPtr<T>::operator*() const throw()
{
  return *t_;
}

template <typename T> inline typename XercesAutoPtr<T>::element_type*
XercesAutoPtr<T>::operator->() const throw()
{
  return t_;
}

template <typename T> inline typename XercesAutoPtr<T>::element_type*
XercesAutoPtr<T>::get() const throw()
{
  return t_;
}

template <typename T> inline typename XercesAutoPtr<T>::element_type*
XercesAutoPtr<T>::release() throw()
{
  T* tmp = t_;
  t_ = 0;
  return tmp;
}
  
template <typename T> inline void
XercesAutoPtr<T>::reset(typename XercesAutoPtr<T>::element_type* p) throw()
{
  if (t_)
    t_->release();
  t_ = p;
}

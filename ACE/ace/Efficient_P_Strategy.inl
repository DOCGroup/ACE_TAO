
template <typename AnnotationId>
ACE_INLINE 
Efficient_P_Strategy<AnnotationId>::Efficient_P_Strategy(int maxThreads)
:DA_Strategy_Base(maxThreads),
 t_r(maxThreads),
 p_r(maxThreads),
 _lock()
{
}

template <typename AnnotationId>
ACE_INLINE 
Efficient_P_Strategy<AnnotationId>::~Efficient_P_Strategy()
{
}

template <typename AnnotationId>
ACE_INLINE 
bool 
Efficient_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
  int annotation = get_annotation(handle);
  int min_illegal = 0;
  if (t_r != 0) {
    min_illegal = p_r;
  }
  return !(annotation < min_illegal);
}

template <typename AnnotationId>
ACE_INLINE 
void 
Efficient_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
  int annotation = get_annotation(handle);
  {
    ACE_Guard<ACE_Thread_Mutex> guard(_lock);
    t_r--;
    if (annotation > 1) 
    {
      p_r--;
    }   
  }
}

template <typename AnnotationId>
ACE_INLINE 
void 
Efficient_P_Strategy<AnnotationId>::release(AnnotationId upcall_handle)
{
  int annotation = get_annotation(upcall_handle);
  {
    ACE_Guard<ACE_Thread_Mutex> guard(_lock);
    t_r ++;
    if (annotation > 1) 
    {
      p_r ++;
    }  
  }
}
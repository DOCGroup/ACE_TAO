
template <typename AnnotationId>
ACE_INLINE 
Basic_P_Strategy<AnnotationId>::Basic_P_Strategy(int maxThreads)
:DA_Strategy_Base<AnnotationId>(maxThreads),
 t_r(maxThreads)
{
}

template <typename AnnotationId>
ACE_INLINE 
Basic_P_Strategy<AnnotationId>::~Basic_P_Strategy()
{

}

template <typename AnnotationId>
ACE_INLINE 
bool Basic_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
  int annotation = get_annotation(handle);
  return !(annotation < t_r);
}

template <typename AnnotationId>
ACE_INLINE 
void Basic_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
  --t_r;
}

template <typename AnnotationId>
ACE_INLINE 
void Basic_P_Strategy<AnnotationId>::release(AnnotationId upcall_handle)
{
  ++t_r;
}

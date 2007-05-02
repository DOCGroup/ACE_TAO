
template <typename AnnotationId, int k>
ACE_INLINE 
k_Efficient_P_Strategy<AnnotationId, k>::k_Efficient_P_Strategy(int maxThreads)
:DA_Strategy_Base(maxThreads),
  {
    a.resize(k + 1);
    A.resize(k + 1);
    for (unsigned i=0; i<k; ++i) {
      a[i] = 0;
      A[i] = 0;
    }
    min_ilegal_ = maxThreads;
    min_ilegal_is_computed_ = true;
}

template <typename AnnotationId, int k>
ACE_INLINE 
k_Efficient_P_Strategy<AnnotationId, k>::~k_Efficient_P_Strategy()
{

}

template <typename AnnotationId, int k>
ACE_INLINE 
bool k_Efficient_P_Strategy<AnnotationId, k>::is_deadlock_potential(AnnotationId handle)
{
  int annotation = get_annotation(handle);
  return (annotation >= min_ilegal());
}

template <typename AnnotationId, int k>
ACE_INLINE
int
k_Efficient_P_Strategy<AnnotationId, k>::compute_min_illegal() 
{
  int T = get_max_threads();
  for (int i=0; i<k; ++i) {
    if (!(A[i] < (T - i))) {
      return i;
    }
  }
  if (A[k]>0) {
    return (T - A[k]);
  } 
  return  T;
}

template <typename AnnotationId, int k>
ACE_INLINE
int
k_Efficient_P_Strategy<AnnotationId, k>::get_min_illegal() 
{
  if (!min_illegal_is_computed_) {
	  min_illegal_ = compute_min_ilegal();
	  min_illegal_is_computed_ = true;
  }
  return min_illegal_;
}

template <typename AnnotationId, int k>
ACE_INLINE 
void k_Efficient_P_Strategy<AnnotationId, k>::grant(AnnotationId handle)
{
    int annotation = get_annotation(handle);

    if (annotation < k) 
    {
      a[annotation] ++;
      for (int i=0; i<=annotation; ++i) 
      {
	      A[i]++;
      }
    } 
    else
    {
      a[k] ++;
      for (int i=0; i<=k ; ++i)
      {
	      A[i]++;
      }
    }
    min_ilegal_is_computed_ = false;
    return true;
}

template <typename AnnotationId, int k>
ACE_INLINE 
void k_Efficient_P_Strategy<AnnotationId, k>::release(AnnotationId handle)
{
    int annotation = get_annotation(handle);
/*    if (annotation < k ) {
      assert(a[annotation]>0);
    } else {
      assert(a[k] >0);
    }
*/
    if (annotation < k)
    {
      a[annotation] --;
      for (unsigned i=0; i<=annotation; ++i) 
      {
	      A[i]--;
      }
    } 
    else 
    {
      a[k] --;
      for (unsigned i=0; i<=k ; ++i) 
      {
	      A[i]--;
      }
    }
    min_ilegal_is_computed_ = false;
}
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename AnnotationId>
ACE_INLINE 
k_Efficient_P_Strategy<AnnotationId>::k_Efficient_P_Strategy(int maxThreads, int k)
:DA_Strategy_Base<AnnotationId>(maxThreads),
 k_(k)
  {
    a.resize(k_ + 1);
    A.resize(k_ + 1);
    for (int i=0; i<k_; ++i) {
      a[i] = 0;
      A[i] = 0;
    }
    min_illegal_ = maxThreads;
    min_illegal_is_computed_ = true;
}

template <typename AnnotationId>
ACE_INLINE 
k_Efficient_P_Strategy<AnnotationId>::~k_Efficient_P_Strategy()
{

}

template <typename AnnotationId>
ACE_INLINE 
bool k_Efficient_P_Strategy<AnnotationId>::is_deadlock_potential(AnnotationId handle)
{
  int annotation = DA_Strategy_Base<AnnotationId>::get_annotation(handle);
  return (annotation >= get_min_illegal());
}

template <typename AnnotationId>
ACE_INLINE
int
k_Efficient_P_Strategy<AnnotationId>::compute_min_illegal() 
{
  int T = this->get_max_threads();
  for (int i=0; i<k_; ++i) {
    if (!(A[i] < (T - i))) {
      return i;
    }
  }
  if (A[k_]>0) {
    return (T - A[k_]);
  } 
  return  T;
}

template <typename AnnotationId>
ACE_INLINE
int
k_Efficient_P_Strategy<AnnotationId>::get_min_illegal() 
{
  computation_mutex_.acquire();
  if (!min_illegal_is_computed_) {
	  min_illegal_ = compute_min_illegal();
	  min_illegal_is_computed_ = true;
  }
  computation_mutex_.release();
  return min_illegal_;
}

template <typename AnnotationId>
ACE_INLINE 
void k_Efficient_P_Strategy<AnnotationId>::grant(AnnotationId handle)
{
    int annotation = get_annotation(handle);
    computation_mutex_.acquire();
    if (annotation < k_) 
    {
      a[annotation] ++;
      for (int i=0; i<=annotation; ++i) 
      {
	      A[i]++;
      }
    } 
    else
    {
      a[k_] ++;
      for (int i=0; i<=k_ ; ++i)
      {
	      A[i]++;
      }
    }
    min_illegal_is_computed_ = false;
    computation_mutex_.release();
}

template <typename AnnotationId>
ACE_INLINE 
void k_Efficient_P_Strategy<AnnotationId>::release(AnnotationId handle)
{
    int annotation = get_annotation(handle);
    computation_mutex_.acquire();
/*    if (annotation < k ) {
      assert(a[annotation]>0);
    } else {
      assert(a[k] >0);
    }
*/
    if (annotation < k_)
    {
      a[annotation] --;
      for (int i=0; i<=annotation; ++i) 
      {
	      A[i]--;
      }
    } 
    else 
    {
      a[k_] --;
      for (int i=0; i<=k_ ; ++i) 
      {
	      A[i]--;
      }
    }
    min_illegal_is_computed_ = false;
    computation_mutex_.release();
}


ACE_END_VERSIONED_NAMESPACE_DECL
template <typename AnnotationId>
ACE_INLINE 
DA_Strategy_Base<AnnotationId>::DA_Strategy_Base (int maxThreads)
  :num_avail_threads_ (maxThreads)
{
}

template <typename AnnotationId>
ACE_INLINE
DA_Strategy_Base<AnnotationId>::~DA_Strategy_Base()
{
}

template <typename AnnotationId>
ACE_INLINE int 
DA_Strategy_Base<AnnotationId>::get_annotation (AnnotationId id) const
{
  int annotation;
  if (annotations_repo_.find (id, annotation) == -1)
    return -1;
  else return annotation;
}

template <typename AnnotationId>
ACE_INLINE int 
DA_Strategy_Base<AnnotationId>::set_annotations_table (
        const HASH_ANNOTATIONS_REVERSE_ITER& table)
{
  HASH_ANNOTATIONS_REVERSE_ITER iter(table);
  int rc=0;
  
  for (;!(iter.done()); iter++)
    {
      rc = annotations_repo_.bind((*iter).ext_id_, (*iter).int_id_);
      if (rc != 0) break;
    }

  return rc;
}

template <typename AnnotationId>
ACE_INLINE int 
DA_Strategy_Base<AnnotationId>::add_annotation (AnnotationId id, int annotation)
{
  int rc;
  if (annotation > num_avail_threads_.value()) {
    rc = -1;
    ACE_ERROR ((LM_ERROR,
               ACE_TEXT ("%p.\n"),
               ACE_TEXT ("DA_Strategy_Base annotation may not exceed number of threads")));
  } else {
    rc = annotations_repo_.bind (id, annotation);
  }
  /*  
    ACE_DEBUG ((LM_DEBUG, "In add_annotation\n"));
  HASH_ANNOTATIONS_CONST_ITER iter(annotations_repo_);
  for (;!(iter.done()); iter++)
    {
      ACE_DEBUG ((LM_DEBUG, "%d-%d\n", (*iter).ext_id_, (*iter).int_id_));
    }
  */
  return rc;
}

template <typename AnnotationId>
ACE_INLINE ACE_Hash_Map_Const_Iterator_Ex<AnnotationId,
                                       int,
                                       ACE_Hash<AnnotationId>,
                                       ACE_Equal_To<AnnotationId>,
                                       ACE_Thread_Mutex>
DA_Strategy_Base<AnnotationId>::get_annotations_iter() const
{
  
  return annotations_repo_.begin();
}

template <typename AnnotationId>
ACE_INLINE int  
DA_Strategy_Base<AnnotationId>::remove_annotation (AnnotationId id)
{
  return annotations_repo_.unbind (id);
}


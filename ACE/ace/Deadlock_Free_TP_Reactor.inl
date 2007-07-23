/* -*- C++ -*- */
// TP_Reactor.inl,v 4.2 2004/08/20 15:21:02 bala Exp
/*
ACE_INLINE 
ACE_Deadlock_Free_TP_Reactor::ACE_Deadlock_Free_TP_Reactor (ACE_Sig_Handler *sh,
                                                            ACE_Timer_Queue *tq,
                                                            int mask_signals,
                                                            int s_queue,
                                                            int maxThreads,
                                                            int k)
  :ACE_TP_Reactor (sh, tq, mask_signals, s_queue),
    strategy_(maxThreads)
{
}
*/

namespace DA_Reactor_Init {

   size_t size = 0;
   DA_Strategy_Base<ACE_HANDLE>* da_strategy = 0;
   int rs = 0;
   ACE_Sig_Handler* sh = 0;
   ACE_Timer_Queue* tq = 0;
   int mask_signals = 0;
   int s_queue = 0;

}

ACE_INLINE
void
ACE_Deadlock_Free_TP_Reactor_Impl::initialize (size_t size,
                                               DA_Strategy_Base<ACE_HANDLE>* da_strategy,
                                               int rs,
                                               ACE_Sig_Handler *sh,
                                               ACE_Timer_Queue *tq,
                                               int mask_signals,
                                               int s_queue)
{
  DA_Reactor_Init::size = size;
  DA_Reactor_Init::da_strategy = da_strategy;
  DA_Reactor_Init::rs = rs;
  DA_Reactor_Init::sh = sh;
  DA_Reactor_Init::tq = tq;
  DA_Reactor_Init::mask_signals = mask_signals;
  DA_Reactor_Init::s_queue = s_queue;
}


ACE_INLINE
ACE_Deadlock_Free_TP_Reactor_Impl::ACE_Deadlock_Free_TP_Reactor_Impl()
  :ACE_TP_Reactor (DA_Reactor_Init::size, 
                   DA_Reactor_Init::rs, 
                   DA_Reactor_Init::sh, 
                   DA_Reactor_Init::tq, 
                   DA_Reactor_Init::mask_signals, 
                   DA_Reactor_Init::s_queue),
    da_strategy_(DA_Reactor_Init::da_strategy)
{
}

ACE_INLINE
ACE_Deadlock_Free_TP_Reactor_Impl::~ACE_Deadlock_Free_TP_Reactor_Impl()
{
  //delete da_strategy_;
}

ACE_INLINE void
ACE_Deadlock_Free_TP_Reactor_Impl::pre_upcall_hook (ACE_HANDLE upcall_handle)
{
  da_strategy_->grant(upcall_handle);

  //ACE_DEBUG ((LM_DEBUG, "%d: Protocol Entry - Before hook\n", gettid()));
  //ACE_DEBUG ((LM_DEBUG, "%d: Dispatching handle %d\n", gettid(), upcall_handle));
  
  this->filter_deadlock_potential_handles (upcall_handle);
}

ACE_INLINE void
ACE_Deadlock_Free_TP_Reactor_Impl::post_upcall_hook (ACE_HANDLE upcall_handle)
{
  da_strategy_->release(upcall_handle);

  //ACE_DEBUG ((LM_DEBUG, "%d: Protocol Exit - After hook\n", gettid()));
    
  this->filter_deadlock_potential_handles (upcall_handle);
}

ACE_INLINE int 
ACE_Deadlock_Free_TP_Reactor_Impl::get_annotation (ACE_Event_Handler* eh)
{
  return da_strategy_->get_annotation(eh->get_handle());
}

ACE_INLINE int 
ACE_Deadlock_Free_TP_Reactor_Impl::add_annotation (ACE_Event_Handler* eh, int annotation)
{
  return da_strategy_->add_annotation(eh->get_handle(), annotation);
}

ACE_INLINE int  
ACE_Deadlock_Free_TP_Reactor_Impl::remove_annotation (ACE_Event_Handler* eh)
{
  return da_strategy_->remove_annotation(eh->get_handle());
}

ACE_INLINE int 
ACE_Deadlock_Free_TP_Reactor_Impl::filter_deadlock_potential_handles(ACE_HANDLE upcall_handle)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_RW_Thread_Mutex, ace_mon, this->lock_, 0));

  ACE_Event_Handler *eh = 0;
  
  //ACE_HANDLE h;
  /*
  ACE_DEBUG ((LM_DEBUG, "%d: Before filter\n", gettid()));

  ACE_DEBUG ((LM_DEBUG, "%d: read_handle = ", gettid() ));
  for (ACE_Handle_Set_Iterator handle_iter_rd (this->wait_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "%d,", h));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  
  ACE_DEBUG ((LM_DEBUG, "%d: suspend_handle = ", gettid() ));
  for (ACE_Handle_Set_Iterator handle_iter_rd (this->suspend_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "%d,", h));
  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_DEBUG ((LM_DEBUG, "%d: ready_handle = ", gettid() ));
  for (ACE_Handle_Set_Iterator handle_iter_rd (this->ready_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "%d,", h));
  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_DEBUG ((LM_DEBUG, "num_avail_threads = %d\n", num_avail_threads_.value()));
  */  
 
  ACE_Hash_Map_Const_Iterator_Ex<ACE_HANDLE,
                                 int,
                                 ACE_Hash<ACE_HANDLE>,
                                 ACE_Equal_To<ACE_HANDLE>,
                                 ACE_Thread_Mutex>  iter = da_strategy_->get_annotations_iter();
  ACE_Hash_Map_Entry<ACE_HANDLE, int>* entry = 0;

  for (;iter.next(entry) != 0; iter.advance()) 
  {
    ACE_HANDLE handle = entry->key();
    int annotation = entry->item();

    if (handle == upcall_handle) continue;
    if (da_strategy_->is_deadlock_potential(handle)) 
    {
      if (!(this->is_suspended_i(handle)))
      {
        this->suspend_i(handle);
      }
    } 
    else
    {
      if (this->is_suspended_i(handle))
      {
        this->resume_i(handle);
      }
    }

    /* this is the old way, provided for comparison - PAO

    for (ACE_Select_Reactor_Handler_Repository_Iterator iter (&this->handler_rep_);
       iter.next (eh) != 0;
       iter.advance ())
    {
      int annotation = get_annotation(eh);
      if (eh->get_handle () == upcall_handle) continue;
      if (annotation <= 0) continue;
      if (annotation > this->num_avail_threads_.value () && 
	        !(this->is_suspended_i (eh->get_handle ())))
	       this->suspend_i (eh->get_handle ());
      else if (annotation  <= num_avail_threads_.value () && 
	             this->is_suspended_i (eh->get_handle ()))
	       this->resume_i (eh->get_handle ());
*/
// metrics commented out - PAO 4/10/07
//      this->da_metrics_->number_of_handles_worked_on_[this->da_metrics_->index] = 
//	this->da_metrics_->number_of_handles_worked_on_[this->da_metrics_->index]+1;
      
      //      ACE_DEBUG ((LM_DEBUG, "%d: this->da_metrics_->num_handles_worked_on[%d] = %d\n",
      //		  gettid(), this->da_metrics_->index,
      //		  this->da_metrics_->number_of_handles_worked_on_[this->da_metrics_->index]));
      //ACE_DEBUG ((LM_DEBUG, "%d: da_metrics_->index = %d\n", gettid (), (*(this->da_metrics_))->index));

    }

  /*  
  ACE_DEBUG ((LM_DEBUG, "%d: After filter\n", gettid()));
  ACE_DEBUG ((LM_DEBUG, "%d: read_handle = ", gettid() ));
  for (ACE_Handle_Set_Iterator handle_iter_rd (this->wait_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "%d,", h));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  
  ACE_DEBUG ((LM_DEBUG, "%d: suspend_handle = ", gettid() ));
  for (ACE_Handle_Set_Iterator handle_iter_rd (this->suspend_set_.rd_mask_);
       (h = handle_iter_rd ()) != ACE_INVALID_HANDLE;
       ++handle_iter_rd)
    ACE_DEBUG ((LM_DEBUG, "%d,", h));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  */  
  return 0;
}

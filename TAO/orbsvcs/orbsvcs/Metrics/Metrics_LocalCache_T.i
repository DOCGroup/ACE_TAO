// $Id$

#ifndef METRICS_LOCAL_CACHE_T_I
#define METRICS_LOCAL_CACHE_T_I

//////////////////////////////////////
// class TAO_Metrics_LocalTimeprobe //
//////////////////////////////////////

// Installs or unistalls the data set into which probe will report.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR>::
install_data_set (Metrics::TimeprobeParameter_Set *data_set)
{
   data_set_ = data_set;
}


// Returns the last timestamp that was recorded in the probe.
// If no stamps are recorded, returns zero.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE ACE_Time_Value
TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR>::last_recorded_timestamp (void)
{
   return (this->current_size_ > 0)
      ? this->timeprobes ()[this->current_size_ - 1].time_
      : ((report_buffer_full_ == 0)
      ? ACE_Time_Value::zero
      : this->timeprobes ()[this->max_size_ - 1].time_);
}

//////////////////////////////////
// Class TAO_Metrics_LocalCache //
//////////////////////////////////


// Default constructor.
template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
TAO_Metrics_LocalCache (u_long table_size,
                        u_long number_of_probes,
                        ALLOCATOR * alloc)
  : ACE_Metrics_Cache<ACE_LOCK, ALLOCATOR> (table_size, number_of_probes, alloc),
    sched_count_ (0),
    quo_count_ (0),
    tns_count_ (0),
    teec_count_ (0),
    rtarm_count_ (0),
    upcall_adapter_count_ (0),
    next_base_metrics_handle_ (1)
{
   // Ensure that the high res timer global scale factor
   // is set before any of its static methods are used
   ACE_High_Res_Timer::global_scale_factor ();

   // Allocate the probe arrays, based on passed count
   if (init (number_of_probes) != 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
         "Metrics_Metrics::Metrics_Metrics init failed"));
#endif
   }
}

// Factory method for constructing probes.
template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE int
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::init (u_long number_of_probes)
{
   if (number_of_probes == 0
      || number_of_probes >= TAO_Metrics_Utils::METRICS_DEQUEUE_PROBE_ID_BASE)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
         "TAO_Metrics_LocalCache::init "
         "number_of_probes out of range"));
#endif
      return -1;
   }

   // Allocate the probe arrays, based on passed count

   u_long *count_temp = 0;
   ACE_NEW_MALLOC_ARRAY_RETURN (count_temp,
                                (u_long *) this->allocator ()->
                                malloc (sizeof (u_long)
                                * number_of_probes),
                                u_long,
                                number_of_probes,
                                -1);
   enqueue_count_ [0] = count_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (count_temp,
                                (u_long *) this->allocator ()->
                                malloc (sizeof (u_long)
                                * number_of_probes),
                                u_long,
                                number_of_probes,
                                -1);
   enqueue_count_ [1] = count_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (count_temp,
                                (u_long *) this->allocator ()->
                                malloc (sizeof (u_long)
                                * number_of_probes),
                                u_long,
                                number_of_probes,
                                -1);
   dequeue_count_ [0] = count_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (count_temp,
                                (u_long *) this->allocator ()->
                                malloc (sizeof (u_long)
                                * number_of_probes),
                                u_long,
                                number_of_probes,
                                -1);
   dequeue_count_ [1] = count_temp;

   ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE * probes_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (probes_temp,
                                (ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE*)
                                this->allocator ()->
                                malloc (sizeof(ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE)
                                * number_of_probes),
                                ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE,
                                number_of_probes,
                                -1);
   enqueue_probes_ [0] = probes_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (probes_temp,
                                (ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE*)
                                this->allocator ()->
                                malloc (sizeof(ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE)
                                * number_of_probes),
                                ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE,
                                number_of_probes,
                                -1);
   enqueue_probes_ [1] = probes_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (probes_temp,
                                (ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE*)
                                this->allocator ()->
                                malloc (sizeof(ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE)
                                * number_of_probes),
                                ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE,
                                number_of_probes,
                                -1);
   dequeue_probes_ [0] =  probes_temp;

   ACE_NEW_MALLOC_ARRAY_RETURN (probes_temp,
                                (ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE*)
                                this->allocator ()->
                                malloc (sizeof(ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE)
                                * number_of_probes),
                                ACE_METRICS_TIMEPROBE_BASED_PTR_TYPE,
                                number_of_probes,
                                -1);
   dequeue_probes_ [1] = probes_temp;

   u_int i;
   for (i = 0; i < number_of_probes; ++i)
   {
      TAO_METRICS_TIMEPROBE_TYPE *timeprobe_temp;
      ACE_NEW_MALLOC_RETURN (timeprobe_temp,
                             (TAO_METRICS_TIMEPROBE_TYPE *) this->
                             allocator ()->
                             malloc (sizeof (TAO_METRICS_TIMEPROBE_TYPE)),
                             TAO_METRICS_TIMEPROBE_TYPE,
                             -1);
      // Modified by BRM.  Not sure what the equivalent for ACE_Based_Pointers is.
      //      enqueue_probes_ [0].addr () [i] = timeprobe_temp;
      enqueue_probes_ [0][i] = timeprobe_temp;

      ACE_NEW_MALLOC_RETURN (timeprobe_temp,
                             (TAO_METRICS_TIMEPROBE_TYPE *) this->
                             allocator ()->
                             malloc (sizeof (TAO_METRICS_TIMEPROBE_TYPE)),
                             TAO_METRICS_TIMEPROBE_TYPE,
                             -1);
      // Modified by BRM.  Not sure what the equivalent for ACE_Based_Pointers is.
      //      enqueue_probes_ [1].addr () [i] = timeprobe_temp;
      enqueue_probes_ [1][i] = timeprobe_temp;

      ACE_NEW_MALLOC_RETURN (timeprobe_temp,
                             (TAO_METRICS_TIMEPROBE_TYPE *) this->
                             allocator ()->
                             malloc (sizeof (TAO_METRICS_TIMEPROBE_TYPE)),
                             TAO_METRICS_TIMEPROBE_TYPE,
                             -1);
      // Modified by BRM.  Not sure what the equivalent for ACE_Based_Pointers is.
      //      dequeue_probes_ [0].addr () [i] = timeprobe_temp;
      dequeue_probes_ [0][i] = timeprobe_temp;

      ACE_NEW_MALLOC_RETURN (timeprobe_temp,
                             (TAO_METRICS_TIMEPROBE_TYPE *) this->
                             allocator ()->
                             malloc (sizeof (TAO_METRICS_TIMEPROBE_TYPE)),
                             TAO_METRICS_TIMEPROBE_TYPE,
                             -1);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      dequeue_probes_ [1].addr () [i] = timeprobe_temp;
      dequeue_probes_ [1][i] = timeprobe_temp;
   }

   ACE_METRICS_STRING_TYPE * names_temp;
   u_int j = 0;
   names_temp =
      (ACE_METRICS_STRING_TYPE*) this->allocator ()->
      malloc (sizeof (ACE_METRICS_STRING_TYPE) * number_of_probes);
   if (names_temp == 0)
   {
      errno = ENOMEM;
      return -1;
   }
   else
   {
      // Explicit constructor invocation using array placement
      // new, to work around broken placement array new on MSVC++
      for (j = 0; j < number_of_probes; ++j)
      {
         new (names_temp + j) ACE_METRICS_STRING_TYPE;
      }
   }
   enqueue_names_ = names_temp;

   names_temp =
      (ACE_METRICS_STRING_TYPE*) this->allocator ()->
      malloc (sizeof (ACE_METRICS_STRING_TYPE) * number_of_probes);
   if (names_temp == 0)
   {
      errno = ENOMEM;
      return -1;
   }
   else
   {
      // Explicit constructor invocation using array placement
      // new, to work around broken placement array new on MSVC++
      for (u_int j = 0; j < number_of_probes; ++j)
      {
         new (names_temp + j) ACE_METRICS_STRING_TYPE;
      }
   }
   dequeue_names_ = names_temp;

   // Allocate a single block for names, into which both
   // enqueue and dequeue names arrays will point.
   char *temp;
   ACE_NEW_MALLOC_ARRAY_RETURN (temp,
                                (char *) this->allocator ()->
                                malloc (TAO_Metrics_Utils::METRICS_BUFSIZE
                                * 2 * number_of_probes),
                                char,
                                TAO_Metrics_Utils::METRICS_BUFSIZE
                                * 2 * number_of_probes,
                                -1);

   // Iterate to initialize the arrays
   for (i = 0; i < number_of_probes; ++i)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      enqueue_names_.addr () [i] =
      enqueue_names_[i] = temp + i * TAO_Metrics_Utils::METRICS_BUFSIZE;
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      dequeue_names_.addr () [i] =
      dequeue_names_[i] = temp + (i + number_of_probes) * TAO_Metrics_Utils::METRICS_BUFSIZE;

      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_count_ [0].addr () [i] = 0;
      this->enqueue_count_ [0][i] = 0;
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_count_ [1].addr () [i] = 0;
      this->enqueue_count_ [1] [i] = 0;
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      ACE_OS::memset (this->enqueue_names_.addr () [i].addr (),
      ACE_OS::memset (this->enqueue_names_ [i],
                      0, TAO_Metrics_Utils::METRICS_BUFSIZE);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      ACE_OS::sprintf (this->enqueue_names_.addr () [i].addr (),
      ACE_OS::sprintf (this->enqueue_names_ [i],
                       "enqueue_%d", i);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_probes_ [0].addr () [i].addr ()->
      this->enqueue_probes_ [0] [i]->
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //        probe_name (this->enqueue_names_.addr () [i].addr ());
         probe_name (this->enqueue_names_ [i]);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_probes_ [1].addr () [i].addr ()->
      this->enqueue_probes_ [1] [i]->
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //        probe_name (this->enqueue_names_.addr () [i].addr ());
         probe_name (this->enqueue_names_ [i]);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_probes_ [0].addr () [i].addr ()->
      this->enqueue_probes_ [0] [i]->
         probe_id (TAO_Metrics_Utils::METRICS_ENQUEUE_PROBE_ID_BASE + i);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_probes_ [1].addr () [i].addr ()->
      this->enqueue_probes_ [1] [i]->
         probe_id (TAO_Metrics_Utils::METRICS_ENQUEUE_PROBE_ID_BASE + i);

      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_probes_ [0].addr () [i].addr ()->
      this->enqueue_probes_ [0] [i]->
         increase_size (this->table_size_);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->enqueue_probes_ [1].addr () [i].addr ()->
      this->enqueue_probes_ [1] [i]->
         increase_size (this->table_size_);

      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_count_ [0].addr() [i] = 0;
      this->dequeue_count_ [0][i] = 0;
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_count_ [1].addr () [i] = 0;
      this->dequeue_count_ [1] [i] = 0;
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      ACE_OS::memset (this->dequeue_names_.addr () [i].addr (),
      ACE_OS::memset (this->dequeue_names_ [i],
                      0, TAO_Metrics_Utils::METRICS_BUFSIZE);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      ACE_OS::sprintf (this->dequeue_names_.addr () [i].addr (),
      ACE_OS::sprintf (this->dequeue_names_ [i],
         "dequeue_%d", i);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_probes_ [0].addr () [i].addr ()->
      this->dequeue_probes_ [0] [i]->
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //        probe_name (this->dequeue_names_.addr () [i].addr ());
         probe_name (this->dequeue_names_ [i]);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_probes_ [1].addr () [i].addr ()->
      this->dequeue_probes_ [1] [i]->
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //        probe_name (this->dequeue_names_.addr () [i].addr ());
         probe_name (this->dequeue_names_ [i]);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_probes_ [0].addr () [i].addr ()->
      this->dequeue_probes_ [0] [i]->
         probe_id (TAO_Metrics_Utils::METRICS_DEQUEUE_PROBE_ID_BASE + i);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_probes_ [1].addr () [i].addr ()->
      this->dequeue_probes_ [1] [i]->
         probe_id (TAO_Metrics_Utils::METRICS_DEQUEUE_PROBE_ID_BASE + i);

      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_probes_ [0].addr () [i].addr ()->
      this->dequeue_probes_ [0] [i]->
         increase_size (this->table_size_);
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->dequeue_probes_ [1].addr () [i].addr ()->
      this->dequeue_probes_ [1] [i]->
         increase_size (this->table_size_);
   }

   // If we got here we're pretty much OK.
   this->probe_set_size_ = number_of_probes;

   return 0;
}


// Method for destroying probes.
template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::fini ()
{
   if (this->probe_set_size_ > 0)
   {
      // Destroy the monolithic block of storage allocated to hold all names.
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      if (enqueue_names_.addr () [0].addr ())
      if (enqueue_names_ [0])
      {
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          this->allocator ()->free (enqueue_names_.addr () [0].addr ());
         this->allocator ()->free (enqueue_names_ [0]);
      }
   }

   // Destroy storage allocated to hold counts.
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (enqueue_count_ [0].addr ())
   if (enqueue_count_ [0])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (enqueue_count_ [0].addr ());
      this->allocator ()->free (enqueue_count_ [0]);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (enqueue_count_ [1].addr ())
   if (enqueue_count_ [1])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (enqueue_count_ [1].addr ());
      this->allocator ()->free (enqueue_count_ [1]);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (dequeue_count_ [0].addr())
   if (dequeue_count_ [0])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (dequeue_count_ [0].addr());
      this->allocator ()->free (dequeue_count_ [0]);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (dequeue_count_ [1].addr())
   if (dequeue_count_ [1])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (dequeue_count_ [1].addr());
      this->allocator ()->free (dequeue_count_ [1]);
   }

   // Destroy the actual probes
   TAO_METRICS_TIMEPROBE_TYPE *probe_ptr;
   for (u_int i = 0; i < this->probe_set_size_; ++i)
   {
      //DCL:Change to static_cast for now to remove Linux compile error
      //probe_ptr = ACE_dynamic_cast (TAO_METRICS_TIMEPROBE_TYPE *,
      //                              enqueue_probes_ [0].addr () [i].addr ());
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //    probe_ptr = static_cast<TAO_METRICS_TIMEPROBE_TYPE *>(enqueue_probes_ [0].addr () [i].addr ());
      probe_ptr = static_cast<TAO_METRICS_TIMEPROBE_TYPE *>(enqueue_probes_ [0] [i]);
      ACE_DES_FREE (probe_ptr,
                    this->allocator ()->free,
                    TAO_METRICS_TIMEPROBE_TYPE);
      //probe_ptr = ACE_dynamic_cast (TAO_METRICS_TIMEPROBE_TYPE *,
      //                              enqueue_probes_ [1].addr () [i].addr ());
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //    probe_ptr = static_cast<TAO_METRICS_TIMEPROBE_TYPE *>(enqueue_probes_ [1].addr () [i].addr ());
      probe_ptr = static_cast<TAO_METRICS_TIMEPROBE_TYPE *>(enqueue_probes_ [1] [i]);
      ACE_DES_FREE (probe_ptr,
                    this->allocator ()->free,
                    TAO_METRICS_TIMEPROBE_TYPE);
      //probe_ptr = ACE_dynamic_cast (TAO_METRICS_TIMEPROBE_TYPE *,
      //                              dequeue_probes_ [0].addr () [i].addr ());
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //    probe_ptr = static_cast <TAO_METRICS_TIMEPROBE_TYPE *>(dequeue_probes_ [0].addr () [i].addr ());
      probe_ptr = static_cast <TAO_METRICS_TIMEPROBE_TYPE *>(dequeue_probes_ [0] [i]);
      ACE_DES_FREE (probe_ptr,
                    this->allocator ()->free,
                    TAO_METRICS_TIMEPROBE_TYPE);
      //probe_ptr = ACE_dynamic_cast (TAO_METRICS_TIMEPROBE_TYPE *,
      //                              dequeue_probes_ [1].addr () [i].addr ());
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //    probe_ptr = static_cast <TAO_METRICS_TIMEPROBE_TYPE *>(dequeue_probes_ [1].addr () [i].addr ());
      probe_ptr = static_cast <TAO_METRICS_TIMEPROBE_TYPE *>(dequeue_probes_ [1] [i]);
      ACE_DES_FREE (probe_ptr,
         this->allocator ()->free,
         TAO_METRICS_TIMEPROBE_TYPE);
   }

   // Destroy storage allocated to hold probe pointers.
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (enqueue_probes_ [0].addr ())
   if (enqueue_probes_ [0])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (enqueue_probes_ [0].addr ());
      this->allocator ()->free (enqueue_probes_ [0]);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (enqueue_probes_ [1].addr ())
   if (enqueue_probes_ [1])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (enqueue_probes_ [1].addr ());
      this->allocator ()->free (enqueue_probes_ [1]);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (dequeue_probes_ [0].addr ())
   if (dequeue_probes_ [0])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (dequeue_probes_ [0].addr ());
      this->allocator ()->free (dequeue_probes_ [0]);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (dequeue_probes_ [1].addr ())
   if (dequeue_probes_ [1])
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (dequeue_probes_ [1].addr ());
      this->allocator ()->free (dequeue_probes_ [1]);
   }

   // Destroy storage for string pointers
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (enqueue_names_.addr ())
   if (enqueue_names_)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (enqueue_names_.addr ());
      this->allocator ()->free (enqueue_names_);
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (dequeue_names_.addr ())
   if (dequeue_names_)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      this->allocator ()->free (dequeue_names_.addr ());
      this->allocator ()->free (dequeue_names_);
   }

   TAO_Metrics_Cache_Data_Base * base_data = 0;

   for (METRICS_BASE_MONITOR_MAP_ITERATOR base_iter_0 (this->
                                                       base_monitor_maps_ [0]);
        base_iter_0.done () == 0;
        ++base_iter_0)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      base_data = (*base_iter_0).int_id_.addr ();
      base_data = (*base_iter_0).int_id_;
      if (base_data)
      {
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          if (base_data->probe_name.addr ())
         if (base_data->probe_name)
         {
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //              this->allocator ()->free (base_data->probe_name.addr ());
            this->allocator ()->free (base_data->probe_name);
         }

         ACE_DES_FREE (base_data,
                       this->allocator ()->free,
                       TAO_Metrics_Cache_Data_Base);
      }
   }

   for (METRICS_BASE_MONITOR_MAP_ITERATOR
        base_iter_1 (this->base_monitor_maps_ [1]);
        base_iter_1.done () == 0;
        ++base_iter_1)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      base_data = (*base_iter_1).int_id_.addr ();
      base_data = (*base_iter_1).int_id_;
      if (base_data)
      {
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          if (base_data->probe_name.addr ())
         if (base_data->probe_name)
         {
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //              this->allocator ()->free (base_data->probe_name.addr ());
            this->allocator ()->free (base_data->probe_name);
         }

         ACE_DES_FREE (base_data,
                       this->allocator ()->free,
                       TAO_Metrics_Cache_Data_Base);
      }
   }

   TAO_Metrics_Cache_Data * data = 0;

   for (METRICS_MONITOR_MAP_ITERATOR data_iter_0 (this->monitor_maps_ [0]);
        data_iter_0.done () == 0;
        ++data_iter_0)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      data = (*data_iter_0).int_id_.addr ();
      data = (*data_iter_0).int_id_;
      if (data)
      {
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          if (data->probe_name.addr ())
         if (data->probe_name)
         {
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //              this->allocator ()->free (data->probe_name.addr ());
            this->allocator ()->free (data->probe_name);
         }

         ACE_DES_FREE (data,
                       this->allocator ()->free,
                       TAO_Metrics_Cache_Data);
      }
   }

   for (METRICS_MONITOR_MAP_ITERATOR data_iter_1 (this->monitor_maps_ [1]);
        data_iter_1.done () == 0;
        ++data_iter_1)
   {
      // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
      //      data = (*data_iter_1).int_id_.addr ();
      data = (*data_iter_1).int_id_;
      if (data)
      {
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          if (data->probe_name.addr ())
         if (data->probe_name)
         {
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //              this->allocator ()->free (data->probe_name.addr ());
            this->allocator ()->free (data->probe_name);
         }

         ACE_DES_FREE (data,
                       this->allocator ()->free,
                       TAO_Metrics_Cache_Data);
      }
   }
}

// Registers an upcall adapter, allocates monitoring data structures
// for it in hash maps if not already present.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
register_upcall_adapter (RtecScheduler::handle_t handle,
                         const char *name,
                         int is_hrt,
                         unsigned short incr_upcall_id)
{
   ACE_METRICS_CACHE_DATA_TYPE data = 0;
   TAO_Metrics_Cache_Data * data_temp;
   for (u_int i = 0; i < 2; ++i)
   {
      // Look for an existing data structure entry in each hash map.
      if (monitor_maps_ [i].find (handle, data) != 0
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          || (data_temp = data.addr ()) == 0)
         || (data_temp = data) == 0)
      {
         // Create and bind if not there
         ACE_NEW_MALLOC (data_temp,
                         (TAO_Metrics_Cache_Data *) this->allocator ()->
                         malloc (sizeof (TAO_Metrics_Cache_Data)),
                         TAO_Metrics_Cache_Data);
         data = data_temp;

         char * probe_name_temp;
         ACE_NEW_MALLOC_ARRAY (probe_name_temp,
                               (char *) this->allocator ()->
                               malloc (TAO_Metrics_Utils::METRICS_BUFSIZE),
                               char,
                               TAO_Metrics_Utils::METRICS_BUFSIZE);
         data_temp->probe_name = probe_name_temp;

         ACE_OS::memset (data_temp->
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //                            probe_name.addr (), 0, TAO_Metrics_Utils::METRICS_BUFSIZE);
            probe_name, 0, TAO_Metrics_Utils::METRICS_BUFSIZE);

         ACE_OS::strncpy (data_temp->
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //                             probe_name.addr (), name, TAO_Metrics_Utils::METRICS_BUFSIZE-1);
            probe_name, name, TAO_Metrics_Utils::METRICS_BUFSIZE-1);
         data_temp->probe.probe_name (data_temp->
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //                                                  probe_name.addr ());
            probe_name);

         data_temp->probe.mark_as_operation ();
         data_temp->probe.set_hrt (is_hrt);

         data_temp->is_hrt = is_hrt;

         data_temp->probe.probe_id (this->upcall_adapter_count_
            + TAO_Metrics_Utils::METRICS_UPCALL_MONITOR_ADAPTER_ID_BASE);

         data_temp->missed_deadlines = 0;
         data_temp->made_deadlines = 0;
         data_temp->cancellations = 0;

         if (monitor_maps_ [i].rebind (handle, data) < 0)
         {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
            ACE_ERROR ((LM_ERROR,
               "TAO_Metrics_LocalCache::register_upcall_adapter"
               " bind failed.\n"));
#endif
            return;
         }

      }
   }

   if (incr_upcall_id)
   {
      this->increment_upcall_handle();
   }
}



// Registers a base metrics probe, allocates monitoring data structures
// for it in hash maps, returns a handle to it.
template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE RtecScheduler::handle_t
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
register_base_metrics (const char *name,
                       TAO_Metrics_Utils::Base_Metrics_Type bmt,
                       unsigned short incr_base_id)
{
   TAO_Metrics_Cache_Data_Base* base_data = 0;
   TAO_Metrics_Cache_Data_Base * base_data_temp = 0;
   RtecScheduler::handle_t handle =
      this->next_base_metrics_handle_++;

   for (u_int i = 0; i < 2; ++i)
   {
      // Look for an existing data structure entry in each hash map.
      if (base_monitor_maps_ [i].find (handle, base_data) != 0
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //          || (base_data_temp = base_data.addr ()) == 0)
         || (base_data_temp = base_data) == 0)
      {
         // Create and bind if not there
         ACE_NEW_MALLOC_RETURN (base_data_temp,
                                (TAO_Metrics_Cache_Data_Base *)
                                 this->allocator ()->malloc
                                 (sizeof (TAO_Metrics_Cache_Data_Base)),
                                 TAO_Metrics_Cache_Data_Base,
                                 RtecScheduler::handle_t (-1));
         base_data = base_data_temp;

         char * probe_name_temp;
         ACE_NEW_MALLOC_ARRAY_RETURN (probe_name_temp,
                                      (char *) this->allocator ()->malloc
                                      (TAO_Metrics_Utils::METRICS_BUFSIZE),
                                      char,
                                      TAO_Metrics_Utils::METRICS_BUFSIZE,
                                      RtecScheduler::handle_t (-1));
         base_data_temp->probe_name = probe_name_temp;

         ACE_OS::memset (base_data_temp->
                         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
                         //                            probe_name.addr (), 0, TAO_Metrics_Utils::METRICS_BUFSIZE);
                         probe_name, 0, TAO_Metrics_Utils::METRICS_BUFSIZE);
         ACE_OS::strncpy (base_data_temp->
                          // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
                          //                             probe_name.addr (), name, TAO_Metrics_Utils::METRICS_BUFSIZE-1);
                          probe_name, name, TAO_Metrics_Utils::METRICS_BUFSIZE-1);
         base_data_temp->probe.probe_name (base_data_temp->
                                           // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
                                           //                                                  probe_name.addr ());
                                           probe_name);

         switch (bmt)
         {
         case TAO_Metrics_Utils::SCHED:
            base_data_temp->
               probe.probe_id (this->sched_count_
               + TAO_Metrics_Utils::METRICS_SCHED_PROBE_ID_BASE);
            break;
         case TAO_Metrics_Utils::QUO:
            base_data_temp->
               probe.probe_id (this->quo_count_
               + TAO_Metrics_Utils::METRICS_QUO_PROBE_ID_BASE);
            break;
         case TAO_Metrics_Utils::TNS:
            base_data_temp->
               probe.probe_id (this->tns_count_
               + TAO_Metrics_Utils::METRICS_TNS_PROBE_ID_BASE);
            break;
         case TAO_Metrics_Utils::TEEC:
            base_data_temp->
               probe.probe_id (this->teec_count_
               + TAO_Metrics_Utils::METRICS_TEEC_PROBE_ID_BASE);
            break;
         case TAO_Metrics_Utils::RTARM:  // treat RTARM as the default case
         default:
            base_data_temp->
               probe.probe_id (this->rtarm_count_
               + TAO_Metrics_Utils::METRICS_RTARM_PROBE_ID_BASE);
            break;
         }

         if (base_monitor_maps_ [i].rebind (handle, base_data) < 0)
         {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
            ACE_ERROR ((LM_ERROR,
               "TAO_Metrics_LocalCache::register_base_metrics"
               " bind failed.\n"));
#endif
         }

      }
   }

   if (incr_base_id == 1)
   {
      increment_base_handle(bmt);
   }
   return handle;
}


// Inline: Records start time of an upcall.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::report_upcall_start (RtecScheduler::handle_t handle)
{
   ACE_METRICS_CACHE_DATA_TYPE data = 0;
   TAO_Metrics_Cache_Data * data_tmp = 0;
   if (monitor_maps_ [supplier_index_].find (handle, data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_start:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((data_tmp = data.addr ()) == 0)
   else if ((data_tmp = data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_start:\n"
                  "Data pointer bound for handle %d is 0.\n",
                   handle));
#endif
   }
   else
   {
      if (! this->interval_initialized_)
      {
         this->interval_initialized_ = 1;
         ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
         ACE_High_Res_Timer::hrtime_to_tv (this->interval_start_,
                                           hrtime_now);
         this->interval_end_.set (this->interval_start_.sec (),
                                  this->interval_start_.usec ());
      }

      // Take timeprobe stamp
      data_tmp->probe.timeprobe (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::
                                 WORK_START);
   }
}


// Inline: Records end time of an upcall, returns 0 if the deadline
// was made, 1 if the deadline was missed, or -1 if an error occurred.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE int
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
report_upcall_stop (RtecScheduler::handle_t handle,
                    const ACE_Time_Value & deadline)
{
   // Capture current time value.
   ACE_Time_Value current_time;
   ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
   ACE_High_Res_Timer::hrtime_to_tv (current_time, hrtime_now);

   ACE_METRICS_CACHE_DATA_TYPE data = 0;
   TAO_Metrics_Cache_Data * data_tmp = 0;
   if (monitor_maps_ [supplier_index_].find (handle, data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_stop:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
      return -1;
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((data_tmp = data.addr ()) == 0)
   else if ((data_tmp = data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                 "TAO_Metrics_LocalCache::report_upcall_stop:\n"
                 "Data pointer bound for handle %d is 0.\n",
                  handle));
#endif
      return -1;
   }
   else
   {
      // Take timeprobe stamp
      data_tmp->probe.timeprobe (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::
                                 WORK_STOP);

      this->interval_end_ =
         data_tmp->probe.last_recorded_timestamp ();

      // Check time value, record whether we made or missed deadline.
      if (current_time <= deadline || deadline == ACE_Time_Value::zero)
      {
         ++data_tmp->made_deadlines;
         return 0;
      }
      else
      {
         ++data_tmp->missed_deadlines;
#if defined (METRICS_CACHE_DEBUG_OUTPUT_ENABLED)
         ACE_DEBUG((LM_DEBUG,
                    "Missed Deadline: deadline = %u:%u, "
                    "current time = %u:%u\n",
                    deadline.sec (),
                    deadline.usec (),
                    current_time.sec (),
                    current_time.usec ()));
#endif
         return 1;
      }
   }
}

// Inline: Records cancelled upcall.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
report_upcall_cancellation (RtecScheduler::handle_t handle)
{
   ACE_METRICS_CACHE_DATA_TYPE data = 0;
   TAO_Metrics_Cache_Data * data_tmp = 0;
   if (monitor_maps_ [supplier_index_].find (handle, data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_cancellation:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((data_tmp = data.addr ()) == 0)
   else if ((data_tmp = data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_cancellation:\n"
                  "Data pointer bound for handle %d is 0.\n",
                   handle));
#endif
   }
   else
   {
      // Increment the cancellation count for that operation.
      ++data_tmp->cancellations;

      // Update the metrics interval endpoint(s).
      if (this->interval_initialized_)
      {
         ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
         ACE_High_Res_Timer::hrtime_to_tv (this->interval_end_,
                                           hrtime_now);
      }
      else
      {
         this->interval_initialized_ = 1;
         ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
         ACE_High_Res_Timer::hrtime_to_tv (this->interval_start_,
                                           hrtime_now);
         this->interval_end_.set (this->interval_start_.sec (),
                                  this->interval_start_.usec ());
      }
   }
}


// Inline: Records start time of a base metrics segment.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::report_base_metrics_start (RtecScheduler::handle_t handle)
{
   TAO_Metrics_Cache_Data_Base* base_data = 0;
   TAO_Metrics_Cache_Data_Base * base_data_tmp = 0;
   if (base_monitor_maps_ [supplier_index_].find (handle, base_data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_start:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((base_data_tmp = base_data.addr ()) == 0)
   else if ((base_data_tmp = base_data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_start:\n"
                  "Data pointer bound for handle %d is 0.\n",
                   handle));
#endif
   }
   else
   {
      if (! this->interval_initialized_)
      {
         this->interval_initialized_ = 1;
         ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
         ACE_High_Res_Timer::hrtime_to_tv (this->interval_start_,
                                           hrtime_now);
         this->interval_end_.set (this->interval_start_.sec (),
                                  this->interval_start_.usec ());
      }

      // Take timeprobe stamp
      base_data_tmp->probe.timeprobe (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::
                                      WORK_START);
   }
}


// Inline: Records stop time of a base metrics segment.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
report_base_metrics_stop (RtecScheduler::handle_t handle)
{
   TAO_Metrics_Cache_Data_Base* base_data = 0;
   TAO_Metrics_Cache_Data_Base * base_data_tmp = 0;
   if (base_monitor_maps_ [supplier_index_].find (handle, base_data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_stop:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
      return;
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((base_data_tmp = base_data.addr ()) == 0)
   else if ((base_data_tmp = base_data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                 "TAO_Metrics_LocalCache::report_upcall_stop:\n"
                 "Data pointer bound for handle %d is 0.\n",
                  handle));
#endif
      return;
   }
   else
   {
      // Take timeprobe stamp
      base_data_tmp->probe.timeprobe (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::
                                      WORK_STOP);

      this->interval_end_ =
         base_data_tmp->probe.last_recorded_timestamp ();
   }
}

// Inline: Records suspend time of a base metrics segment.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
report_base_metrics_suspend (RtecScheduler::handle_t handle)
{
   TAO_Metrics_Cache_Data_Base* base_data = 0;
   TAO_Metrics_Cache_Data_Base * base_data_tmp = 0;
   if (base_monitor_maps_ [supplier_index_].find (handle, base_data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_suspend:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
      return;
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((base_data_tmp = base_data.addr ()) == 0)
   else if ((base_data_tmp = base_data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                 "TAO_Metrics_LocalCache::report_upcall_suspend:\n"
                 "Data pointer bound for handle %d is 0.\n",
                  handle));
#endif
      return;
   }
   else
   {
      // Take timeprobe stamp
      base_data_tmp->probe.timeprobe (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::
                                      WORK_SUSPEND);

      this->interval_end_ =
         base_data_tmp->probe.last_recorded_timestamp ();
   }
}


// Inline: Records resume time of a base metrics segment.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
report_base_metrics_resume (RtecScheduler::handle_t handle)
{
   TAO_Metrics_Cache_Data_Base* base_data = 0;
   TAO_Metrics_Cache_Data_Base * base_data_tmp = 0;
   if (base_monitor_maps_ [supplier_index_].find (handle, base_data) < 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::report_upcall_resume:\n"
                  "Could not find data for handle %d.\n",
                   handle));
#endif
      return;
   }
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  else if ((base_data_tmp = base_data.addr ()) == 0)
   else if ((base_data_tmp = base_data) == 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                 "TAO_Metrics_LocalCache::report_upcall_resume:\n"
                 "Data pointer bound for handle %d is 0.\n",
                  handle));
#endif
      return;
   }
   else
   {
      // Take timeprobe stamp
      base_data_tmp->probe.timeprobe (ACE_Metrics_Timeprobe<ACE_LOCK, ALLOCATOR>::
                                      WORK_RESUME);

      this->interval_end_ =
         base_data_tmp->probe.last_recorded_timestamp ();
   }
}

template <class ACE_LOCK, class ALLOCATOR>
void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
increment_base_handle(TAO_Metrics_Utils::Base_Metrics_Type bmt)
{
   switch (bmt)
   {
   case TAO_Metrics_Utils::SCHED:
      ++this->sched_count_;
      break;
   case TAO_Metrics_Utils::QUO:
      ++this->quo_count_;
      break;
   case TAO_Metrics_Utils::TNS:
      ++this->tns_count_;
      break;
   case TAO_Metrics_Utils::TEEC:
      ++this->teec_count_;
      break;
   case TAO_Metrics_Utils::RTARM: // treat RTARM as the default case
   default:
      ++this->rtarm_count_;
      break;
   }
}

template <class ACE_LOCK, class ALLOCATOR>
void
TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::
increment_upcall_handle()
{
   ++this->upcall_adapter_count_;
}

///////////////////////////////////////////
// Class TAO_Metrics_ReportingLocalCache //
///////////////////////////////////////////


// Default constructor.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE
TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::
TAO_Metrics_ReportingLocalCache (u_long table_size,
                                 u_long number_of_probes,
                                 ALLOCATOR * alloc)
  : TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> (table_size, number_of_probes, alloc)
{
}


// Registers probes with the logger.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE int
TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::register_probes (Metrics::QoSLogger_ptr logger)
{
   ACE_TRY_NEW_ENV
   {
      if (CORBA::is_nil (logger))
      {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
         ACE_ERROR ((LM_ERROR,
                     "TAO_Metrics_LocalCache::register_probes:\n"
                     "passed logger pointer was nil.\n"));
#endif
         return -1;
      }

      // Sequence to register probe names and ids with the logger.
      Metrics::ProbeIdentity_Set probe_identities (this->probe_set_size_ * 2);
      probe_identities.length (this->probe_set_size_ * 2);

      for (u_int i = 0; i < this->probe_set_size_; ++i)
      {
         // Batch up the enqueue ids and enqueue names so we
         // can register them with the logger.
         probe_identities [i].probe_id =
            i + TAO_Metrics_Utils::METRICS_ENQUEUE_PROBE_ID_BASE;
         probe_identities [i].probe_name =
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //            (const char *) this->enqueue_names_.addr () [i].addr ();
            (const char *) this->enqueue_names_ [i];
         probe_identities [i + this->probe_set_size_].probe_id =
            i + TAO_Metrics_Utils::METRICS_DEQUEUE_PROBE_ID_BASE;
         probe_identities [i + this->probe_set_size_].probe_name =
            // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
            //            (const char *) this->dequeue_names_.addr () [i].addr ();
            (const char *) this->dequeue_names_ [i];
      }

      logger->set_identities (probe_identities, ACE_TRY_ENV);
      ACE_TRY_CHECK;
   }
   ACE_CATCHANY
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_Metrics_LocalCache::register_probes exception");
#endif
      return -1;
   }
   ACE_ENDTRY;

   return 0;
}


template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::output_statistics (Metrics::QoSLogger_ptr logger)
{
   if (metrics_enabled_)
   {
   if (CORBA::is_nil (logger))
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::output_statistics:\n"
                  "passed logger pointer was nil.\n"));
#endif
      return;
   }

   Metrics::TimeprobeParameter_Set data_set;
   Metrics::QoSParameter_Set qos_set;

   // For each probe on the metrics consumer side, install data set,
   // call probe's report_intervals method, and then uninstall data
   // set.

   for (u_int j = 0; j < this->probe_set_size_; ++j)
   {
      {
         // The cast is needed to convert the base class pointer
         // (which we got from the (outer) based pointer addr () call) to a
         // derived class pointer.
        TAO_Metrics_LocalTimeprobe<ACE_LOCK, ALLOCATOR> *probe_ptr =
            ACE_dynamic_cast_2_ptr (TAO_Metrics_LocalTimeprobe,
                                    ACE_LOCK,
                                    ALLOCATOR,
                                    *this->enqueue_probes_[j]);
// Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
//                                  [this->consumer_index_].addr () [j].addr ());

         if (probe_ptr)
         {
            probe_ptr->install_data_set (& data_set);
            probe_ptr->report_intervals ();
            probe_ptr->install_data_set (0);
            probe_ptr->install_data_set (& data_set);
            probe_ptr->report_intervals ();
            probe_ptr->install_data_set (0);
         }
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
         else
         {
            ACE_ERROR ((LM_ERROR,
                        "TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR>::output_statistics: "
                         "pointer static cast or down cast failed"));
         }
#endif
      }
   }

   // Iterate over registered base metrics data and harvest those probes.
   // UGLY: shouldn't have to do this...?
   typename TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::ACE_METRICS_CACHE_DATA_BASE_TYPE base_data = 0;

   //METRICS_BASE_MONITOR_MAP_ITERATOR
   /*
     ACE_Hash_Map_Iterator_Ex<RtecScheduler::handle_t,
     ACE_Based_Pointer<TAO_Metrics_Cache_Data_Base>,
     ACE_Hash<RtecScheduler::handle_t>,
     ACE_Equal_To<RtecScheduler::handle_t>,
     ACE_Null_Mutex>
   */
   typename TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::METRICS_BASE_MONITOR_MAP_ITERATOR
     base_iter (this->base_monitor_maps_ [this->consumer_index_]);
   for (;
        base_iter.done () == 0;
        ++base_iter)
   {
// Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
//      base_data = (*base_iter).int_id_.addr ();
      base_data = (*base_iter).int_id_;
      if (base_data)
      {
         base_data->probe.install_data_set (& data_set);
         base_data->probe.report_intervals ();
         base_data->probe.install_data_set (0);
      }
    }


    // Iterate over registered upcall data and harvest those probes, and
    // the aggregate QoS data too.
    typename TAO_Metrics_ReportingLocalCache<ACE_LOCK,ALLOCATOR>::TAO_Metrics_Cache_Data * data = 0;
      
    typename TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::METRICS_MONITOR_MAP_ITERATOR
      data_iter (this->monitor_maps_ [this->consumer_index_]);

    for (u_long qos_length = 1;
         data_iter.done () == 0;
         ++data_iter, ++qos_length)
      {
        // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
        //      data = (*data_iter).int_id_.addr ();
           data = (*data_iter).int_id_;
           if (data)
           {
              data->probe.install_data_set (& data_set);
              data->probe.report_intervals ();
              data->probe.install_data_set (0);

              qos_set.length (qos_length);
              qos_set [qos_length - 1].entry_point
// Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
//            = (const char *) data->probe_name.addr ();
              = (const char *) data->probe_name;
              qos_set [qos_length - 1].deadlines_made = data->made_deadlines;
              qos_set [qos_length - 1].deadlines_missed = data->missed_deadlines;
              qos_set [qos_length - 1].operations_cancelled = data->cancellations;
              qos_set [qos_length - 1].is_hrt = data->is_hrt;
           }
        }

        // Then, give harvested data sets to the remote logger.

        Metrics::Time interval;
        ACE_Time_Value interval_tv ((this->interval_initialized_)
                                     ? (this->interval_end_ - this->interval_start_)
                                     : ACE_Time_Value::zero);
        ORBSVCS_Time::Time_Value_to_TimeT (interval, interval_tv);

        ACE_TRY_NEW_ENV
        {
           logger->log_timeprobe_data (data_set, interval, ACE_TRY_ENV);
           ACE_TRY_CHECK;

           logger->log_aggregate_QoS (qos_set, interval, ACE_TRY_ENV);
           ACE_TRY_CHECK;
        }
        ACE_CATCHANY
        {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
           ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                "TAO_Metrics_LocalCache::output_statistics:"
                                " logger call failed\n");
#endif
        }
        ACE_ENDTRY;

        // Finally, reset the metrics data on the consumer side, for the
        // next round of logging.  TBD - consider whether or not we should
        // do this here, or make resetting an independent action.
        this->reset_statistics ();
        this->reset_base_statistics ();
   }
}


// Registers an upcall adapter, allocates monitoring data structures
// for it in hash maps if not already present.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::
register_upcall_adapter (RtecScheduler::handle_t handle,
                         const char *name,
                         int is_hrt,
                         Metrics::QoSLogger_ptr logger)
{
   // Call the base class method: the wierd syntax is needed because
   // MSVC++ was choking on the direct invocation using the this pointer.
   TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> *base_ptr = this;
   const unsigned short incr_upcall_id = 0;
   base_ptr->register_upcall_adapter (handle, name, is_hrt, incr_upcall_id);

   // Look for an existing data structure entry in the 0th hash map.
   typename TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::ACE_METRICS_CACHE_DATA_TYPE
     data = 0;
   if (monitor_maps_ [0].find (handle, data) != 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::register_upcall_adapter"
                  " bind failed.\n"));
#endif
      return;
   }

   // Register upcall monitor adapter's name and id with the logger:
   // allows registrations of the same adapter under different ids.
   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (! CORBA::is_nil (logger) && data.addr () != 0)
   if (! CORBA::is_nil (logger) && data != 0)
   {
      Metrics::ProbeIdentity_t probe_identity;
      probe_identity.probe_id = this->upcall_adapter_count_
                                + TAO_Metrics_Utils::METRICS_UPCALL_MONITOR_ADAPTER_ID_BASE;
      probe_identity.probe_name =
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //        (const char *) data.addr ()->probe_name.addr ();
         (const char *) data->probe_name;

      // Increment the probe id now that we're finished using it.
      this->increment_upcall_handle();

      ACE_TRY_NEW_ENV
      {
         logger->set_identity (probe_identity, ACE_TRY_ENV);
         ACE_TRY_CHECK;
      }
      ACE_CATCHANY
      {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
         ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                              "TAO_Metrics_LocalCache::"
                              "register_upcall_adapter:"
                              " logger call failed\n");
#endif
      }
      ACE_ENDTRY;
   }
}

// Registers a base metrics probe, allocates monitoring data structures
// for it in hash maps, returns a handle to it.
template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE RtecScheduler::handle_t
TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::
register_base_metrics (const char *name,
                       TAO_Metrics_Utils::Base_Metrics_Type bmt,
                       Metrics::QoSLogger_ptr logger)
{
   // Call the base class method and grab the handle it returns;
   TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> *base_ptr = this;
   const unsigned short incr_probe_id = 0;
   RtecScheduler::handle_t handle = base_ptr->register_base_metrics (name, bmt, incr_probe_id);

   // If there was an error, just return.
   if (handle == 0)
   {
      return handle;
   }

   // Look for an existing data structure entry in each hash map.
   typename TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR>::ACE_METRICS_CACHE_DATA_BASE_TYPE
     base_data = 0;
   if (base_monitor_maps_ [0].find (handle, base_data) != 0)
   {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::register_upcall_adapter"
                  " bind failed.\n"));
#endif
      return 0;
   }

   // Register base metrics segment name and id with the logger.

   // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
   //  if (! CORBA::is_nil (logger) && base_data.addr () != 0)
   if (! CORBA::is_nil (logger) && base_data != 0)
   {
      Metrics::ProbeIdentity_t probe_identity;

      switch (bmt)
      {
      case TAO_Metrics_Utils::SCHED:
         probe_identity.probe_id = this->sched_count_
            + TAO_Metrics_Utils::METRICS_SCHED_PROBE_ID_BASE;
         break;
      case TAO_Metrics_Utils::QUO:
         probe_identity.probe_id = this->quo_count_
            + TAO_Metrics_Utils::METRICS_QUO_PROBE_ID_BASE;
         break;
      case TAO_Metrics_Utils::TNS:
         probe_identity.probe_id = this->tns_count_
            + TAO_Metrics_Utils::METRICS_TNS_PROBE_ID_BASE;
         break;
      case TAO_Metrics_Utils::TEEC:
         probe_identity.probe_id = this->teec_count_
            + TAO_Metrics_Utils::METRICS_TEEC_PROBE_ID_BASE;
         break;
      case TAO_Metrics_Utils::RTARM: // treat RTARM as the default case
      default:
         probe_identity.probe_id = this->rtarm_count_
            + TAO_Metrics_Utils::METRICS_RTARM_PROBE_ID_BASE;
         break;
      }

      // Now increment the handle.
      increment_base_handle(bmt);

      probe_identity.probe_name =
         // Modified by BRM.  Not sure what the equivalent is for ACE_Based_Pointers
         //        (const char *) base_data.addr ()->probe_name.addr ();
         (const char *) base_data->probe_name;

      ACE_TRY_NEW_ENV
      {
         logger->set_identity (probe_identity, ACE_TRY_ENV);
         ACE_TRY_CHECK;
      }
      ACE_CATCHANY
      {
#if defined (METRICS_CACHE_ERROR_OUTPUT_ENABLED)
         ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                              "TAO_Metrics_LocalCache::"
                              "register_base_metrics:"
                              " logger call failed\n");
#endif
      }
      ACE_ENDTRY;
   }

   return handle;
}


#endif /* METRICS_LOCAL_CACHE_T_I */

//$Id$

#ifndef CACHING_STRATEGIES_T_C
#define CACHING_STRATEGIES_T_C

#include "Caching_Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE 
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "Caching_Strategies_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Web_Crawler, Caching_Strategies_T, "$Id$")

template<class CONTAINER> int
ACE_LRU_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container) 
{
  // Check that the purge_percent is non-zero.
  if (purge_percent_ == 0)
    return 0;

  // Also whether the number of entries in the cache is just one!
  // Oops! then thers no way out but exiting. So return an error.
  if (this->entries_ == 1)
    return -1;

  KEY key_to_remove;
  VALUE value_to_remove;
   
  ITEM *item = 0;
   
  ATTRIBUTES min = 0;

   // Return value.
   int result = 0;

   // Calculate the no of entries to remove form the cache depending upon the <purge_percent>.
   int no_of_entries = (this->purge_percent_/100) * this->entries_;
   // If the number of entries is less than 10 with the default percent 
   // being 10, the calculated no_pf_entries equals 0. So increment it
   // so that atleast one entry gets purged.
   if (no_of_entries == 0)
     {
       if (container.current_size () >= 1)
         no_of_entries = 1;
     }

   for (int i = 0; i < no_of_entries ; ++i)
     {
       ITERATOR iter (container);

       // The iterator moves thru the container searching for the entry with the
       // lowest ATTRIBUTES.
       
       for (min = (*iter).int_id_.second (), key_to_remove = (*iter).ext_id_, value_to_remove = (*iter).int_id_;
            iter.next (item) != 0 ;
            ++iter)
         {
           // Ah! an item with lower ATTTRIBUTES...
           if (min > (*iter).int_id_.second ())
             {
               min = (*iter).int_id_.second ();
               key_to_remove = (*iter).ext_id_;
               value_to_remove = (*iter).int_id_;
             }
         }
       ACE_DEBUG ((LM_DEBUG, "AUTO_PURGE\nLRU: before unbind: current_size %d\n", container.current_size ()));
       
       // Delete the dynamically allocated value object.
       if (value_to_remove.first () != 0)
         {
           (value_to_remove.first ())->recycler (0, 0);
           
           result = (value_to_remove.first ())->close();
           if (result == -1)
             return result;
         }
       // Remove the item from cache.       
       result = container.unbind (key_to_remove);
       --this->entries_;
       ACE_DEBUG ((LM_DEBUG, "LRU:after unbind: result %d current_size %d\n", result, container.current_size ()));
     }
   return result; 
}

////////////////////////////////////////////////////////////////////////////////////////////////
template<class CONTAINER> int
ACE_LFU_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{
   // Check that the purge_percent is non-zero.
   if (purge_percent_ == 0)
    return 0;

  // Also whether the number of entries in the cache is just one!
  // Oops! then thers no way out but exiting. So return an error.
  if (this->entries_ == 1)
    return -1;

   KEY key_to_remove;
   VALUE value_to_remove;
   
   ITEM *item = 0;
   
   ATTRIBUTES min = 0;
   
   // Return value.
   int result = 0;
   
   // Calculate the no of entries to remove form the cache depending upon the <purge_percent>.
   int no_of_entries = (this->purge_percent_/100) * this->entries_;
   // If the number of entries is less than 10 with the default percent 
   // being 10, the calculated no_pf_entries equals 0. So increment it
   // so that atleast one entry gets purged.
   if (no_of_entries == 0)
     {
       if (container.current_size () >= 1)
         no_of_entries = 1;
     }
   
   for (int i = 0; i < no_of_entries ; ++i)
     {
       ITERATOR iter (container);
       // The iterator moves thru the container searching for the entry with the
       // lowest ATTRIBUTES.
       
       for (min = (*iter).int_id_.second (), key_to_remove = (*iter).ext_id_, value_to_remove = (*iter).int_id_;
            iter.next (item) != 0 ;
            ++iter)
         {
           // Ah! an item with lower ATTTRIBUTES...
           if (min > (*iter).int_id_.second ())
             {
               min = (*iter).int_id_.second ();
               key_to_remove = (*iter).ext_id_;
               value_to_remove = (*iter).int_id_;
             }
         }
       ACE_DEBUG ((LM_DEBUG, "AUTO_PURGE\nLFU: before unbind: current_size %d\n", container.current_size ()));
       
       // Delete the dynamically allocated value object.
       if (value_to_remove.first () != 0)
         {
           (value_to_remove.first ())->recycler (0, 0);
           
           result = (value_to_remove.first ())->close();
           if (result == -1)
             return result;
         }
       // Remove the item from cache.       
       result = container.unbind (key_to_remove);
       --this->entries_;
       ACE_DEBUG ((LM_DEBUG, "LFU:after unbind: result %d current_size %d\n", result, container.current_size ()));
     }
   return result; 

}

////////////////////////////////////////////////////////////////////////////////////////////////
template<class CONTAINER> int
ACE_FIFO_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{
   // Check that the purge_percent is non-zero.
   if (this->purge_percent_ == 0)
    return 0;

  // Also whether the number of entries in the cache is just one!
  // Oops! then thers no way out but exiting. So return an error.
  if (this->entries_ == 1)
    return -1;

   KEY key_to_remove;
   VALUE value_to_remove;
   
   ITEM *item = 0;
   
   ATTRIBUTES min = 0;

   // Return value.
   int result = 0;

   // Calculate the no of entries to remove form the cache depending upon the <purge_percent>.
   int no_of_entries = (this->purge_percent_/100) * this->entries_;
   // If the number of entries is less than 10 with the default percent 
   // being 10, the calculated no_pf_entries equals 0. So increment it
   // so that atleast one entry gets purged.
   if (no_of_entries == 0)
     {
       if (container.current_size () >= 1)
         no_of_entries = 1;
     }

   for (int i = 0; i < no_of_entries ; ++i)
     {
       ITERATOR iter (container);
       // The iterator moves thru the container searching for the entry with the
       // lowest ATTRIBUTES.
       for (min = (*iter).int_id_.second (), key_to_remove = (*iter).ext_id_, value_to_remove = (*iter).int_id_;
            iter.next (item) != 0 ;
            ++iter)
         {
           // Ah! an item with lower ATTTRIBUTES...
           if (min > (*iter).int_id_.second ())
             {
               min = (*iter).int_id_.second ();
               key_to_remove = (*iter).ext_id_;
               value_to_remove = (*iter).int_id_;
             }
         }
       ACE_DEBUG ((LM_DEBUG, "AUTO_PURGE\nFIFO: before unbind: current_size %d\n", container.current_size ()));
       
       // Delete the dynamically allocated value object.
       if (value_to_remove.first () != 0)
         {
           (value_to_remove.first ())->recycler (0, 0);
           
           result = (value_to_remove.first ())->close();
           if (result == -1)
             return result;
         }
       // Remove the item from cache.       
       result = container.unbind (key_to_remove);
       --this->entries_;
       ACE_DEBUG ((LM_DEBUG, "FIFO:after unbind: result %d current_size %d\n", result, container.current_size ()));
     }
   return result; 
   
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class CONTAINER> int
ACE_Null_Caching_Strategy<CONTAINER>::clear_cache (CONTAINER &container)
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

ACE_ALLOC_HOOK_DEFINE(ACE_LRU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_LFU_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_FIFO_Caching_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_Null_Caching_Strategy)
#endif /* CACHING_STRATEGIES_T_C */


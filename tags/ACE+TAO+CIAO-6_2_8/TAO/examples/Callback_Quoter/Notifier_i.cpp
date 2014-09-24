
//=============================================================================
/**
 *  @file    Notifier_i.cpp
 *
 *  $Id$
 *
 * Implementation of the Notifier_i class. This class is the servant
 * object for the callback quoter server.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "Notifier_i.h"


Notifier_i::Notifier_i (void)
  : notifier_exited_(0)
{
  // No-op
}

Notifier_i::~Notifier_i (void)
{
  // No-op
}

// Register a distributed callback handler that is invoked when the
// given stock reaches the desired threshold value.

void
Notifier_i::register_callback (const char *stock_name,
                               CORBA::Long threshold_value,
                               Callback_Quoter::Consumer_ptr consumer_handler)
{
  // Store the client information.
  Consumer_Data consumer_data;

  // Necessary to make another copy of the consumer_handler using
  // <_duplicate> so that we dont lose the consumer object reference
  // after the method invocation is done.
  consumer_data.consumer_ =
    Callback_Quoter::Consumer::_duplicate (consumer_handler);

  consumer_data.desired_value_= threshold_value;

  CONSUMERS *consumers = 0;

  // The consumer_map consists of the stockname and various consumers
  // with their threshold values. To register a consumer into this
  // map, first the stockname is matched with an existing one (if any)
  // and the consumer and the threshold value is attached. Else, a new
  // entry is created for the stockname.

  if (this->consumer_map_.find (stock_name, consumers) == 0)
    {
     if ( consumers->insert (consumer_data) == -1)
       throw Callback_Quoter::Invalid_Stock (
         "Insertion failed! Invalid Stock!\n");
     else
      ACE_DEBUG ((LM_DEBUG,
                  "Inserted map entry: stockname %s threshold %d",
                  stock_name,
                  threshold_value));
    }
  else
    {
      // the unbounded set entry is created.
      ACE_NEW_THROW_EX (consumers, CONSUMERS, CORBA::NO_MEMORY ());

      // When a new entry is tried to be inserted into the unbounded set and it
      // fails an exception is raised.
      if (consumers->insert (consumer_data) == -1)
        throw Callback_Quoter::Invalid_Stock (
          "Insertion failed! Invalid Stock!\n");

      // The bond between the stockname <hash_key> and the consumers <hash_value>
      // is fused.
      if (this->consumer_map_.bind (stock_name, consumers) == -1)
         ACE_ERROR ((LM_ERROR,
                   "register_callback: Bind failed!/n"));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "new map entry: stockname %s threshold %d\n",
                    stock_name,
                    threshold_value));
    }
}

// Obtain a pointer to the orb.

void
Notifier_i::orb (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
}

// Remove the client handler.

void
Notifier_i::unregister_callback (Callback_Quoter::Consumer_ptr consumer)
{
  // The consumer_map consists of a map of stocknames with consumers
  // and their threshold values attached to it. To unregister a
  // consumer it is necessary to remove that entry from the
  // map. Hence, the map is iterated till the consumer entry to be
  // removed is found and then removed from the map.

  // Check to see whether the hash_map still exists. Chances are there
  // that the notifier has exited closing the hash map.
  if (notifier_exited_ == 1)
    return;

  for (CONSUMER_MAP::ITERATOR iter = this->consumer_map_.begin ();
       iter != this->consumer_map_.end ();
       ++iter)
    {
      // The *iter is nothing but the stockname + unbounded set of
      // consumers+threshold values, i.e a ACE_Hash_Map_Entry.

       Consumer_Data consumer_to_remove;

       consumer_to_remove.consumer_ =
         Callback_Quoter::Consumer::_duplicate (consumer);

       // int_id is a member of the ACE_Hash_Map_Entry.  The remove
       // method will do a find internally using operator == which
       // will check only the consumer pointers.  If match found it
       // will be removed from the set. If the consumer cannot be
       // removed an exception is raised.

       if ((*iter).int_id_->remove (consumer_to_remove) == -1)
         throw Callback_Quoter::Invalid_Handle (
           "Unregistration failed! Invalid Consumer Handle!\n");
       else
        ACE_DEBUG ((LM_DEBUG,
                    "unregister_callback:consumer removed\n"));
    }
}

// Gets the market status and sends the information to the consumer
// who is interested in it.

void
Notifier_i::market_status (const char *stock_name,
                           CORBA::Long stock_value)
{
  ACE_DEBUG ((LM_DEBUG,
              "Notifier_i:: The stockname is %s with price %d\n",
              stock_name,
              stock_value));

  CONSUMERS *consumers = 0;

  if (this->consumer_map_.find (stock_name, consumers) == 0)
    {
      // Go through the list of <Consumer_Data> to find which
      // registered client wants to be notified.

      for (CONSUMERS::ITERATOR iter = consumers->begin ();
           iter != consumers->end ();
           ++iter)
        {
          // Check whether the stockname is equal before proceeding
          // further.
          if (stock_value >= (*iter).desired_value_)
            {
              Callback_Quoter::Info interested_consumer_data;

              interested_consumer_data.stock_name =
                CORBA::string_dup (stock_name);
              interested_consumer_data.value =
                stock_value;

              ACE_DEBUG ((LM_DEBUG,
                          "pushing information to consumer\n"));

              // The status desired by the consumer is then passed to
              // it.
              (*iter).consumer_->push (interested_consumer_data);
            }
        }
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                " Stock Not Present!\n"));
  // Raising an exception caused problems as they were caught by the Market daemon
  // who exited prematurely.

}

void
Notifier_i::shutdown (void)
{
  if ( this->consumer_map_.close () > 0)
    ACE_ERROR ((LM_ERROR,
                "Consumer_map_close error!\n"));
  else
  // This marks the exit of the notifier. This should be taken care of
  // before the consumer tries to unregister after the notifier quits.
  notifier_exited_ = 1;

  ACE_DEBUG ((LM_DEBUG,
              "The Callback Quoter server is shutting down...\n"));


  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();

}

bool
Notifier_i::Consumer_Data::operator== (const Consumer_Data &rhs) const
{
  // The <_is_equivalent> function checks if the _var and _ptr objects
  // are the same.  NOTE: this call might not behave well on other
  // ORBs since <_is_equivalent> isn't guaranteed to differentiate
  // object references.

  return this->consumer_->_is_equivalent (rhs.consumer_.in ());
}


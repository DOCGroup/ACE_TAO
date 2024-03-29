/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Notifier_i.h
 *
 *  Defines the implementation header for the Supplier interface.
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFIER_I_H
#define NOTIFIER_I_H

#include "NotifierS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ConsumerC.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers.h"
#include "ace/SString.h"
#include "ace/Null_Mutex.h"

/**
 * @class Notifier_i
 *
 * @brief Notifier servant class.
 *
 * The implementation of the Notifier class, which is the servant
 * object for the callback quoter server.
 */
class Notifier_i : public POA_Notifier
{
public:
  /// Constructor.
  Notifier_i () = default;

  /// Destructor.
  ~Notifier_i () = default;

  /// Register a distributed callback handler that is invoked when the
  /// given stock reaches the desired threshold value.
  void register_callback (const char *stock_name,
                          CORBA::Long threshold_value,
                          Callback_Quoter::Consumer_ptr consumer_handler) override;

  /// Remove the consumer object.
  void unregister_callback (Callback_Quoter::Consumer_ptr consumer_handler) override;


  /// Get the market status.
  void market_status (const char *stock_name, CORBA::Long stock_value) override;

  /// Get the orb pointer.
  void orb (CORBA::ORB_ptr orb);

  /// Shutdown the Notifier.
  void shutdown () override;

public:
  /// The ORB manager.
  CORBA::ORB_var orb_;

  /**
   * @class Consumer_Data
   *
   * @brief Saves the Consumer_var and the threshold stock value.
   */
  class Consumer_Data
  {
  public:
    /// Comparison operator.
    bool operator== (const Consumer_Data &rhs) const;

    /// Stores the consumer object reference.
    Callback_Quoter::Consumer_var consumer_;

    /// Stores the stock threshold value.
    CORBA::Long desired_value_;
  };

  typedef ACE_Unbounded_Set<Consumer_Data> CONSUMERS;

  typedef ACE_Hash_Map_Manager<ACE_CString, CONSUMERS *, ACE_Null_Mutex> CONSUMER_MAP;

  /// This is the hash map with each hash_entry consisting of the stockname
  /// and an unbounded set of consumer object pointer and the desired stockvalue.
  CONSUMER_MAP consumer_map_;

  ///This marks the exit of the notifier. This should be taken care of
  /// before the consumer tries to unregister after the notifier quits.
  int notifier_exited_ {0};
};

#endif /* NOTIFIER_I_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file
 *
 *  $Id
 *
 *  @author
 */
//=============================================================================

#ifndef TAO_DYNAMIC_TP_POA_STRATEGYIMPL_H
#define TAO_DYNAMIC_TP_POA_STRATEGYIMPL_H

#include /**/ "ace/pre.h"
#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "tao/Dynamic_TP/Dynamic_TP_Config.h"
//#include "tao/CSD_ThreadPool/CSD_TP_Task.h"
//#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.h"
//#include "tao/CSD_ThreadPool/CSD_TP_Servant_State_Map.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "tao/CSD_Framework/CSD_Strategy_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/Intrusive_Ref_Count_Handle_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


    class TAO_Dynamic_TP_POA_StrategyImpl;
    typedef TAO_Intrusive_Ref_Count_Handle<TAO_Dynamic_TP_POA_StrategyImpl> TAO_Dynamic_TP_POA_StrategyImpl_Handle;

  //  class TP_Custom_Request_Operation;

    /**
     * @class TAO_Dynamic_TP_POA_Strategy
     *
     * @brief A custom Dynamc Thread-Pool servant dispatching strategy class.
     *
     * This class represents an extension implementation of a "Custom
     * Servant Dispatching Strategy" called Dynamic_TP_POA_Strategy.
     *
     * A custom servant dispatching strategy object can be applied to a
     * POA object in order to carry out the servant dispatching duties
     * for that POA.
     *
     */
    class TAO_Dynamic_TP_Export TAO_Dynamic_TP_POA_StrategyImpl: public TAO::CSD::TP_Strategy
    {
    public:

	  /// Constructor for dynamic thread control
	  TAO_Dynamic_TP_POA_StrategyImpl(TAO_DTP_Definition * dynamic_tp_config,
								  bool     serialize_servants);

      /// Virtual Destructor.
      virtual ~TAO_Dynamic_TP_POA_StrategyImpl();

      /// Set the number of threads in the pool (must be > 0).
      void set_num_threads(TAO::CSD::Thread_Counter num_threads);

      /// Turn on/off serialization of servants.
      void set_servant_serialization(bool serialize_servants);


    private:

      /**
      * Helper method that is responsible for looking up the servant
      * state object in the servant state map *if* the "serialize
      * servants" flag is set to true.  In the case where the
      * "serialize servants" flag is set to false, then a "nil"
      * servant state handle object is returned.
      *
      * @param servant - input - a pointer to the servant object.
      *
      * @returns a handle to a servant state object.
      *
      * @throw PortableServer::POA::ServantNotActive if the servant
      *        state cannot be determined.
      */
      TAO::CSD::TP_Servant_State::HandleType get_servant_state
                                      (PortableServer::Servant servant);



	  /// The number of static pool threads to create up front. Theoretically this could be higher than min_pool_threads.
	  int initial_pool_threads_;

	  /// The low water mark for dynamic threads to settle to.
      int min_pool_threads_;

	  /// The high water mark for dynamic threads to be limited to.
	  int max_pool_threads_;

	  /// If the max_pool_threads_ value has been met, then ORB requests coming in can be queued. This is the maximum number that will be allowed.
	  size_t max_request_queue_depth_;

	  /// This is the memory stack size allowable for each thread.
	  size_t thread_stack_size_;

	  /// This is the maximum amount of time in seconds that an idle thread can stay alive before being taken out of the pool.
	  time_t thread_idle_time_;

	  /// This is the active object used by the worker threads.
      /// The request queue is owned/managed by the task object.
      /// The strategy object puts requests into the task's request
      /// queue, and the worker threads service the queued requests
      /// by performing the actual servant request dispatching logic.
      TAO::CSD::TP_Task task_;

      /// The "serialize servants" flag.
      bool serialize_servants_;


      /// The map of servant state objects - only used when the
      /// "serialize servants" flag is set to true.
      TAO::CSD::TP_Servant_State_Map servant_state_map_;
    };


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Dynamic_TP/Dynamic_TP_POA_StrategyImpl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_DYNAMIC_TP_POA_STRATEGYIMPL_H */

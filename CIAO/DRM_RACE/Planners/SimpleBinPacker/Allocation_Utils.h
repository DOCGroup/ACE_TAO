/**
 * @file Allocation_Utils.h
 * @author William Otte <wotte@dre.vanderbilt.edu
 * @author Ed Mulholland <emulholl@atl.lmco.com>
 *
 * Various utilities for allocation algorithms
 */
#ifndef RACE_ALLOCATION_UTILS_H
#define RACE_ALLOCATION_UTILS_H

#include "ace/Task.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ciao/CIAO_common.h"

namespace Deployment
{
  struct Domain;
  class InstanceDeploymentDescriptions;
  class MonolithicDeploymentDescriptions;
}

namespace CIAO
{
  namespace RACE
  {
    namespace Allocation_Utils
    {
      /**
       * @class Format_Converter
       * @brief Converts between IDL DeploymentPlan/Domain to containers suitable for allocation algorithms.
       *
       */
      template <typename Bins,
                typename Items>
      class Format_Converter
      {
      public:
        /**
         * @brief Constructor.
         * @param domain Current Domain information, from TargetManager
         * @param idds InstanceDeploymentDescriptoins from the plan being allocated.
         */
        Format_Converter (const Deployment::Domain *domain,
                          Deployment::InstanceDeploymentDescriptions *idds,
                          Deployment::MonolithicDeploymentDescriptions *mdds);

        /// Accessor to get raw bins to pass to allocation algorithms
        const Bins& get_bins (void)
        {
          return bins_;
        }

        /// Accessor to get raw items to pass to allocation algorithms
        const Items& get_items (void)
        {
          return items_;
        }

        /// Modifies stored IDD sequence reference with the allocations contained
        /// in allocated_items.
        bool commit_allocation (const Items &allocated_items);

      private:
        void operator () (const typename Items::value_type &);

        Deployment::InstanceDeploymentDescriptions *idds_;
        const Deployment::Domain *domain_;
        Bins bins_;
        Items items_;

      };

      struct Allocation_Thread_Manager_Base
      {
        virtual void start (void) = 0;

        virtual void terminate (void) = 0;

        virtual void commit (void) = 0;
      };

      template <typename Bins,
                typename Items,
                int (*Allocate)(Bins &, Items &)>
      class Allocation_Thread_Adapter : public ACE_Task_Base
      {
      public:
        Allocation_Thread_Adapter (const Bins &bins,
                                   const Items &items,
                                   ACE_Thread_Mutex &mutex,
                                   ACE_Condition_Thread_Mutex &condition,
                                   int id,
                                   std::pair <int, int> &shared)
          : bins_ (bins),
            items_ (items),
            mutex_ (mutex),
            condition_ (condition),
            id_ (id),
            shared_ (shared)
        {
        }

        virtual int svc (void)
        {
          //          CIAO_DEBUG (3, "Allocation %i starting\n", id_);

          if (Allocate (bins_, items_) == 1)
            {
              //  CIAO_DEBUG (3, "Allocation %i successful\n", this->id_);

              this->mutex_.acquire ();
              this->shared_.first = this->id_;
              --this->shared_.second;

              this->condition_.signal ();
              return 0;
            }

          //          CIAO_DEBUG (3, "Allocation %i failed\n", id_);

          this->mutex_.acquire ();

          --this->shared_.second;

          //          CIAO_DEBUG (3, "Allocation %i complete, %i remain\n", this->shared_.second);

          if (this->shared_.second <= 0)
            this->condition_.signal ();
          else
            this->mutex_.release ();

          return 0;
        }

        const Bins& get_bins (void)
        {
          return bins_;
        }

        const Items& get_items (void)
        {
          return items_;
        }

      private:
        Bins bins_;
        Items items_;

        ACE_Thread_Mutex &mutex_;
        ACE_Condition_Thread_Mutex &condition_;

        const int id_;
        std::pair<int, int>  &shared_;

      };
    }
  }
}

#include "Allocation_Utils.tpp"

#endif /* RACE_ALLOCATION_UTILS_H */

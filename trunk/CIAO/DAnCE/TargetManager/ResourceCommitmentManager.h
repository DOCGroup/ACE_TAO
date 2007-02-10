// $Id$
// 
/**
 * @file ResourceCommitmentManager.h
 *
 * @brief This file declares the ResourceCommitmentManager class
 * 
 * This class acts as the servant of the interface 
 * ResourceCommitmentManager
 */
#ifndef DEPLOYMENT_RESOURCECOMMITMENTMANAGERI_H_
#define DEPLOYMENT_RESOURCECOMMITMENTMANAGERI_H_

#include "Deployment_ResourceCommitmentManagerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO {

  class  ResourceCommitmentManager_i
   : public virtual POA_Deployment::ResourceCommitmentManager
    {
      public:
        // Constructor
        ResourceCommitmentManager_i (void);

        // Destructor
        virtual ~ResourceCommitmentManager_i (void);

        /**
         * @function commitResources
         * @brief Commits the resources 
         *
         * @description This function makes a call to the DomainDataManager in order
         * to commit the resources mentioned in the ResourceAllocation 
         * sequence. If the resource cannot be allocated throws a 
         * ResourceCommitmentFailed exception
         */
        virtual
          void commitResources (
              const ::Deployment::ResourceAllocations& resources
              )
          ACE_THROW_SPEC ((::CORBA::SystemException,
                ::Deployment::ResourceCommitmentFailure));

        virtual
          void releaseResources (
              const ::Deployment::ResourceAllocations & resources
              )
          ACE_THROW_SPEC ((::CORBA::SystemException,
                ::Deployment::ResourceCommitmentFailure));

      private:
        /**
         * @function add_to_commited_resource
         * @brief This function adds the res to already commited resources. 
         *        This is to be called from within commitResources
         */
        int add_to_commited_resource (::Deployment::ResourceAllocations res);

        /// The commited resource
        ::Deployment::ResourceAllocations resources_;
    };

}

#endif /* DEPLOYMENT_RESOURCECOMMITMENTMANAGERI_H_  */

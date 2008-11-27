// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Strategy_Factory.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_STRATEGY_FACTORY_H
#define TAO_CSD_TP_STRATEGY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"
#include "tao/Versioned_Namespace.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    /**
     * @class TP_Strategy_Factory
     *
     * @brief An ACE_Service_Object capable of creating TP_Strategy objects.
     *
     * TBD - Explain in more detail.
     *
     */
    class TAO_CSD_TP_Export TP_Strategy_Factory : public ACE_Service_Object
    {
      public:

        /// Constructor.
        TP_Strategy_Factory();

        /// Virtual Destructor.
        virtual ~TP_Strategy_Factory();

        int init (int argc, ACE_TCHAR* argv[]);
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT(TAO_CSD_TP, TAO_CSD_TP_Strategy_Factory)
ACE_FACTORY_DECLARE(TAO_CSD_TP, TAO_CSD_TP_Strategy_Factory)

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_STRATEGY_FACTORY_H */

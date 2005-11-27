// -*- C++ -*-
// $Id$

//=============================================================================
/**
 *  @file    HTBP_Filter_Factory.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla
 */
//=============================================================================

#ifndef ACE_HTBP_FILTER_FACTORY
#define ACE_HTBP_FILTER_FACTORY
#include /**/ "ace/pre.h"

#include "HTBP_Export.h"
#include "HTBP_Filter.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTBP
  {
    class Stream;

    /**
     * @class Filter_Factory
     *
     * @brief Factory to get the Filters
     *
     * The Filter Factory is to be used for getting filters based on the
     * mode requested. This class is a ACE_Service_Object and should be
     * loadable with the help of the service configurator framework.
     */
    class HTBP_Export Filter_Factory : public ACE_Service_Object
    {
    public:

      /// Constructor
      Filter_Factory ();

      ~Filter_Factory ();

      /// Initialization hook.
      virtual int init (int argc, ACE_TCHAR *argv[]);

      //
      Filter *get_filter (int inside);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (HTBP, ACE_HTBP_Filter_Factory)
ACE_FACTORY_DECLARE (HTBP, ACE_HTBP_Filter_Factory)



#include /**/ "ace/post.h"
#endif /* ACE_HTBP_FILTER_FACTORY */

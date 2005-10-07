/**
 * $Id$
 * 
 * @file DynEnum_Handler.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef CIAO_CONFIG_HANDLERS_DYNENUM_HANDLER_H
#define CIAO_CONFIG_HANDLERS_DYNENUM_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#include "tao/ORB.h"
#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Config_Handlers
  {
    class Any;
    class DataType;
    class DataValue;
    
    /**
     * @class DynEnum_Handler
     * @brief Translates enums specified in deployment plans into Dynamic Anys
     *
     */
    class Config_Handlers_Export DynEnum_Handler
    {
    public:
      static DynamicAny::DynAny_ptr extract_into_dynany (const DataType &type,
                                                      const DataValue &value);
      
      static Any extract_out_of_dynany (const DynamicAny::DynAny_ptr dyn);
    };
    
  }
}


#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_DYNENUM_HANDLER_H*/


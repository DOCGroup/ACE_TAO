//==============================================================
/**
 *  @file  DynAny_Handler.h
 *
 *  $Id$
 *
 *  @author Will Otte <wotte@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_DYNANY_HANDLER_H
#define CIAO_CONFIG_HANDLERS_DYNANY_HANDLER_H
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
    * @class Any_Handler
    *
    * @brief Handler class for <ComponentInterfaceDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC Any objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    class Config_Handlers_Export DynAny_Handler
    {
    public:
      ~DynAny_Handler (void);
      
      static DynAny_Handler *instance ();
      
      DynamicAny::DynAny_ptr  extract_into_dynany (const DataType& type,
                                                const DataValue& value);
      
      //      Any get_any (const CORBA::Any &src);
      
      CORBA::ORB_ptr orb ();
            
      DynamicAny::DynAnyFactory_ptr daf ();
            
    private:
      DynAny_Handler (int, char**);
      
      static DynAny_Handler *instance_;
      
      CORBA::ORB_var orb_;
      
      DynamicAny::DynAnyFactory_var daf_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ANY_HANDLER_H*/

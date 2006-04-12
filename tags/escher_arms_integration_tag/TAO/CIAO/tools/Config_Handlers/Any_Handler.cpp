// $Id$

#include /**/ "ace/pre.h"

#include "Any_Handler.h"
#include "DataType_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "tao/AnyTypeCode/Any.h"
#include "ciao/CIAO_common.h"
#include "DynAny_Handler/DynAny_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    Any_Handler::Any_Handler (void)
    {
    }

    Any_Handler::~Any_Handler (void)
    {
    }

    void
    Any_Handler::extract_into_any (const Any& desc,
                                   CORBA::Any& toconfig)

    {
      CIAO_TRACE("Any_Handler::extract_into_any");
      DynamicAny::DynAny_var dyn = DYNANY_HANDLER->extract_into_dynany (desc.type (),
                                                 desc.value ());
      
      toconfig = *dyn->to_any ();
      
      dyn->destroy ();
    }

    Any Any_Handler::get_any (const ::CORBA::Any& src)
    {
      CIAO_TRACE("Any_Handler::extract_into_any - reverse");
      //MAJO
      //@Bala ... still haven't figured this one out yet
      //I was saving it for last...going from corba::any
      //to xsc DataValue
      DataType type = DataType_Handler::data_type (src.type ());
      DataValue value;
      Any any (type,value);
      return any;
    }
  }
}
#include /**/ "ace/post.h"

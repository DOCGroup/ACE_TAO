// -*- C++ -*-
// $Id$

#include "PSDL_Visitor_Factory.h"
#include "PSDL_Module_Visitor.h"
#include "PSDL_Type_Dcl_Visitor.h"
#include "PSDL_Interface_Visitor.h"
#include "PSDL_Struct_Visitor.h"
#include "PSDL_Enum_Type_Visitor.h"
#include "PSDL_Exception_Visitor.h"



TAO_PSDL_Visitor_Factory::TAO_PSDL_Visitor_Factory (void)
{
}

TAO_PSDL_Visitor_Factory::~TAO_PSDL_Visitor_Factory (void)
{
}

TAO_PSDL_Node_Visitor *
TAO_PSDL_Visitor_Factory::make_visitor (int type)
{
  switch (type)
    {
    case 258:
      {
        TAO_PSDL_Module_Visitor *module_visitor = 0;
        ACE_NEW_RETURN (module_visitor,
                        TAO_PSDL_Module_Visitor,
                        0);
        return module_visitor;
      }
    case 291:
      {
        TAO_PSDL_Struct_Visitor *struct_visitor = 0;
        ACE_NEW_RETURN (struct_visitor,
                        TAO_PSDL_Struct_Visitor,
                        0);
        return struct_visitor;
      }
    case 300:
      {
       TAO_PSDL_Type_Dcl_Visitor *type_dcl_visitor = 0;
       ACE_NEW_RETURN (type_dcl_visitor,
                       TAO_PSDL_Type_Dcl_Visitor,
                       0);
       return type_dcl_visitor;
      }
    case 324:
      {
        TAO_PSDL_Exception_Visitor *exception_visitor = 0;
        ACE_NEW_RETURN (exception_visitor,
                        TAO_PSDL_Exception_Visitor,
                        0);
        return exception_visitor;
      }
    case 325:
      {
       TAO_PSDL_Interface_Visitor *interface_visitor = 0;
       ACE_NEW_RETURN (interface_visitor,
                       TAO_PSDL_Interface_Visitor,
                       0);
       return interface_visitor;
      }
    case 337:
      {
        TAO_PSDL_Enum_Type_Visitor *enum_type_visitor = 0;
        ACE_NEW_RETURN (enum_type_visitor,
                        TAO_PSDL_Enum_Type_Visitor,
                        0);
        return enum_type_visitor;
      }
    default:
      return 0;
    }
}

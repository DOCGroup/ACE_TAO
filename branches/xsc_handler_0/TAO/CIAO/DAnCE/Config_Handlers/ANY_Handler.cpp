// $Id$

#include "ANY_Handler.h"

CIAO::Config_Handlers::ANY_Handler::ANY_Handler()
{}
CIAO::Config_Handlers::ANY_Handler::~ANY_Handler()
{}



using CIAO::Config_Handlers;

CORBA::Any& 
CIAO::Config_Handlers::ANY_Handler::get_Any(
                        CORBA::Any& toconfig, Any& desc)
{ 
 
 //Get the value that should be assigned to the Any.
 DataValue value = desc.value();
 
 //Here, we check each type to see if 
 //it is present. If a type is listed as 
 //present, we can assume that it is the 
 //intended value for the Any. This relieves
 //us from the burden of having to check the
 //type field on <desc>.
 if(value.short_p()){
    toconfig <<= ACE_static_cast (CORBA::Short, 
                                CORBA::Short(value.short_()));
 }
  if(value.ushort_p()){
    toconfig <<= ACE_static_cast (CORBA::UShort,
                                CORBA::UShort(value.ushort()));
 }
 if(value.long_p()){
    toconfig <<= ACE_static_cast (CORBA::Long,
                                CORBA::Long(value.long_()));
 }
 if(value.ulong_p()){
    toconfig <<= ACE_static_cast (CORBA::ULong,
                                CORBA::ULong(value.ulong()));
 }
 if(value.boolean_p()){
    toconfig <<= ACE_static_cast (CORBA::Boolean,
                                CORBA::Boolean(value.boolean()));
 }
 if(value.double_p()){
    toconfig <<= ACE_static_cast (CORBA::Double, 
                                CORBA::Double(value.double_()));
 }
 if(value.float_p()){
    toconfig <<= ACE_static_cast (CORBA::Float, 
                                CORBA::Float(value.float_()));
 }
 if(value.string_p()){
    toconfig <<= CORBA::string_dup (value.string().c_str());
 }
 if(value.octet_p()){
    toconfig <<= CORBA::Any::from_octet ( 
                                CORBA::Octet(value.octet()));
 }
 if(value.longlong_p()){
    toconfig <<= ACE_static_cast (CORBA::LongLong, 
                                CORBA::LongLong(value.longlong()));
 }
 if(value.ulonglong_p()){
    toconfig <<= ACE_static_cast (CORBA::ULongLong, 
                                CORBA::ULongLong(value.ulonglong()));
 }
 //if(value.longdouble_p()){
 //   toconfig <<= ACE_static_cast (CORBA::LongDouble, 
 //                               CORBA::LongDouble(value.longdouble()));
 // }
 
 return toconfig;
}

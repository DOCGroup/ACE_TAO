// $Id$

#include "cubit_impl.h"

ACE_RCSID(base_server, cubit_impl, "$Id$")

CORBA::Octet Cubit_Impl:: cube_octet (CORBA::Octet o) {
  return (CORBA::Octet) (o * o * o); 
}

CORBA::Short Cubit_Impl:: cube_short (CORBA::Short s) {
  return (CORBA::Short) (s * s * s);
}

CORBA::Long Cubit_Impl:: cube_long (CORBA::Long l) {
   return (CORBA::Long) (l * l * l);
}

Cubit::Many Cubit_Impl:: cube_struct (const Cubit::Many& values) {
  Cubit::Many out_values;
  out_values.o = values.o * values.o * values.o;
  out_values.s = values.s * values.s * values.s;
  out_values.l = values.l * values.l * values.l;
  return out_values; 
}

Cubit::oneof Cubit_Impl:: cube_union (const Cubit::oneof& values) {
  Cubit::oneof out_values;
   switch (values._d ()) {
   case Cubit::e_0th:
     out_values.o (values.o () * values.o ()  * values.o ());
     break;
   case Cubit::e_1st:
     out_values.s (values.s () * values.s () * values.s ());
     break;
   case Cubit::e_2nd:
     out_values.l (values.l () * values.l () * values.l ());
     break;
   case Cubit::e_3rd:
   default:
     out_values.cm ().o = values.cm ().o * values.cm ().o * values.cm ().o ;
     out_values.cm ().s = values.cm ().s * values.cm ().s * values.cm ().s ;
     out_values.cm ().l = values.cm ().l * values.cm ().l * values.cm ().l ;
     break;
   }
   return out_values;  
}

void Cubit_Impl:: please_exit () 
  {}

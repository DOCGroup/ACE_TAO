// $Id$

#include "cubit_impl.h"

ACE_RCSID(tps, cubit_impl, "$Id$")

CORBA::Octet Cubit_Impl:: cube_octet (CORBA::Octet o, CORBA::Environment &) throw (CORBA::SystemException) {
  return (CORBA::Octet) (o * o * o); 
}

CORBA::Short Cubit_Impl:: cube_short (CORBA::Short s, CORBA::Environment &) throw (CORBA::SystemException) {
  return (CORBA::Short) (s * s * s);
}

CORBA::Long Cubit_Impl:: cube_long (CORBA::Long l, CORBA::Environment &) throw (CORBA::SystemException) {
   return (CORBA::Long) (l * l * l);
}

Cubit::Many Cubit_Impl:: cube_struct (const Cubit::Many& values, CORBA::Environment &) throw (CORBA::SystemException) {
  Cubit::Many out_values;
  out_values.o = values.o * values.o * values.o;
  out_values.s = values.s * values.s * values.s;
  out_values.l = values.l * values.l * values.l;
  return out_values; 
}

Cubit::oneof Cubit_Impl:: cube_union (const Cubit::oneof& values, CORBA::Environment &) throw (CORBA::SystemException) {
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

void Cubit_Impl:: please_exit (CORBA::Environment &) throw (CORBA::SystemException) {
}

Cubit_ptr Cubit_Factory_Impl:: create_cubit (const char * name, CORBA::Environment &) throw (CORBA::SystemException) {
  cerr << "Creating Cubit for " << name << endl;

#ifdef Cubit_USE_BOA
  Cubit_Impl_Task *cubit = new Cubit_Impl_Task ();
#else
  Cubit_Impl_Task *cubit = new TIE_Cubit (Cubit_Impl) (new Cubit_Impl_Task);
#endif
  Cubit::_duplicate ((Cubit *)cubit);
  cubit->activate (1);
  return (Cubit *) cubit;
}

int
Cubit_Impl_Task::svc (void) 
{
  while (1) 
    {
      // Dequeue message
      ACE_Message_Block *mb;
      this->msg_queue ()->dequeue_head (mb);
      CORBA::Request *vp = (CORBA::Request *) mb->base ();
      mb->release ();
      // Hand it off to Orbix
      CORBA::Orbix.continueThreadDispatch (*vp);
    }
  return 0;
}

void *
Cubit_Impl_Task::_deref (void) 
{
  return this;
}

// $Id$

#include "cubit_impl.h"
#include "ace/ACE.h"

ACE_RCSID(MT_Cubit, cubit_impl, "$Id$")

Cubit_Impl::Cubit_Impl (Task_State *ts)
  :ts_ (ts),
   util_started_ (0)
{
}

CORBA::Octet Cubit_Impl:: cube_octet (CORBA::Octet o, CORBA::Environment &IT_env) 
{
  //  ACE_DEBUG ((LM_DEBUG, "octet cubed is %d\n", o*o*o));
  if (ts_->run_server_utilization_test_ == 1 && 
      ts_->utilization_task_started_ == 0 && 
      this->util_started_ == 0 )
    {
      this->util_started_ = 1;
      ts_->barrier_->wait ();
    }
  
  ts_->loop_count_++;

  return (CORBA::Octet) (o * o * o); 
}

CORBA::Short Cubit_Impl:: cube_short (CORBA::Short s, CORBA::Environment &IT_env)  {
  return (CORBA::Short) (s * s * s);
}

CORBA::Long Cubit_Impl:: cube_long (CORBA::Long l, CORBA::Environment &IT_env)  {
  return (CORBA::Long) (l * l * l);
}

Cubit::Many Cubit_Impl:: cube_struct (const Cubit::Many& values, CORBA::Environment &IT_env)  {
  Cubit::Many out_values;
  out_values.o = values.o * values.o * values.o;
  out_values.s = values.s * values.s * values.s;
  out_values.l = values.l * values.l * values.l;
  return out_values; 
}

Cubit::oneof Cubit_Impl:: cube_union (const Cubit::oneof& values, CORBA::Environment &IT_env)  {
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

void Cubit_Impl:: please_exit (CORBA::Environment &IT_env)  {
}

// $Id$
#include "cubit_impl.hh"

// This file is a skeleton of the implementation of the _impl class.


//============== Cubit implementation ==============
Cubit_impl::Cubit_impl() : _sk_Cubit(0) {
// Do not change the signature for this constructor

}

Cubit_impl::~Cubit_impl() {

}

void Cubit_impl::cube_oneway(
  ) {
}


void Cubit_impl::cube_void(
  ) {
}

CORBA::Octet Cubit_impl::cube_octet(
          CORBA::Octet o
  ) {
    return o * o * o;
}


CORBA::Short Cubit_impl::cube_short(
          CORBA::Short s
  ) {
  return s * s * s;
}


CORBA::Long Cubit_impl::cube_long(
          CORBA::Long l
  ) {
  return l * l * l;
}


Cubit::Many Cubit_impl::cube_struct(
          const Cubit::Many& values
  ) {
  Cubit::Many temp;

  temp.o = values.o * values.o * values.o;
  temp.s = values.s * values.s * values.s;
  temp.l = values.l * values.l * values.l;

  return temp;

}

// Cubit::oneof Cubit_impl::cube_union(
//           const Cubit::oneof& values
//   ) {
//   Cubit::oneof temp;

//   ACE_UNUSED_ARG (env);
//   switch (values._d ())
//     {
//     case Cubit::e_0th:
//       temp.o (values.o () * values.o () * values.o ());
//       break;
//     case Cubit::e_1st:
//       temp.s (values.s () * values.s () * values.s ());
//       break;
//     case Cubit::e_2nd:
//       temp.l (values.l () * values.l () * values.l ());
//       break;
//     case Cubit::e_3rd:
//     default:
//       temp._d (values._d ()); // set the discriminant
//       // use the read/write accessor
//       temp.cm ().o  = values.cm ().o * values.cm ().o * values.cm ().o;
//       temp.cm ().s  = values.cm ().s * values.cm ().s * values.cm ().s;
//       temp.cm ().l  = values.cm ().l * values.cm ().l * values.cm ().l;

//     }
//   return temp;
// }

void Cubit_impl::cube_long_sequence(
          const Cubit::long_seq& input,
         Cubit::long_seq*& output
  ) {

  if (output == 0)
    output = new Cubit::long_seq (input.length ());

  output->length (input.length ());
  CORBA::ULong i = 0;
  CORBA::Long x = input[0];
  output[i] = x * x * x;

}


void Cubit_impl::cube_octet_sequence(
          const Cubit::octet_seq& input,
         Cubit::octet_seq*& output
  ) {
  if (output == 0)
    {
      output = new Cubit::octet_seq (input.length ());
    }
  // We don't want to cube all the elements on the sequence because
  // that will take too long and will affect the performance. Further,
  // we want to show that octet sequences have constant marshalling
  // time, but making a copy of each element will hide that.
  CORBA::ULong i = 0;
  CORBA::Octet x = input[0];
  output[i] = x * x * x;

}


void Cubit_impl::shutdown(
  ) {

}



// //============== Cubit_Shutdown implementation ==============
// Cubit_Shutdown_impl::Cubit_Shutdown_impl() : _sk_Cubit_Shutdown(0) {
// // Do not change the signature for this constructor

// }

// Cubit_Shutdown_impl::~Cubit_Shutdown_impl() {

// }

// void Cubit_Shutdown_impl::shutdown(
//   ) {

// }



// //============== Cubit_Factory implementation ==============
// Cubit_Factory_impl::Cubit_Factory_impl() : _sk_Cubit_Factory(0) {
// // Do not change the signature for this constructor

// }

// Cubit_Factory_impl::~Cubit_Factory_impl() {

// }

// Cubit_ptr Cubit_Factory_impl::make_cubit(
//   ) {
//   return this->my_cubit_;
// }



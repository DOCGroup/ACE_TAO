// $Id$
#ifndef __cubit__impl_hh_def
#define __cubit__impl_hh_def

#include "cubit.hh"

// This file is a skeleton for the _impl class.


//============== Cubit_impl ==============
class Cubit_impl;
typedef Cubit_impl * Cubit_impl_ptr;

class Cubit_impl : public virtual _sk_Cubit {
public: 

Cubit_impl();
~Cubit_impl();

CORBA::Octet cube_octet(
          CORBA::Octet o
  );
CORBA::Short cube_short(
          CORBA::Short s
  );
CORBA::Long cube_long(
          CORBA::Long l
  );
Cubit::Many cube_struct(
          const Cubit::Many& values
  );
// Cubit::oneof cube_union(
//           const Cubit::oneof& values
//   );
void cube_long_sequence(
          const Cubit::long_seq& input,
         Cubit::long_seq*& output
  );
void cube_octet_sequence(
          const Cubit::octet_seq& input,
         Cubit::octet_seq*& output
  );
void shutdown(
  );
private:
}; // End of class Cubit_impl


// //============== Cubit_Shutdown_impl ==============
// class Cubit_Shutdown_impl;
// typedef Cubit_Shutdown_impl * Cubit_Shutdown_impl_ptr;

// class Cubit_Shutdown_impl : public virtual _sk_Cubit_Shutdown {
// public: 

// Cubit_Shutdown_impl();
// ~Cubit_Shutdown_impl();

// void shutdown(
//   );
// private:
// }; // End of class Cubit_Shutdown_impl


//============== Cubit_Factory_impl ==============
// class Cubit_Factory_impl;
// typedef Cubit_Factory_impl * Cubit_Factory_impl_ptr;

// class Cubit_Factory_impl : public virtual _sk_Cubit_Factory {
// public: 

// Cubit_Factory_impl();
// ~Cubit_Factory_impl();

// Cubit_ptr make_cubit(
//   );
// private:
//   Cubit_impl my_cubit_;

// }; // End of class Cubit_Factory_impl

#endif

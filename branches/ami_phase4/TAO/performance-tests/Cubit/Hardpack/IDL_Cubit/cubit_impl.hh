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

void cube_oneway(
  );
void cube_void(
  );
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

#endif

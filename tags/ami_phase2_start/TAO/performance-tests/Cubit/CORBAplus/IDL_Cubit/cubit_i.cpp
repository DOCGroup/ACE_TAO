// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Cubit/CORBAplus/IDL_Cubit
//
// = FILENAME
//    cubit_i.cpp
//
// = AUTHOR
//    Sergio Flores-Gaitan
//
// ============================================================================

#include "pbroker/corba/orb.h"
#include "pbroker/corba/environ.h"
#include <pbroker/corba/xpsboa.h>
#include <pbroker/corba/lifecycl/impldecl.h>
#include "cubit_i.h"

ACE_RCSID(IDL_Cubit, cubit_i, "$Id$")

// Declare the Implementation 
XPS_DECLARE_IMPLEMENTATION_ID(Cubit_Factory_iId,"Cubit_Factory_iId")
XPS_DECLARE_IMPLEMENTATION(Cubit_Factory_decl,Cubit_Factory_iId,Cubit_Factory)

// Declare the associated activator, 
XPS_DECLARE_DEFAULT_ACTIVATOR(Cubit_Factory_decl,Cubit_FactoryDefActivator,Cubit_Factory,Cubit_Factory_i)

XPS_CAST1_IMPL(Cubit_Factory_i,Cubit_Factory_base_impl)

// Constructor

Cubit_Factory_i::Cubit_Factory_i (const char *key, int numobjs)
  :numobjs_ (numobjs)
{
  // Create implementation object with user specified key.
  this->my_cubit_ = new Cubit_i_ptr [this->numobjs_];

  static char obj_str [MAXNAMELEN];

  for (u_int i = 0; i < this->numobjs_; i++)
    {
      ACE_OS::memset (obj_str, '\0', MAXNAMELEN);
      ACE_OS::sprintf (obj_str, "key%d", i);

      my_cubit_[i] = new Cubit_i (obj_str);

      if (my_cubit_[i] == 0)
        ACE_ERROR ((LM_ERROR,
                    " (%P|%t) Unable to create implementation object%d\n",
                    i));
    }
}

// Destructor

Cubit_Factory_i::~Cubit_Factory_i (void)
{
  ACE_DEBUG ((LM_DEBUG, "~~~ in destructor ~Cubit_Factory_i()\n"));
  delete [] this->my_cubit_;
}

Cubit_ptr
Cubit_Factory_i::make_cubit (const char *key)
{
  CORBA_Environment env;

  for (size_t i = 0; i < this->numobjs_; i++)
    {
      const char *obj_str = this->my_cubit_[i]->_get_name ();

      // Keys matched.
      if (!ACE_OS::strcmp (obj_str, key))
        return Cubit::_duplicate (this->my_cubit_ [i]);
    }

  return Cubit::_nil ();
}

// Constructor

Cubit_i::Cubit_i (const char *obj_name)
  : key_(obj_name)
{
}

const char *
Cubit_i::_get_name (void)
{
  return key_;
}

// Destructor

Cubit_i::~Cubit_i (void)
{
}

// Cube an octet

CORBA::Octet
Cubit_i::cube_octet (CORBA::Octet o)
{
  return o * o * o;
}

// Cube a short.

CORBA::Short
Cubit_i::cube_short (CORBA::Short s)
{
  return s * s * s;
}

// Cube a long

CORBA::Long
Cubit_i::cube_long (CORBA::Long l)
{
  return l * l * l;
}

// Cube a struct

Cubit::Many
Cubit_i::cube_struct (const Cubit::Many &values)
{
  Cubit::Many temp;

  temp.o = values.o * values.o * values.o;
  temp.s = values.s * values.s * values.s;
  temp.l = values.l * values.l * values.l;

  return temp;
}

// Cube a union

Cubit::oneof
Cubit_i::cube_union (const Cubit::oneof &values)
{
  Cubit::oneof temp;

  switch (values._d ())
    {
    case e_0th:
      temp.o (values.o () * values.o () * values.o ());
      break;
    case e_1st:
      temp.s (values.s () * values.s () * values.s ());
      break;
    case e_2nd:
      temp.l (values.l () * values.l () * values.l ());
      break;
    case e_3rd:
    default:
      temp._d (values._d ()); // set the discriminant
      // use the read/write accessor
      temp.cm ().o  = values.cm ().o * values.cm ().o * values.cm ().o;
      temp.cm ().s  = values.cm ().s * values.cm ().s * values.cm ().s;
      temp.cm ().l  = values.cm ().l * values.cm ().l * values.cm ().l;

    }
  return temp;
}

// Shutdown.

void Cubit_i::please_exit (void)
{
  ACE_DEBUG ((LM_DEBUG, "I've been asked to shut down...\n"));
}

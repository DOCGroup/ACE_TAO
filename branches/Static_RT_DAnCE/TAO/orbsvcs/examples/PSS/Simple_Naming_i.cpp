// $Id$

#include "Simple_Naming_i.h"
#include "orbsvcs/PSS/PSDL_Code_Gen.h"

Naming_Context_i::Naming_Context_i (CORBA::ORB_ptr orb)
  : code_gen_ (0),
    orb_ (orb)
{
  // constructor
  // An instance of TAO_PSDL_Code_Gen
  ACE_NEW (this->code_gen_,
           TAO_PSDL_Code_Gen (this->orb_.in ()));
}

Naming_Context_i::~Naming_Context_i ()
{
  delete this->code_gen_;
}

int
Naming_Context_i::bind (const char *n,
                        const char *obj)
{
  // Save the binding to database.
  int result =
    this->code_gen_->set_name_obj_ref (n, obj);

  return result;
}

char *
Naming_Context_i::find (const char *n)
{
  // Get the Stringified object reference corresponding to
  // 'n'
  CORBA::String_var obj_ref =
    this->code_gen_->get_obj_ref (n);

  return CORBA::string_dup (obj_ref.in ());
}

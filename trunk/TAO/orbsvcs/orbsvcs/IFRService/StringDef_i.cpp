// $Id$

#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/StringDef_i.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_StringDef_i::TAO_StringDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_StringDef_i::~TAO_StringDef_i (void)
{
}

CORBA::DefinitionKind
TAO_StringDef_i::def_kind (void)
{
  return CORBA::dk_String;
}

void
TAO_StringDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_StringDef_i::destroy_i (void)
{
  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            "name",
                                            name);

  this->repo_->config ()->remove_section (this->repo_->strings_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_StringDef_i::type (void)
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_StringDef_i::type_i (void)
{
  CORBA::ULong bound = this->bound_i ();

  return this->repo_->tc_factory ()->create_string_tc (
                                         bound
                                       );
}

CORBA::ULong
TAO_StringDef_i::bound (void)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->bound_i ();
}

CORBA::ULong
TAO_StringDef_i::bound_i (void)
{
  u_int retval = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             "bound",
                                             retval);

  return static_cast<CORBA::ULong> (retval);
}

void
TAO_StringDef_i::bound (CORBA::ULong bound)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->bound_i (bound);
}

void
TAO_StringDef_i::bound_i (CORBA::ULong bound)
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             "bound",
                                             bound);
}

TAO_END_VERSIONED_NAMESPACE_DECL

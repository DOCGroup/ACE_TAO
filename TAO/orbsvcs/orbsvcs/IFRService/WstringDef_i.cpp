#include "Repository_i.h"
#include "WstringDef_i.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_WstringDef_i::TAO_WstringDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_IDLType_i (repo)
{
}

TAO_WstringDef_i::~TAO_WstringDef_i ()
{
}

CORBA::DefinitionKind
TAO_WstringDef_i::def_kind ()
{
  return CORBA::dk_Wstring;
}

void
TAO_WstringDef_i::destroy ()
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->destroy_i ();
}

void
TAO_WstringDef_i::destroy_i ()
{
  ACE_TString name;
  this->repo_->config ()->get_string_value (this->section_key_,
                                            ACE_TEXT("name"),
                                            name);

  this->repo_->config ()->remove_section (this->repo_->wstrings_key (),
                                          name.c_str (),
                                          0);
}

CORBA::TypeCode_ptr
TAO_WstringDef_i::type ()
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key ();

  return this->type_i ();
}

CORBA::TypeCode_ptr
TAO_WstringDef_i::type_i ()
{
  CORBA::ULong bound = this->bound_i ();

  return this->repo_->tc_factory ()->create_wstring_tc (bound);
}
CORBA::ULong
TAO_WstringDef_i::bound ()
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->bound_i ();
}


CORBA::ULong
TAO_WstringDef_i::bound_i ()
{
  u_int retval = 0;
  this->repo_->config ()->get_integer_value (this->section_key_,
                                             ACE_TEXT("bound"),
                                             retval);

  return static_cast<CORBA::ULong> (retval);
}

void
TAO_WstringDef_i::bound (CORBA::ULong bound)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  this->bound_i (bound);
}

void
TAO_WstringDef_i::bound_i (CORBA::ULong bound)
{
  this->repo_->config ()->set_integer_value (this->section_key_,
                                             ACE_TEXT("bound"),
                                             bound);
}

TAO_END_VERSIONED_NAMESPACE_DECL

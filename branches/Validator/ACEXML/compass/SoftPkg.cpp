// $Id$

#include "ACEXML/compass/SoftPkg.h"

ACEXML_SoftPkg::ACEXML_SoftPkg()
  : current_code_ (0), current_dep_ (0), current_impl_ (0)
{}

ACEXML_SoftPkg::~ACEXML_SoftPkg()
{
  Dependency** dep = 0;
  DEP_ITERATOR dep_iter (this->deps_);
  for (; dep_iter.next (dep); dep_iter.advance())
    delete *dep;
  Implementation** impl = 0;
  IMPL_ITERATOR impl_iter (this->impls_);
  for (; impl_iter.next (impl); impl_iter.advance())
    delete *impl;
}

void
ACEXML_SoftPkg::dump (void)const
{
  ACE_DEBUG ((LM_DEBUG, "Name = %s\n", this->name_.c_str()));
  ACE_DEBUG ((LM_DEBUG, "Version = %s\n", this->version_.c_str()));
  ACE_DEBUG ((LM_DEBUG, "Package Type = %s\n", this->pkgtype_.c_str()));
  ACE_DEBUG ((LM_DEBUG, "Title = %s\n", this->title_.c_str()));
  ACE_DEBUG ((LM_DEBUG, "Description = %s\n", this->description_.c_str()));
  this->propfile_.dump();
  Dependency** dep = 0;
  Dependencies::const_iterator dep_iter (this->deps_);
  for (; dep_iter.next (dep); dep_iter.advance())
    (*dep)->dump();
  Implementation** impl = 0;
  Implementations::const_iterator impl_iter (this->impls_);
  for (; impl_iter.next (impl); impl_iter.advance())
    (*impl)->dump();
}

void
ACEXML_SoftPkg::PropertyFile::dump(void)const
{
  ACE_DEBUG ((LM_DEBUG, "Property File = %s\n", this->file_.c_str()));
}

void
ACEXML_SoftPkg::OperSys::dump(void)const
{
  ACE_DEBUG ((LM_DEBUG, "OS = %s Version = %s\n",
              this->name_.c_str(), this->version_.c_str()));
}
void
ACEXML_SoftPkg::Code::dump(void)const
{
  ACE_DEBUG ((LM_DEBUG, " Code: \n  Type = %s\n  File = %s\n  "
              "EntryPoint = %s\n", this->type_.c_str(), this->file_.c_str(),
              this->func_.c_str()));
}
void
ACEXML_SoftPkg::Dependency::dump(void)const
{
  ACE_DEBUG ((LM_DEBUG, "Dependency: \n Type = %s\n File = %s\n",
              this->type_.c_str(), this->localfile_.c_str()));
}
void
ACEXML_SoftPkg::Implementation::dump(void)const
{
  ACE_DEBUG ((LM_DEBUG, "Implementation: \n Id = %s\n ", this->id_.c_str()));
  this->os_.dump();
  ACE_DEBUG ((LM_DEBUG, " Processor = %s\n Compiler = %s\n ProgLang = %s\n ",
              this->cpu_.c_str(), this->compiler_.c_str(),
              this->lang_.c_str()));
  this->propfile_.dump();
  this->code_.dump();
}

int
ACEXML_SoftPkg::start_element (const ACEXML_Char* element,
                               const ACEXML_String& id)
{
  if (ACE_OS::strcmp (element, "implementation") == 0)
    {
      Implementation* impl = 0;
      ACE_NEW_RETURN (impl, Implementation, -1);
      impl->id_ = id;
      if (this->impls_.insert (impl) < 0)
        {
          ACE_ERROR ((LM_ERROR, "Implementation already exists\n"));
          return -1;
        }
      this->current_impl_ = impl;
    }
  else if (ACE_OS::strcmp (element, "dependency") == 0)
    {
      Dependency* dep = 0;
      ACE_NEW_RETURN (dep, Dependency, -1);
      dep->type_ = id;
      if (this->deps_.insert (dep) < 0)
        {
          ACE_ERROR ((LM_ERROR, "Dependency already exists\n"));
          return -1;
        }
      this->current_dep_ = dep;
    }
  else if (ACE_OS::strcmp (element, "code") == 0)
    {
      if (!this->current_impl_)
        {
          ACE_ERROR ((LM_ERROR, "Code section outside of implementation\n"));
          return -1;
        }
      this->current_code_ = &this->current_impl_->code_;
      this->current_code_->type_ = id;
    }
  else
    return -1;
  return 0;
}

void
ACEXML_SoftPkg::end_element (const ACEXML_Char* element)
{
  if (ACE_OS::strcmp (element, "implementation") == 0)
    this->current_impl_ = 0;
  else if (ACE_OS::strcmp (element, "dependency") == 0)
    this->current_dep_ = 0;
  else if (ACE_OS::strcmp (element, "code") == 0)
    this->current_code_ = 0;
  return;
}

const ACEXML_SoftPkg::Implementations&
ACEXML_SoftPkg::get_impls (void) const
{
  return this->impls_;
}

const ACEXML_SoftPkg::Dependencies&
ACEXML_SoftPkg::get_deps (void) const
{
  return this->deps_;
}

int
ACEXML_SoftPkg::set (const ACEXML_Char* element,
                     const ACEXML_String& attname,
                     const ACEXML_String& value)
{
  if (ACE_OS::strcmp (element, "softpkg") == 0)
    {
      if (attname == "name")
        this->name_ = value;
      else if (attname == "version")
        this->version_ = value;
    }
  else if (ACE_OS::strcmp (element, "pkgtype") == 0)
    this->pkgtype_ = value;
  else if (ACE_OS::strcmp (element, "title") == 0)
    this->title_ = value;
  else if (ACE_OS::strcmp (element, "description") == 0)
    this->description_ = value;
  else if (ACE_OS::strcmp (element, "localfile") == 0)
    this->current_dep_->localfile_ = value;
  else if (ACE_OS::strcmp (element, "os") == 0)
    {
      if (attname == "name")
        this->current_impl_->os_.name_ = value;
      else
        this->current_impl_->os_.version_ = value;
    }
  else if (ACE_OS::strcmp (element, "processor") == 0)
    this->current_impl_->cpu_ = value;
  else if (ACE_OS::strcmp (element, "compiler") == 0)
    this->current_impl_->compiler_ = value;
  else if (ACE_OS::strcmp (element, "programminglanguage") == 0)
    this->current_impl_->lang_ = value;
  else if (ACE_OS::strcmp (element, "fileinarchive") == 0)
    {
      if (this->current_code_)
        this->current_code_->file_ = value;
      else if (this->current_impl_)
        this->current_impl_->propfile_.file_ = value;
      else
        this->propfile_.file_ = value;
    }
  else if (ACE_OS::strcmp (element, "entrypoint") == 0)
    this->current_code_->func_ = value;
  else
    return -1;
  return 0;
}

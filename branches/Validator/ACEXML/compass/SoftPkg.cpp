// $Id$

#include "ACEXML/compass/SoftPkg.h"

ACEXML_SoftPkg::ACEXML_SoftPkg()
  : current_dep_ (0), current_impl_ (0)
{}

ACEXML_SoftPkg::~ACEXML_SoftPkg()
{
  Dependency* dep = 0;
  Dependencies::iterator dep_iter (this->deps_);
  for (; dep_iter.next (dep); dep_iter.advance())
    delete dep;
  Implementation* impl = 0;
  Implementations::iterator impl_iter (this->impls_);
  for (; impl_iter.next (impl); impl_iter.advance())
    delete impl;
}

int
ACEXML_SoftPkg::add_impl (const ACEXML_String& id)
{
  Implementation* impl = 0;
  ACE_NEW_RETURN (impl, Implementation, -1);
  impl.id_ = id;
  if (this->impls_.insert (impl) < 0)
    {
      ACE_ERROR ((LM_ERROR, "Implementation already exists\n"));
      return -1;
    }




  int add_dep (const ACEXML_String& type);
  Implementation* get_current_impl (void);
  const ACE_Unbouded_Set<Implementation*>& get_all_impl (void) const;
private:
  ACEXML_String name_;
  ACEXML_String version_;
  ACEXML_String pkgtype_;
  ACEXML_String title_;
  ACEXML_String author_;
  ACEXML_String company_;
  ACEXML_String webpage_;
  ACEXML_String description_;
  Dependency* current_dep_;
  Implementation* current_impl_;
  ACE_Unbouded_Set<Dependency*> deps_;
  ACE_Unbouded_Set<Implementation*> impls_;
};

#include "ace/post.h"

#endif /* ACEXML_SOFTPKG_H */

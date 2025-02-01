#include "ast_annotation_appls.h"
#include "ast_annotation_appl.h"

AST_Annotation_Appls::AST_Annotation_Appls ()
{
}

AST_Annotation_Appls::AST_Annotation_Appls (const AST_Annotation_Appls& other)
{
  *this = other;
}

AST_Annotation_Appls::~AST_Annotation_Appls ()
{
}

AST_Annotation_Appls &
AST_Annotation_Appls::operator= (const AST_Annotation_Appls& other)
{
  vector_.clear ();
  add (other);
  return *this;
}

void
AST_Annotation_Appls::add (AST_Annotation_Appl *appl)
{
  vector_.push_back (AST_Annotation_Appl_Ptr (appl));
}

void
AST_Annotation_Appls::add (const AST_Annotation_Appls &other)
{
  for (const_iterator i = other.begin (); i != other.end (); ++i)
    {
      vector_.push_back (*i);
    }
}

bool
AST_Annotation_Appls::empty () const
{
  return !vector_.size ();
}

size_t
AST_Annotation_Appls::size () const
{
  return vector_.size ();
}

AST_Annotation_Appls::iterator
AST_Annotation_Appls::begin ()
{
  return vector_.begin ();
}

AST_Annotation_Appls::iterator
AST_Annotation_Appls::end ()
{
  return vector_.end ();
}

AST_Annotation_Appls::const_iterator
AST_Annotation_Appls::begin () const
{
  return vector_.begin ();
}

AST_Annotation_Appls::const_iterator
AST_Annotation_Appls::end () const
{
  return vector_.end ();
}

AST_Annotation_Appl *
AST_Annotation_Appls::operator[] (size_t index)
{
  return vector_[index].get ();
}

AST_Annotation_Appl *
AST_Annotation_Appls::find (AST_Annotation_Decl *annotation)
{
  AST_Annotation_Appl *result = nullptr;
  for (iterator i = begin (); i != end (); ++i)
    {
      AST_Annotation_Appl *appl = i->get ();
      if (appl && appl->annotation_decl () == annotation)
        {
          result = appl;
        }
    }
  return result;
}

AST_Annotation_Appl *
AST_Annotation_Appls::find (const char *annotation)
{
  if (!annotation)
    {
      return nullptr;
    }

  UTL_Scope* bottom = idl_global->scopes ().bottom ();
  if (!bottom)
    {
      return nullptr;
    }

  AST_Decl* decl = bottom->lookup_by_name (annotation);
  if (!decl)
    {
      return nullptr;
    }

  return find (dynamic_cast<AST_Annotation_Decl*> (decl));
}

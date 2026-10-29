#include "ast_annotation_appls.h"
#include "ast_annotation_appl.h"
#include "global_extern.h"
#include "utl_scope.h"

#include <string>

AST_Annotation_Appls::AST_Annotation_Appls () = default;

AST_Annotation_Appls::AST_Annotation_Appls (AST_Annotation_Appls const &other)
{
  *this = other;
}

AST_Annotation_Appls::~AST_Annotation_Appls () = default;

AST_Annotation_Appls &
AST_Annotation_Appls::operator= (AST_Annotation_Appls const &other)
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
AST_Annotation_Appls::add (AST_Annotation_Appls const &other)
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
AST_Annotation_Appls::find (std::string_view annotation)
{
  if (annotation.empty ())
    {
      return nullptr;
    }

  UTL_Scope * bottom = idl_global->scopes ().bottom ();
  if (bottom == nullptr)
    {
      return nullptr;
    }

  AST_Decl * decl = bottom->lookup_by_name (annotation);
  if (decl == nullptr)
    {
      return nullptr;
    }

  return find (dynamic_cast<AST_Annotation_Decl*> (decl));
}

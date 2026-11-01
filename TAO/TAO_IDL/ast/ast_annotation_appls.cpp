#include "ast_annotation_appls.h"
#include "ast_annotation_appl.h"
#include "global_extern.h"
#include "utl_scope.h"

#include <string>

AST_Annotation_Appls::AST_Annotation_Appls () = default;

AST_Annotation_Appls::AST_Annotation_Appls (AST_Annotation_Appls const &other)
  : vector_ (other.vector_)
{
}

AST_Annotation_Appls::~AST_Annotation_Appls () = default;

AST_Annotation_Appls &
AST_Annotation_Appls::operator= (AST_Annotation_Appls const &other)
{
  if (this != &other)
    {
      vector_ = other.vector_;
    }

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
  if (this == &other)
    {
      AST_Annotation_Appl_Ptrs const original (vector_);
      vector_.insert (vector_.end (), original.begin (), original.end ());
    }
  else
    {
      vector_.insert (vector_.end (), other.vector_.begin (), other.vector_.end ());
    }
}

bool
AST_Annotation_Appls::empty () const
{
  return vector_.empty ();
}

std::size_t
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
AST_Annotation_Appls::operator[] (std::size_t index) const
{
  return vector_[index].get ();
}

AST_Annotation_Appl *
AST_Annotation_Appls::find (AST_Annotation_Decl *annotation) const
{
  AST_Annotation_Appl *result = nullptr;
  for (AST_Annotation_Appl_Ptr const& appl_ptr : vector_)
    {
      AST_Annotation_Appl *appl = appl_ptr.get ();
      if (appl && appl->annotation_decl () == annotation)
        {
          result = appl;
        }
    }
  return result;
}

AST_Annotation_Appl *
AST_Annotation_Appls::find (std::string const& annotation) const
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

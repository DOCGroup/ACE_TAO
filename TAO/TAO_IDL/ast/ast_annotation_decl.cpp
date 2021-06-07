#include "ast_annotation_decl.h"
#include "ast_annotation_member.h"
#include "utl_indenter.h"
#include "utl_identifier.h"
#include "ast_constant.h"

AST_Decl::NodeType const AST_Annotation_Decl::NT =
  AST_Decl::NT_annotation_decl;

AST_Annotation_Decl::AST_Annotation_Decl (UTL_ScopedName *name)
  : AST_Decl (NT, name),
    AST_Type (NT, name),
    AST_ConcreteType (NT, name),
    UTL_Scope (NT),
    AST_Structure (name, false, false)
{
  if (!ACE_OS::strcmp(name->last_component ()->get_string (), "@annotation"))
    {
      idl_global->err ()-> misc_error(
        "An Annotation can not be named \"annotation\"");
    }
}

AST_Annotation_Decl::~AST_Annotation_Decl ()
{
}

void AST_Annotation_Decl::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "@annotation ");
  AST_Decl::dump (o);
  this->dump_i (o, " {\n");
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  this->dump_i (o, "}");
}

void AST_Annotation_Decl::destroy ()
{
  AST_Decl::destroy ();
}

void
AST_Annotation_Decl::escape_name (Identifier *name)
{
  FE_Utils::original_local_name (name); // Annotations can't clash with C++ keywords
  char *old_name = name->get_string ();
  char *new_name = new char [ACE_OS::strlen (old_name) + 2]; // '@' and '\0'
  if (new_name)
    {
      new_name[0] = '@';
      new_name[1] = '\0';
      ACE_OS::strcat (new_name, old_name);
      name->replace_string (new_name);
      delete [] new_name;
    }
}

void
AST_Annotation_Decl::escape_name (UTL_ScopedName *name)
{
  escape_name (name->last_component ());
}

bool
AST_Annotation_Decl::annotatable () const
{
  return false;
}

AST_Annotation_Member *
AST_Annotation_Decl::fe_add_annotation_member (
  AST_Annotation_Member *annotation_member)
{
  AST_Decl *d = fe_add_decl (annotation_member);
  if (d)
    {
      AST_Type *ft = annotation_member->field_type ();
      UTL_ScopedName *mru = ft->last_referenced_as ();
      if (mru)
        {
          add_to_referenced (ft, false, mru->first_component ());
        }
    }

  AST_Annotation_Decl *s = dynamic_cast<AST_Annotation_Decl*> (this);
  if (s)
    {
      s->fields ().enqueue_tail (annotation_member);
    }

  return dynamic_cast<AST_Annotation_Member*> (d);
}

AST_Constant *
AST_Annotation_Decl::fe_add_constant (AST_Constant *t)
{
  return dynamic_cast<AST_Constant*> (fe_add_decl (t));
}

int
AST_Annotation_Decl::ast_accept (ast_visitor *visitor)
{
  ACE_UNUSED_ARG (visitor);
  return 0;
}

bool
AST_Annotation_Decl::ami_visit ()
{
  return false;
}

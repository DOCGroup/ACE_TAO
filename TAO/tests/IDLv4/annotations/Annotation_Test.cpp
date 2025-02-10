#include "Annotation_Test.h"

#include <global_extern.h>

unsigned Annotation_Test::failed_test_count_ = 0;
unsigned Annotation_Test::total_test_count_ = 0;

Annotation_Test::Annotation_Test (const std::string &name)
  : name_ (name),
    failed_ (false),
    error_count_ (0),
    last_error_ (UTL_Error::EIDL_OK),
    last_warning_ (UTL_Error::EIDL_OK),
    scope_ (0),
    disable_output_ (false)
{
  total_test_count_++;
}

Annotation_Test::~Annotation_Test ()
{
  if (idl_global->err ()->last_error == UTL_Error::EIDL_SYNTAX_ERROR)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("Annotation Test: %C: ")
        ACE_TEXT ("FAILED because of syntax error in:\n"),
        name_.c_str ()));

      print_idl_with_line_numbers ();

      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("Check syntax error message above for more information.\n"),
        ACE_TEXT ("Failures beyond this might be false positives.\n")));
      ++failed_test_count_;
    }
  else if (!failed_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("Annotation Test: %C: PASSED\n"), name_.c_str ()));
    }

  idl_global->err ()->reset_last_error_and_warning ();
}

void
Annotation_Test::failed (const std::string &message)
{
  if (message.length ())
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C: %C\n"),
        name_.c_str (), message.c_str ()));
    }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("Annotation Test: %C: FAILED\nFailed IDL:\n"),
    name_.c_str ()));
  print_idl_with_line_numbers ();
  failed_test_count_++;
  failed_ = true;
  throw Failed ();
}

Annotation_Test &
Annotation_Test::error_count (int error_count)
{
  error_count_ = error_count;
  return *this;
}

Annotation_Test &
Annotation_Test::last_error (UTL_Error::ErrorCode last_error)
{
  last_error_ = last_error;
  return *this;
}

Annotation_Test &
Annotation_Test::last_warning (UTL_Error::ErrorCode last_warning)
{
  last_warning_ = last_warning;
  return *this;
}

Annotation_Test &
Annotation_Test::run (const std::string &idl)
{
  // Reset Error State
  idl_global->set_err_count (0);
  idl_global->err ()->last_error = UTL_Error::EIDL_OK;
  idl_global->err ()->last_warning = UTL_Error::EIDL_OK;

  // Eval IDL
  idl_ = idl;
  idl_global->eval (idl.c_str (), disable_output_);

  // Look at Results
  if (idl_global->err_count () != error_count_)
    {
      failed_ = true;
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\nError Count: expecting %d, got %d!\n"),
        name_.c_str (), error_count_, idl_global->err_count ()));
    }
  if (idl_global->err ()->last_error != last_error_)
    {
      failed_ = true;
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("Last Error Code (UTL_Error::ErrorCode): expecting "),
        name_.c_str ()));
      if (last_error_ == UTL_Error::EIDL_OK)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("OK")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%d"), last_error_));
        }
      ACE_ERROR ((LM_ERROR, ACE_TEXT (", got ")));
      if (idl_global->err ()->last_error == UTL_Error::EIDL_OK)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("OK")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%d"),
            idl_global->err ()->last_error));
        }
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("!\n")));
    }
  if (idl_global->err ()->last_warning != last_warning_)
    {
      failed_ = true;
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("Last Warning Code (UTL_Error::ErrorCode): expecting "),
        name_.c_str ()));
      if (last_warning_ == UTL_Error::EIDL_OK)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("OK")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%d"), last_warning_));
        }
      ACE_ERROR ((LM_ERROR, ACE_TEXT (", got ")));
      if (idl_global->err ()->last_warning == UTL_Error::EIDL_OK)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("OK")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%d"),
            idl_global->err ()->last_warning));
        }
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("!\n")));
    }
  if (failed_)
    {
      failed ();
    }

  return *this;
}

AST_Decl *
Annotation_Test::assert_node (const char *name, UTL_Scope *from)
{
  AST_Decl *node = 0;
  UTL_ScopedName *scoped_name = FE_Utils::string_to_scoped_name (name);

  UTL_Scope *scope = from ? from : (scope_ ? scope_ : idl_global->scopes ().bottom ());

  if (scoped_name)
    {
      node = scope->lookup_by_name (scoped_name);
    }

  if (!node)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("Failed to Find AST Node named %C!\n"),
        name_.c_str (), name));
      failed ();
    }

  if (scoped_name)
    {
      scoped_name->destroy ();
      delete scoped_name;
    }

  return node;
}

AST_Annotation_Decl *
Annotation_Test::assert_annotation_decl (const char *name)
{
  AST_Annotation_Decl *node = assert_node<AST_Annotation_Decl> (name);

  if (node->node_type () != AST_Decl::NT_annotation_decl)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("AST Node named %C is not an AST_Annotation_Decl!\n"),
        name_.c_str (), name));
      failed ();
    }

  return node;
}

void
Annotation_Test::assert_annotation_appl_count (
  AST_Decl *node, size_t count)
{
  AST_Annotation_Appls &annotations = node->annotations ();
  if (annotations.size () != count)
    {
      char *node_name = node->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("asserted that %C has %d annotation(s), but there are %d!\n"),
        name_.c_str (), node_name, count, annotations.size ()));
      delete [] node_name;
      failed ();
    }
}

AST_Annotation_Appl *
Annotation_Test::assert_annotation_appl (
  AST_Decl *node, size_t index, AST_Annotation_Decl *anno_decl)
{
  if (!anno_decl)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("assert_annotation_appl: annotation decl is null!\n"),
        name_.c_str ()));
      failed ();
    }

  AST_Annotation_Appls &annotations = node->annotations ();
  if (!annotations.size ())
    {
      char *node_name = node->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("can not access %C annotation %d, ")
        ACE_TEXT ("it has no annotations!\n"),
        name_.c_str (), node_name, index));
      delete [] node_name;
      failed ();
    }

  if (index >= annotations.size ())
    {
      char *node_name = node->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("can not access %C annotation %d, ")
        ACE_TEXT ("it only has %d annotation(s)!\n"),
        name_.c_str (), node_name, index, annotations.size ()));
      delete [] node_name;
      failed ();
    }

  AST_Annotation_Appl *anno_appl = annotations[index];
  if (!anno_appl)
    {
      char *node_name = node->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("%C annotation %d is null!\n"),
        name_.c_str (), node_name, index));
      delete [] node_name;
      failed ();
    }
  if (anno_appl->annotation_decl () != anno_decl)
    {
      char *anno_appl_name = anno_appl->name ()->get_string_copy ();
      char *anno_decl_name = anno_decl->name ()->get_string_copy ();
      char *node_name = node->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("%C annotation %d is a %C, looking for a %C!\n"),
        name_.c_str (), node_name, index, anno_appl_name, anno_decl_name));
      delete [] anno_appl_name;
      delete [] anno_decl_name;
      delete [] node_name;
      failed ();
    }

  return anno_appl;
}

void
Annotation_Test::assert_annotation_member_count (
  AST_Annotation_Decl *anno_decl, size_t count)
{
  if (!anno_decl)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("assert_annotation_member_count: annotation decl is null!\n"),
        name_.c_str ()));
      failed ();
    }

  size_t actual_count = anno_decl->member_count ();
  if (actual_count != count)
    {
      char *anno_decl_name = anno_decl->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("%C should have %d members, but it actually has %d!\n"),
        name_.c_str (), anno_decl_name, count, actual_count));
      delete [] anno_decl_name;
      failed ();
    }
}

void
Annotation_Test::assert_annotation_member_count (
  AST_Annotation_Appl *anno_appl, size_t count)
{
  assert_annotation_member_count (
    dynamic_cast<AST_Annotation_Decl *> (anno_appl), count);
}

AST_Annotation_Member *
Annotation_Test::get_annotation_member (
  AST_Annotation_Decl *anno_decl, const char *name)
{
  AST_Decl *decl = (*anno_decl)[name];
  AST_Annotation_Member *member = decl ?
    dynamic_cast<AST_Annotation_Member *> (decl) : 0;
  if (!member)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("Could not get annotation member %C!\n"),
        name_.c_str (), name));
      failed ();
    }
  return member;
}

AST_Annotation_Member *
Annotation_Test::get_annotation_member (
  AST_Annotation_Appl *anno_appl, const char *name)
{
  return get_annotation_member (
    dynamic_cast<AST_Annotation_Decl *> (anno_appl), name);
}

void
Annotation_Test::assert_annotation_member_type (
  AST_Annotation_Member *member, AST_Expression::ExprType type)
{
  if (member->expr_type () != type)
    {
      char *member_name = member->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("For Annotation Member %C, ")
        ACE_TEXT ("expecting it to be a %C, but it is a %C!\n"),
        name_.c_str (), member_name,
        AST_Expression::exprtype_to_string (type),
        AST_Expression::exprtype_to_string (member->expr_type ())));
      delete [] member_name;
      failed ();
    }
}

void
Annotation_Test::assert_annotation_member_value (
  AST_Annotation_Member *member, AST_Expression *expected)
{
  AST_Expression *member_value = member->value ();
  if (!member_value)
    {
      char *member_name = member->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("For Annotation Member %C, ")
        ACE_TEXT ("expecting it to have a value, but it doesn't!\n"),
        name_.c_str (), member_name));
      delete [] member_name;
      failed ();
    }

  if (!expected)
    {
      char *member_name = member->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("For Annotation Member %C, ")
        ACE_TEXT ("expected value is null, can't compare!\n"),
        name_.c_str (), member_name));
      delete [] member_name;
      failed ();
    }

  bool equal;
  if (member_value->ev ()->et == AST_Expression::EV_ulong &&
      expected->ev ()->et == AST_Expression::EV_ulong)
    {
      // For Enums
      equal = expected->ev ()->u.ulval == member_value->ev ()->u.ulval;
    }
  else
    {
      equal = (*expected) == member_value;
    }

  if (!equal)
    {
      char *member_name = member->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("For Annotation Member %C, expecting "),
        name_.c_str (), member_name));
      delete [] member_name;
      expected->dump (*ACE_DEFAULT_LOG_STREAM);
      ACE_ERROR ((LM_ERROR, ACE_TEXT (", got ")));
      member_value->dump (*ACE_DEFAULT_LOG_STREAM);
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("!\n")));
      failed ();
    }
}

void
Annotation_Test::assert_annotation_member_no_value (AST_Annotation_Member *member)
{
  AST_Expression *member_value = member->value ();
  if (member_value)
    {
      char *member_name = member->name ()->get_string_copy ();
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("For Annotation Member %C, ")
        ACE_TEXT ("expecting it to not have a value, but it does!\n"),
        name_.c_str (), member_name));
      delete [] member_name;
      failed ();
    }
}

void
Annotation_Test::set_scope (AST_Decl *scope_node)
{
  scope_ = dynamic_cast<UTL_Scope *> (scope_node);
  if (!scope_)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("Node passed to set_scope isn't a valid UTL_Scope!\n"),
        name_.c_str ()));
      failed ();
    }
}

void
Annotation_Test::disable_output ()
{
  disable_output_ = true;
}

void
Annotation_Test::results ()
{
  if (Annotation_Test::failed_test_count_)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test: %d out of %d subtests failed!\n"),
        failed_test_count_, total_test_count_));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("Annotation Test: All %d subtests passed\n"),
        total_test_count_));
    }
  idl_global->set_err_count (failed_test_count_);
}

void
Annotation_Test::print_idl_with_line_numbers ()
{
  static const char* start_marker =
#ifndef ACE_WIN32
    "\x1b[31m"
#endif
    ">";
  static const char* end_marker =
#ifndef ACE_WIN32
    "\x1b[0m"
#endif
    "";
  const long last_error_line = idl_global->err ()->last_error_lineno;
  const long marked_line = last_error_line != -1 ?
    last_error_line : idl_global->err ()->last_warning_lineno;
  const size_t char_count = idl_.length ();

  long line_number = 0;
  for (size_t start = 0; start < char_count;)
    {
      ++line_number;
      const size_t end = idl_.find ('\n', start);
      const std::string line = idl_.substr (start, end - start);
      const bool mark_line = line_number == marked_line;
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("%C%4u: %C%C\n"),
        mark_line ? start_marker : " ",
        line_number, line.c_str (),
        mark_line ? end_marker : ""));
      if (end == std::string::npos) {
        break;
      }
      start = end + 1;
    }
}

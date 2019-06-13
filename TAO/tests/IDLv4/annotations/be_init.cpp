#include "tao/Version.h"
#include "global_extern.h"
#include "be_extern.h"
#include "drv_extern.h"
#include "idl_version.h"

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

#ifndef ACE_LACKS_IOSTREAM_TOTALLY
#  include "ace/streams.h"
#endif

#include "utl_identifier.h"
#include "ast_generator.h"
#include "utl_scope.h"
#include "fe_declarator.h"
#include "ast_field.h"
#include "ast_annotation_decl.h"
#include "ast_annotation_member.h"
#include "ast_typedef.h"
#include "ast_sequence.h"
#include "ast_union.h"
#include "ast_enum_val.h"
#include "utl_string.h"
#include "ast_array.h"

void
BE_version ()
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("annotest_idl version ") ACE_TEXT (TAO_VERSION)
    ACE_TEXT ("\n")));
}

int
BE_init (int &, ACE_TCHAR *[])
{
  ACE_NEW_RETURN (be_global, BE_GlobalData, -1);

  // Enable Annotations
  idl_global->default_idl_version_ = IDL_VERSION_4;

  // Disable Trying to Parse File Arguments and creating File Output
  idl_global->ignore_files_ = true;

  return 0;
}

/// Test Statistics
///{
static unsigned failed_test_count = 0;
static unsigned total_test_count = 0;
///}

struct Failed {};

class Annotation_Test {
public:
  const char *name_;
  const char *idl_;
  bool failed_;
  int error_count_;
  UTL_Error::ErrorCode last_error_, last_warning_;

  explicit Annotation_Test (const char *name)
    : name_ (name),
      idl_ (0),
      failed_ (false),
      error_count_ (0),
      last_error_ (UTL_Error::EIDL_OK),
      last_warning_ (UTL_Error::EIDL_OK)
  {
    total_test_count++;
  }

  ~Annotation_Test ()
  {
    if (idl_global->err ()->last_error == UTL_Error::EIDL_SYNTAX_ERROR)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("Annotation Test: %C: ")
          ACE_TEXT ("FAILED because of syntax error in:\n%C\n")
          ACE_TEXT ("Check syntax error message above for more information.\n"),
          ACE_TEXT ("Failures beyond this might be false positives.\n"),
          name_, idl_));
        ++failed_test_count;
      }
    else if (!failed_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("Annotation Test: %C: PASSED\n"), name_));
      }
  }

  void failed (const char *message = 0)
  {
    if (message)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C: %C\n"),
          name_, message));
      }
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("Annotation Test: %C: FAILED\nFailed IDL:\n%C\n"),
      name_, idl_));
    failed_test_count++;
    failed_ = true;
    throw Failed ();
  }

  Annotation_Test &error_count (int error_count)
  {
    error_count_ = error_count;
    return *this;
  }

  Annotation_Test &last_error (UTL_Error::ErrorCode last_error)
  {
    last_error_ = last_error;
    return *this;
  }

  Annotation_Test &last_warning (UTL_Error::ErrorCode last_warning)
  {
    last_warning_ = last_warning;
    return *this;
  }

  Annotation_Test &run (const char *idl)
  {
    // Reset Error State
    idl_global->set_err_count (0);
    idl_global->err ()->last_error = UTL_Error::EIDL_OK;
    idl_global->err ()->last_warning = UTL_Error::EIDL_OK;

    // Eval IDL
    idl_ = idl;
    idl_global->eval (idl);

    // Look at Results
    if (idl_global->err_count () != error_count_)
      {
        failed_ = true;
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\nError Count: expecting %d, got %d!\n"),
          name_, error_count_, idl_global->err_count ()));
      }
    if (idl_global->err ()->last_error != last_error_)
      {
        failed_ = true;
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("Last Error Code (UTL_Error::ErrorCode): expecting "),
          name_));
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
          name_));
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

  AST_Decl *assert_node (const char *name, UTL_Scope *from = 0)
  {
    AST_Decl *node = 0;
    UTL_ScopedName *scoped_name = FE_Utils::string_to_scoped_name (name);

    UTL_Scope *scope = from ? from : idl_global->scopes ().bottom ();

    if (scoped_name)
      {
        node = scope->lookup_by_name (scoped_name);
      }

    if (!node)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("Failed to Find AST Node named %C!\n"),
          name_, name));
        failed ();
      }

    if (scoped_name)
      {
        scoped_name->destroy ();
        delete scoped_name;
      }

    return node;
  }

  AST_Annotation_Decl *assert_annotation_decl (const char *name)
  {
    AST_Annotation_Decl *node = dynamic_cast<AST_Annotation_Decl *>(
      assert_node (name));

    if (node->node_type () != AST_Decl::NT_annotation_decl)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("AST Node named %C is not an AST_Annotation_Decl!\n"),
          name_, name));
        failed ();
      }

    return node;
  }

  void assert_annotation_appl_count (
    AST_Decl *node, size_t count)
  {
    AST_Annotation_Appls &annotations = node->annotations ();
    if (annotations.size () != count)
      {
        char *node_name = node->name ()->get_string_copy ();
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("asserting %C has %d annotations, but there are %d!\n"),
          name_, node_name, count, annotations.size ()));
        delete [] node_name;
        failed ();
      }
  }

  AST_Annotation_Appl *assert_annotation_appl (
    AST_Decl *node, size_t index, AST_Annotation_Decl *anno_decl)
  {
    if (!anno_decl)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("assert_annotation_appl: annotation decl is null!\n"),
          name_));
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
          name_, node_name, index));
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
          name_, node_name, index, annotations.size ()));
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
          name_, node_name, index));
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
          name_, node_name, index, anno_appl_name, anno_decl_name));
        delete [] anno_appl_name;
        delete [] anno_decl_name;
        delete [] node_name;
        failed ();
      }

    return anno_appl;
  }

  void assert_annotation_member_count (
    AST_Annotation_Decl *anno_decl, size_t count)
  {
    if (!anno_decl)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("assert_annotation_member_count: annotation decl is null!\n"),
          name_));
        failed ();
      }

    size_t actual_count = anno_decl->member_count ();
    if (actual_count != count)
      {
        char *anno_decl_name = anno_decl->name ()->get_string_copy ();
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("%C should have %d members, but it actually has %d!\n"),
          name_, anno_decl_name, count, actual_count));
        delete [] anno_decl_name;
        failed ();
      }
  }

  void assert_annotation_member_count (
    AST_Annotation_Appl *anno_appl, size_t count)
  {
    assert_annotation_member_count (
      dynamic_cast<AST_Annotation_Decl *>(anno_appl), count);
  }

  AST_Annotation_Member *get_annotation_member (
    AST_Annotation_Decl *anno_decl, const char *name)
  {
    AST_Decl *decl = (*anno_decl)[name];
    AST_Annotation_Member *member = decl ?
        AST_Annotation_Member::narrow_from_decl (decl) : 0;
    if (!member)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("Could not get annotation member %C!\n"),
          name_, name));
        failed ();
      }
    return member;
  }

  AST_Annotation_Member *get_annotation_member (
    AST_Annotation_Appl *anno_appl, const char *name)
  {
    return get_annotation_member(
      dynamic_cast<AST_Annotation_Decl *>(anno_appl), name);
  }

  void assert_annotation_member_type (
    AST_Annotation_Member *member, AST_Expression::ExprType type)
  {
    if (member->expr_type () != type)
      {
        char *member_name = member->name ()->get_string_copy ();
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("For Annotation Member %C, ")
          ACE_TEXT ("expecting it to be a %C, but it is a %C!\n"),
          name_, member_name,
          AST_Expression::exprtype_to_string (type),
          AST_Expression::exprtype_to_string (member->expr_type ())));
        delete [] member_name;
        failed ();
      }
  }

  void assert_annotation_member_value (
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
          name_, member_name));
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
          name_, member_name));
        delete [] member_name;
        failed ();
      }

    bool equal;
    if (member_value->ev ()->et == AST_Expression::EV_ulong &&
        expected->ev ()->et == AST_Expression::EV_ulong)
      {
        // For Enums
        equal = expected->ev()->u.ulval == member_value->ev()->u.ulval;
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
          ACE_TEXT ("For Annotation Member %C, ")
          ACE_TEXT ("expecting "),
          name_, member_name));
        delete [] member_name;
        expected->dump (*ACE_DEFAULT_LOG_STREAM);
        ACE_ERROR ((LM_ERROR, ACE_TEXT (", got ")));
        member_value->dump (*ACE_DEFAULT_LOG_STREAM);
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("!\n")));
        failed ();
      }
  }

  template <typename T1, typename T2>
  void assert_annotation_member_value (
    AST_Annotation_Member *member, T1 expected_value)
  {
    AST_Expression expected (static_cast<T2> (expected_value));
    assert_annotation_member_value (member, &expected);
  }

  void assert_annotation_member_no_value (AST_Annotation_Member *member)
  {
    AST_Expression *member_value = member->value ();
    if (member_value)
      {
        char *member_name = member->name ()->get_string_copy ();
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("For Annotation Member %C, ")
          ACE_TEXT ("expecting it to not have a value, but it does!\n"),
          name_, member_name));
        delete [] member_name;
        failed ();
      }
  }
};

void
BE_post_init (char *[], long)
{
  /*
   * Notes About These Tests
   * =========================================================================
   *
   * - They are in the same IDL namespace, so they can conflict with each
   *   other.
   *
   * - You can't test for a syntax error really because tao_idl throws an
   *   exception for them. Even if the exception was caught, the AST might be
   *   in an invalid state afterwards.
   *
   * - Annotations local names internally start with @ so that they don't
   *   conflict with other non-annotation names. See below for examples.
   *
   * - Some of these tests intentionally cause errors.
   */

  /* -------------------------------------------------------------------------
   * Annotations Declarations
   * -------------------------------------------------------------------------
   * These tests assert that annotations can be declared.
   */

  AST_Annotation_Decl *test_annotation_1 = 0;
  try {
    Annotation_Test t ("Annotation Declaration with No Members");
    test_annotation_1 = t.run (
      "@annotation test_annotation_1 {\n"
      "};\n"
    ).assert_annotation_decl ("::@test_annotation_1");
    t.assert_annotation_member_count (test_annotation_1, 0);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Declaration with Members");
    AST_Annotation_Decl *test_annotation_2 = t.run (
      "@annotation test_annotation_2 {\n"
      "  short short_value;\n"
      "  char char_value;\n"
      "  long long_value;\n"
      "  boolean boolean_value;\n"
      "};\n"
    ).assert_annotation_decl ("::@test_annotation_2");
    t.assert_annotation_member_count (test_annotation_2, 4);

    AST_Annotation_Member *short_value =
      t.get_annotation_member (test_annotation_2, "short_value");
    t.assert_annotation_member_type (short_value, AST_Expression::EV_short);
    t.assert_annotation_member_no_value (short_value);

    AST_Annotation_Member *char_value =
      t.get_annotation_member (test_annotation_2, "char_value");
    t.assert_annotation_member_type (char_value, AST_Expression::EV_char);
    t.assert_annotation_member_no_value (char_value);

    AST_Annotation_Member *long_value =
      t.get_annotation_member (test_annotation_2, "long_value");
    t.assert_annotation_member_type (long_value, AST_Expression::EV_long);
    t.assert_annotation_member_no_value (long_value);

    AST_Annotation_Member *boolean_value =
      t.get_annotation_member (test_annotation_2, "boolean_value");
    t.assert_annotation_member_type (boolean_value, AST_Expression::EV_bool);
    t.assert_annotation_member_no_value (boolean_value);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Declaration with Defaulted Members");
    AST_Annotation_Decl *test_annotation_3 = t.run (
      "@annotation test_annotation_3 {\n"
      "  short short_value default 1;\n"
      "  char char_value default '&';\n"
      "  long long_value default -1;\n"
      "  boolean boolean_value default FALSE;\n"
      "};\n"
    ).assert_annotation_decl ("::@test_annotation_3");
    t.assert_annotation_member_count (test_annotation_3, 4);

    AST_Annotation_Member *short_value =
      t.get_annotation_member (test_annotation_3, "short_value");
    t.assert_annotation_member_type (short_value, AST_Expression::EV_short);
    t.assert_annotation_member_value<short, ACE_CDR::Short> (short_value, 1);

    AST_Annotation_Member *char_value =
      t.get_annotation_member (test_annotation_3, "char_value");
    t.assert_annotation_member_type (char_value, AST_Expression::EV_char);
    t.assert_annotation_member_value<char, ACE_CDR::Char> (char_value, '&');

    AST_Annotation_Member *long_value =
      t.get_annotation_member (test_annotation_3, "long_value");
    t.assert_annotation_member_type (long_value, AST_Expression::EV_long);
    t.assert_annotation_member_value<int, ACE_CDR::Long> (long_value, -1);

    AST_Annotation_Member *boolean_value =
      t.get_annotation_member (test_annotation_3, "boolean_value");
    t.assert_annotation_member_type (boolean_value, AST_Expression::EV_bool);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (boolean_value, false);
  } catch (Failed const &) {}

  AST_Annotation_Decl *test_annotation_4 = 0;
  try {
    Annotation_Test t ("Annotation Declaration with Mixed Members");
    test_annotation_4 = t.run (
      "@annotation test_annotation_4 {\n"
      "  short x;\n"
      "  short y default 0;\n"
      "};\n"
    ).assert_annotation_decl ("::@test_annotation_4");
    t.assert_annotation_member_count (test_annotation_4, 2);

    AST_Annotation_Member *x =
      t.get_annotation_member (test_annotation_4, "x");
    t.assert_annotation_member_type (x, AST_Expression::EV_short);
    t.assert_annotation_member_no_value (x);

    AST_Annotation_Member *y =
      t.get_annotation_member (test_annotation_4, "y");
    t.assert_annotation_member_type (y, AST_Expression::EV_short);
    t.assert_annotation_member_value<short, ACE_CDR::Short> (y, 0);
  } catch (Failed const &) {}

  AST_Annotation_Decl *test_annotation_in_module = 0;
  try {
    Annotation_Test t ("Annotation Declaration In Module");
    test_annotation_in_module = t.run (
      "module module_with_annotation_decl {\n"
      "  @annotation test_annotation {\n"
      "  };\n"
      "};\n"
    ).assert_annotation_decl (
      "::module_with_annotation_decl::@test_annotation");
    t.assert_annotation_member_count (test_annotation_in_module, 0);
  } catch (Failed const &) {}

  AST_Annotation_Decl *enum_annotation = 0;
  AST_Expression *enum_annotation_a = 0;
  AST_Expression *enum_annotation_b = 0;
  AST_Expression *enum_annotation_c = 0;
  try {
    Annotation_Test t ("Annotation Declaration with Enum");
    enum_annotation = t.run (
      "@annotation enum_annotation {\n"
      "  enum Enum_t {\n"
      "    A,\n"
      "    B,\n"
      "    C\n"
      "  };\n"
      "  Enum_t value default A;\n"
      "};\n"
    ).assert_annotation_decl ("@enum_annotation");
    t.assert_annotation_member_count (enum_annotation, 1);
    UTL_Scope *scope = dynamic_cast<UTL_Scope*>(enum_annotation);
    AST_Annotation_Member *value =
      t.get_annotation_member (enum_annotation, "value");

    AST_EnumVal *a = AST_EnumVal::narrow_from_decl(
      t.assert_node ("A", scope));
    enum_annotation_a = a->constant_value();

    AST_EnumVal *b = AST_EnumVal::narrow_from_decl(
      t.assert_node ("B", scope));
    enum_annotation_b = b->constant_value();

    AST_EnumVal *c = AST_EnumVal::narrow_from_decl(
      t.assert_node ("C", scope));
    enum_annotation_c = c->constant_value();

    t.assert_annotation_member_value (value, enum_annotation_a);
  } catch (Failed const &) {}

  AST_Annotation_Decl *string_annotation = 0;
  try {
    Annotation_Test t ("Annotation Declaration with String");
    string_annotation = t.run (
      "@annotation string_annotation {\n"
      "  string value default \"This is some text\";\n"
      "};\n"
    ).assert_annotation_decl ("@string_annotation");
    t.assert_annotation_member_count (string_annotation, 1);
    AST_Annotation_Member *value =
      t.get_annotation_member (string_annotation, "value");

    UTL_String test_string("This is some text");
    t.assert_annotation_member_value<UTL_String*, UTL_String*>
      (value, &test_string);
  } catch (Failed const &) {}

  AST_Expression *constant_annotation_x = 0;
  AST_Expression *constant_annotation_y = 0;
  AST_Annotation_Decl *constant_annotation = 0;
  try {
    Annotation_Test t ("Annotation Declaration with Constant");
    constant_annotation = t.run (
      "@annotation constant_annotation {\n"
      "  const short X = 4;\n"
      "  const short Y = 5;\n"
      "  short value default X;\n"
      "};\n"
    ).assert_annotation_decl ("@constant_annotation");
    t.assert_annotation_member_count (constant_annotation, 1);
    UTL_Scope *scope = dynamic_cast<UTL_Scope*>(constant_annotation);
    AST_Annotation_Member *value =
      t.get_annotation_member (constant_annotation, "value");

    AST_Constant *x = AST_Constant::narrow_from_decl(
      t.assert_node ("X", scope));
    constant_annotation_x = x->constant_value();

    AST_Constant *y = AST_Constant::narrow_from_decl(
      t.assert_node ("Y", scope));
    constant_annotation_y = y->constant_value();

    t.assert_annotation_member_value (value, constant_annotation_x);
  } catch (Failed const &) {}

  AST_Annotation_Decl *boolean_annotation = 0;
  try {
    Annotation_Test t ("Annotation Declaration with Single Boolean");
    boolean_annotation = t.run (
      "@annotation boolean_annotation {\n"
      "  boolean value default TRUE;\n"
      "};\n"
    ).assert_annotation_decl ("@boolean_annotation");
    t.assert_annotation_member_count (boolean_annotation, 1);
    AST_Annotation_Member *value =
      t.get_annotation_member (boolean_annotation, "value");
    t.assert_annotation_member_type (value, AST_Expression::EV_bool);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (value, true);
  } catch (Failed const &) {}

  /* -------------------------------------------------------------------------
   * Annotations Applications
   * -------------------------------------------------------------------------
   * These tests assert that annotations can be applied to various IDL
   * constructs.
   */

  try {
    Annotation_Test t ("Module Annotation Application");
    AST_Decl *module1 = t.run (
      "@test_annotation_1\n"
      "module module1 {\n"
      "  struct struct_in_module1 {\n"
      "    short member;\n"
      "  };\n"
      "};\n"
    ).assert_node ("::module1");
    t.assert_annotation_appl_count (module1, 1);
    t.assert_annotation_appl (module1, 0, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Struct Annotation Application");
    AST_Decl *struct1 = t.run (
      "@test_annotation_1\n"
      "struct struct1 {\n"
      "  short member;\n"
      "};\n"
    ).assert_node ("::struct1");
    t.assert_annotation_appl_count (struct1, 1);
    t.assert_annotation_appl (struct1, 0, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Typedef Annotation Application");
    t.run (
      "@enum_annotation\n"
      "typedef short short_int;\n"
      "@string_annotation\n"
      "typedef short_int small_int;\n"
      "@test_annotation_1\n"
      "typedef small_int i16;\n"
      "struct struct6 {\n"
      "  i16 member;\n"
      "};\n"
    );

    // Assert short_int has enum_annotation
    AST_Decl *short_int = t.assert_node ("short_int");
    t.assert_annotation_appl_count (short_int, 1);
    t.assert_annotation_appl (short_int, 0, enum_annotation);

    // Get type of member
    AST_Decl *member_decl = t.assert_node ("struct6::member");
    AST_Field *member = AST_Field::narrow_from_decl (member_decl);
    if (!member)
      {
        t.failed ("Could Not Get member");
      }
    AST_Decl* type = dynamic_cast<AST_Decl *> (member->field_type ());

    // Assert type has enum_annotation, string_annotation, and
    // test_annotation_1.
    t.assert_annotation_appl_count (type, 3);
    t.assert_annotation_appl (type, 0, enum_annotation);
    t.assert_annotation_appl (type, 1, string_annotation);
    t.assert_annotation_appl (type, 2, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Sequence Type Parameter Annotation Application");
    AST_Decl *value_decl = t.run (
      "typedef sequence<@test_annotation_1 short, 5> test_seq_t;\n"
      "struct struct7 {\n"
      "  test_seq_t value;\n"
      "};\n"
    ).assert_node ("::struct7::value");

    // Get Sequence
    AST_Field *value = AST_Field::narrow_from_decl (value_decl);
    if (!value)
      {
        t.failed (
          "Could Not Convert struct7::value from AST_Decl into AST_Field");
      }
    AST_Typedef *typedef_node =
      dynamic_cast<AST_Typedef *> (value->field_type ());
    if (!typedef_node)
      {
        t.failed (
          "Could Not Convert test_seq_t from AST_Decl into AST_Typedef");
      }
    AST_Decl *seq_decl = dynamic_cast<AST_Decl *>(typedef_node->base_type ());
    AST_Sequence *seq = AST_Sequence::narrow_from_decl (seq_decl);
    if (!seq)
      {
        t.failed (
          "Could Not Convert test_seq_t from AST_Typedef in AST_Sequence");
      }

    // Verify Annotation on Base Type
    AST_Annotation_Appls &annotations = seq->base_type_annotations ();
    size_t count = annotations.size ();
    if (count != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected one annotation on test_seq_t base type, ")
          ACE_TEXT ("it has %d annotations!\n"),
          t.name_, count));
        t.failed ();
      }
    AST_Annotation_Appl *annotation = annotations[0];
    if (!annotation)
      {
        t.failed ("annotation for test_seq_t base type is null!");
      }
    if (annotation->annotation_decl () != test_annotation_1)
      {
        UTL_ScopedName *scopedname = annotation->name ();
        const char *name = scopedname ?
          scopedname-> get_string_copy () : "UNKNOWN";
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected annotation for test_seq_t base type to be ")
          ACE_TEXT ("test_annotation_1, but it was %C\n"),
          t.name_, name));
        if (scopedname)
          {
            delete [] name;
          }
        t.failed ();
      }
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Constant Declarations Annotation Application");
    AST_Decl *test_const = t.run (
      "@test_annotation_1\n"
      "const short test_const = 5;\n"
    ).assert_node ("test_const");
    t.assert_annotation_appl_count (test_const, 1);
    t.assert_annotation_appl (test_const, 0, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Multiple Annotation Applications");
    AST_Decl *struct3 = t.run (
      "@test_annotation_1\n"
      "@test_annotation_1\n"
      "struct struct3 {\n"
      "  short test_member_1;\n"
      "};\n"
    ).assert_node ("struct3");
    t.assert_annotation_appl_count (struct3, 2);
    t.assert_annotation_appl (struct3, 0, test_annotation_1);
    t.assert_annotation_appl (struct3, 1, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Application with a Single Parameter");
    AST_Decl *struct4 = t.run (
      "@test_annotation_4 (100)\n"
      "struct struct4 {\n"
      "  short test_member_1;\n"
      "};\n"
    ).assert_node ("struct4");
    t.assert_annotation_appl_count (struct4, 1);
    AST_Annotation_Appl *appl =
      t.assert_annotation_appl (struct4, 0, test_annotation_4);
    t.assert_annotation_member_count (appl, 2);

    AST_Annotation_Member *x = t.get_annotation_member (appl, "x");
    t.assert_annotation_member_value<short, ACE_CDR::Short> (x, 100);

    AST_Annotation_Member *y = t.get_annotation_member (appl, "y");
    t.assert_annotation_member_value<short, ACE_CDR::Short> (y, 0);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Application with Named Parameters");
    AST_Decl *struct2 = t.run (
      "@test_annotation_4 (x = 101, y = 102)\n"
      "struct struct2 {\n"
      "  short test_member_1;\n"
      "};\n"
    ).assert_node ("struct2");
    t.assert_annotation_appl_count (struct2, 1);
    AST_Annotation_Appl *appl =
      t.assert_annotation_appl (struct2, 0, test_annotation_4);
    t.assert_annotation_member_count (appl, 2);

    AST_Annotation_Member *x = t.get_annotation_member (appl, "x");
    t.assert_annotation_member_value<short, ACE_CDR::Short> (x, 101);

    AST_Annotation_Member *y = t.get_annotation_member (appl, "y");
    t.assert_annotation_member_value<short, ACE_CDR::Short> (y, 102);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Applications with Scoped Names");
    AST_Decl *struct5 = t.run (
      "@module_with_annotation_decl::test_annotation\n"
      "@::module_with_annotation_decl::test_annotation\n"
      "struct struct5 {\n"
      "  short test_member_1;\n"
      "};\n"
    ).assert_node ("struct5");
    t.assert_annotation_appl_count (struct5, 2);
    t.assert_annotation_appl (struct5, 0, test_annotation_in_module);
    t.assert_annotation_appl (struct5, 1, test_annotation_in_module);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Applications on/in Unions");
    AST_Decl *test_union_decl = t.run (
      /* Annotations on the union and the discriminator */
      "@test_annotation_1\n"
      "union test_union switch (@test_annotation_1 short) {\n"
      "case 0:\n"
      "case 1:\n"
         /* Annotation on a Union Member */
      "  @test_annotation_1 short union_member_1;\n"
      "default:\n"
      "  short union_member_2;\n"
      "};\n"
    ).assert_node ("test_union");
    AST_Union *test_union = AST_Union::narrow_from_decl (test_union_decl);
    if (!test_union)
      {
        t.failed ("failed to get test_union!");
      }

    // Annotation On Union
    t.assert_annotation_appl_count (test_union_decl, 1);
    t.assert_annotation_appl (test_union_decl, 0, test_annotation_1);

    // Annotation On Discriminator
    AST_Annotation_Appls &annotations = test_union->disc_annotations ();
    size_t count = annotations.size ();
    if (count != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected one annotation on test_union discriminator, ")
          ACE_TEXT ("it has %d annotations!\n"),
          t.name_, count));
        t.failed ();
      }
    AST_Annotation_Appl *annotation = annotations[0];
    if (!annotation)
      {
        t.failed ("annotation for test_seq_t base type is null!");
      }
    if (annotation->annotation_decl () != test_annotation_1)
      {
        UTL_ScopedName *scopedname = annotation->name ();
        const char *name = scopedname ?
          scopedname-> get_string_copy () : "UNKNOWN";
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected annotation for test_union discriminator to be ")
          ACE_TEXT ("test_annotation_1, but it was %C\n"),
          t.name_, name));
        if (scopedname)
          {
            delete [] name;
          }
        t.failed ();
      }

    // Annotation on Union Member
    AST_Decl *union_member_1 =
      t.assert_node ("test_union::union_member_1");
    t.assert_annotation_appl_count (union_member_1, 1);
    t.assert_annotation_appl (union_member_1, 0, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Applications on/in Enums");
    AST_Decl *Test_Enum = t.run (
      /* Annotation on the enum */
      "@test_annotation_1\n"
      "enum Test_Enum {\n"
      "  TEST_ENUM_MEMBER_1,\n"
      /* Annotation on a enumerator */
      "  @test_annotation_1 TEST_ENUM_MEMBER_2,\n"
      "  TEST_ENUM_MEMBER_3\n"
      "};\n"
    ).assert_node ("Test_Enum");

    // Annotation on Enum
    t.assert_annotation_appl_count (Test_Enum, 1);
    t.assert_annotation_appl (Test_Enum, 0, test_annotation_1);

    // Annotation on Enum Member
    AST_Decl *TEST_ENUM_MEMBER_2 =
      t.assert_node ("Test_Enum::TEST_ENUM_MEMBER_2");
    t.assert_annotation_appl_count (TEST_ENUM_MEMBER_2, 1);
    t.assert_annotation_appl (TEST_ENUM_MEMBER_2, 0, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("By Default, Unknown Annotation Application Causes Warning");
    t.last_warning (UTL_Error::EIDL_LOOKUP_ERROR);
    t.run (
      "struct struct11 {\n"
      "  @fake_annotation(fake_param=FAKE_CONSTANT)\n"
      "  short member;\n"
      "};\n"
    );
  } catch (Failed const &) {}

  try {
    idl_global->unknown_annotations_ =
      IDL_GlobalData::UNKNOWN_ANNOTATIONS_ERROR;
    Annotation_Test t ("Optionally, Unknown Annotation Application Causes Err");
                // Any mention of "Error" will be picked up by scoreboard ^^^
    t.last_error (UTL_Error::EIDL_LOOKUP_ERROR).error_count (1);
    t.run (
      "struct struct10 {\n"
      "  @fake_annotation(fake_param=FAKE_CONSTANT)\n"
      "  short member;\n"
      "};\n"
    );
    // Restore Default Behaivor
    idl_global->unknown_annotations_ =
      IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ONCE;
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Application with Enum");
    AST_Decl *value = t.run (
      "struct struct8 {\n"
      "  @enum_annotation\n" // A
      "  @enum_annotation(B)\n"
      "  @enum_annotation(value=C)\n"
      "  short value;\n"
      "};\n"
    ).assert_node ("struct8::value");
    t.assert_annotation_appl_count (value, 3);
    AST_Annotation_Member *member;

    AST_Annotation_Appl *first =
      t.assert_annotation_appl (value, 0, enum_annotation);
    member = t.get_annotation_member (first, "value");
    t.assert_annotation_member_value (member, enum_annotation_a);

    AST_Annotation_Appl *second =
      t.assert_annotation_appl (value, 1, enum_annotation);
    member = t.get_annotation_member (second, "value");
    t.assert_annotation_member_value (member, enum_annotation_b);

    AST_Annotation_Appl *third =
      t.assert_annotation_appl (value, 2, enum_annotation);
    member = t.get_annotation_member (third, "value");
    t.assert_annotation_member_value (member, enum_annotation_c);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Application with String");
    AST_Decl *value = t.run (
      "struct struct9 {\n"
      "  @string_annotation\n" // A
      "  @string_annotation(\"Something else\")\n"
      "  @string_annotation(value=\"One last thing\")\n"
      "  short value;\n"
      "};\n"
    ).assert_node ("struct9::value");
    t.assert_annotation_appl_count (value, 3);
    AST_Annotation_Member *member;
    AST_Annotation_Appl *annotation;

    UTL_String first_string("This is some text");
    annotation = t.assert_annotation_appl (value, 0, string_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value <UTL_String *, UTL_String *>
      (member, &first_string);

    UTL_String second_string("Something else");
    annotation = t.assert_annotation_appl (value, 1, string_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value <UTL_String *, UTL_String *>
      (member, &second_string);

    UTL_String third_string("One last thing");
    annotation = t.assert_annotation_appl (value, 2, string_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value <UTL_String *, UTL_String *>
      (member, &third_string);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Application with Constant");
    AST_Decl *value = t.run (
      "struct struct12 {\n"
      "  @constant_annotation\n" // A
      "  @constant_annotation(Y)\n"
      "  @constant_annotation(100)\n"
      "  short value;\n"
      "};\n"
    ).assert_node ("struct12::value");
    t.assert_annotation_appl_count (value, 3);
    AST_Annotation_Member *member;
    AST_Annotation_Appl *annotation;

    annotation = t.assert_annotation_appl (value, 0, constant_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value (member, constant_annotation_x);

    annotation = t.assert_annotation_appl (value, 1, constant_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value (member, constant_annotation_y);

    annotation = t.assert_annotation_appl (value, 2, constant_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value<short, ACE_CDR::Short> (member, 100);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotate Array Base Type");
    AST_Typedef *thetypedef = AST_Typedef::narrow_from_decl (t.run (
      "typedef struct12 struct12Array @test_annotation_1 [12];\n"
    ).assert_node ("::struct12Array"));
    AST_Array *struct12Array =
      dynamic_cast<AST_Array *> (thetypedef->base_type ());

    // Verify Annotation on Base Type
    AST_Annotation_Appls &annotations =
      struct12Array->base_type_annotations ();
    size_t count = annotations.size ();
    if (count != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected one annotation on struct12Array base type, ")
          ACE_TEXT ("it has %d annotations!\n"),
          t.name_, count));
        t.failed ();
      }
    AST_Annotation_Appl *annotation = annotations[0];
    if (!annotation)
      {
        t.failed ("annotation for struct12Array base type is null!");
      }
    if (annotation->annotation_decl () != test_annotation_1)
      {
        UTL_ScopedName *scopedname = annotation->name ();
        const char *name = scopedname ?
          scopedname-> get_string_copy () : "UNKNOWN";
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected annotation for struct12Array base type to be ")
          ACE_TEXT ("test_annotation_1, but it was %C\n"),
          t.name_, name));
        if (scopedname)
          {
            delete [] name;
          }
        t.failed ();
      }
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Application with Single Boolean");
    t.run (
      "struct struct13 {\n"
      "  @boolean_annotation\n"
      "  short test_member_1;\n"
      "  @boolean_annotation (TRUE)\n"
      "  short test_member_2;\n"
      "  @boolean_annotation (FALSE)\n"
      "  short test_member_3;\n"
      "  @boolean_annotation (value = TRUE)\n"
      "  short test_member_4;\n"
      "  @boolean_annotation (value = FALSE)\n"
      "  short test_member_5;\n"
      "};\n"
    );

    AST_Decl *struct_member = 0;
    AST_Annotation_Appl *appl = 0;

    struct_member = t.assert_node ("struct13::test_member_1");
    t.assert_annotation_appl_count (struct_member, 1);
    appl = t.assert_annotation_appl (struct_member, 0, boolean_annotation);
    t.assert_annotation_member_count (appl, 1);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (
      t.get_annotation_member (appl, "value"), true);

    struct_member = t.assert_node ("struct13::test_member_2");
    t.assert_annotation_appl_count (struct_member, 1);
    appl = t.assert_annotation_appl (struct_member, 0, boolean_annotation);
    t.assert_annotation_member_count (appl, 1);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (
      t.get_annotation_member (appl, "value"), true);

    struct_member = t.assert_node ("struct13::test_member_3");
    t.assert_annotation_appl_count (struct_member, 1);
    appl = t.assert_annotation_appl (struct_member, 0, boolean_annotation);
    t.assert_annotation_member_count (appl, 1);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (
      t.get_annotation_member (appl, "value"), false);

    struct_member = t.assert_node ("struct13::test_member_4");
    t.assert_annotation_appl_count (struct_member, 1);
    appl = t.assert_annotation_appl (struct_member, 0, boolean_annotation);
    t.assert_annotation_member_count (appl, 1);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (
      t.get_annotation_member (appl, "value"), true);

    struct_member = t.assert_node ("struct13::test_member_5");
    t.assert_annotation_appl_count (struct_member, 1);
    appl = t.assert_annotation_appl (struct_member, 0, boolean_annotation);
    t.assert_annotation_member_count (appl, 1);
    t.assert_annotation_member_value<bool, ACE_CDR::Boolean> (
      t.get_annotation_member (appl, "value"), false);
  } catch (Failed const &) {}

  /* -------------------------------------------------------------------------
   * Annotation Names
   * -------------------------------------------------------------------------
   * These tests assert various aspects of how annotations work in regards to
   * naming.
   */

  try {
    Annotation_Test t ("Annotation and Non-Annotation Names Can't Clash");
    t.run (
      "@annotation samename {\n"
      "};\n"
      "struct samename {\n"
      "  short member;\n"
      "};"
    );
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Names Can't Be \"annotation\"");
    t.last_error (UTL_Error::EIDL_MISC).error_count (1);
    t.run (
      "@annotation annotation {\n"
      "};\n"
    );
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotation Names Can Start with \"annotation\"");
    t.run (
      "@annotation annotationannotation {\n"
      "};\n"
      "@annotationannotation\n"
      "struct annotationannotation_struct {\n"
      "  short member;\n"
      "};\n"
    ).assert_annotation_decl ("::@annotationannotation");
  } catch (Failed const &) {}

  /* -------------------------------------------------------------------------
   * Struct Field Visibility Must be vis_NA
   * -------------------------------------------------------------------------
   * Test for: https://github.com/DOCGroup/ACE_TAO/issues/784
   *
   * In the bison file, visibility for valuetype state members (which are the
   * same class as normal fields) was being passed through the bison stack.
   * When adding support for annotations, the grammar was changed and it was
   * broken, causing bogus data to be passed to regular struct field as their
   * visibility.
   *
   * This is a test to assert that struct fields have vis_NA. This can't be put
   * anywhere else at the moment because this is the only test that's an
   * instance of the idl compiler.
   */
  try {
    Annotation_Test t ("Struct Field Visibility Must be vis_NA");
    AST_Decl *member_decl = t.assert_node ("struct1::member");
    AST_Field *member = AST_Field::narrow_from_decl (member_decl);
    if (!member)
      {
        t.failed ("Could Not Get member");
      }
    if (member->visibility() != AST_Field::vis_NA)
      {
        char buffer[100];
        ACE_OS::snprintf (&buffer[0], 100,
          "struct field visibility is %u, which is not equal to vis_NA",
          static_cast<unsigned> (member->visibility ()));
        t.failed (&buffer[0]);
      }
  } catch (Failed const &) {}

  // Done, Print Overall Results
  if (failed_test_count)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test: %d out of %d subtests failed!\n"),
        failed_test_count, total_test_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("Annotation Test: All %d subtests passed\n"),
        total_test_count));
    }
  idl_global->set_err_count (failed_test_count);
}

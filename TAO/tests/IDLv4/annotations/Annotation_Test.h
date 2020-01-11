#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

#ifndef ACE_LACKS_IOSTREAM_TOTALLY
#  include "ace/streams.h"
#endif

#include "ast_annotation_decl.h"
#include "ast_annotation_member.h"
#include "utl_identifier.h"
#include "ast_generator.h"
#include "utl_scope.h"
#include "fe_declarator.h"
#include "ast_field.h"
#include "ast_typedef.h"
#include "ast_sequence.h"
#include "ast_union.h"
#include "ast_enum_val.h"
#include "utl_string.h"
#include "ast_array.h"
#include "ast_interface.h"

struct Failed {};

class Annotation_Test {
public:
  const char *name_;
  const char *idl_;
  bool failed_;
  int error_count_;
  UTL_Error::ErrorCode last_error_, last_warning_;
  UTL_Scope *scope_;
  bool disable_output_;

  static unsigned failed_test_count_;
  static unsigned total_test_count_;

  explicit Annotation_Test (const char *name);

  ~Annotation_Test ();

  void failed (const char *message = 0);

  Annotation_Test &error_count (int error_count);

  Annotation_Test &last_error (UTL_Error::ErrorCode last_error);

  Annotation_Test &last_warning (UTL_Error::ErrorCode last_warning);

  Annotation_Test &run (const char *idl);

  AST_Decl *assert_node (const char *name, UTL_Scope *from = 0);

  template <typename T>
  T *assert_node (const char *name, UTL_Scope *from = 0)
  {
    T *cast = dynamic_cast<T*> (assert_node (name, from));
    if (!cast) {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Annotation Test Error: %C:\n")
        ACE_TEXT ("assert_node: Failed to cast node to requested type!\n"),
        name_));
      failed ();
    }
    return cast;
  }

  AST_Annotation_Decl *assert_annotation_decl (const char *name);

  void assert_annotation_appl_count (AST_Decl *node, size_t count);

  AST_Annotation_Appl *assert_annotation_appl (
    AST_Decl *node, size_t index, AST_Annotation_Decl *anno_decl);

  void assert_annotation_member_count (
    AST_Annotation_Decl *anno_decl, size_t count);

  void assert_annotation_member_count (
    AST_Annotation_Appl *anno_appl, size_t count);

  AST_Annotation_Member *get_annotation_member (
    AST_Annotation_Decl *anno_decl, const char *name);

  AST_Annotation_Member *get_annotation_member (
    AST_Annotation_Appl *anno_appl, const char *name);

  void assert_annotation_member_type (
    AST_Annotation_Member *member, AST_Expression::ExprType type);

  void assert_annotation_member_value (
    AST_Annotation_Member *member, AST_Expression *expected);

  template <typename T1, typename T2>
  void assert_annotation_member_value (
    AST_Annotation_Member *member, T1 expected_value)
  {
    AST_Expression expected (static_cast<T2> (expected_value));
    assert_annotation_member_value (member, &expected);
  }

  void assert_annotation_member_no_value (AST_Annotation_Member *member);

  void set_scope (AST_Decl *scope_node);

  void disable_output ();

  static void results ();
};

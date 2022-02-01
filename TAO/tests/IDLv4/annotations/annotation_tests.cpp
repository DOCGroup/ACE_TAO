#include "Annotation_Test.h"

#include <ast_valuetype.h>
#include <ast_porttype.h>
#include <ast_eventtype.h>
#include <ast_component.h>
#include <ast_union_branch.h>
#include <ast_union_label.h>
#include <ast_expression.h>
#include <string>

namespace {

  void assert_node_has_annotation (
    Annotation_Test &t, const char *node_name, AST_Annotation_Decl *annotation)
  {
    AST_Decl *node = t.assert_node (node_name);
    t.assert_annotation_appl_count (node, 1);
    t.assert_annotation_appl (node, 0, annotation);
  }

  /**
   * Common Test IDL for what the IDL4 grammer calls "attr_dcl"
   */
  const std::string common_attr_dcl_idl =
    "  @test_annotation_1\n"
    "  attribute short rw_attribute;\n"
    "  @test_annotation_1\n"
    "  readonly attribute short ro_attribute;\n";

  void assert_common_attr_dcl_idl (
    Annotation_Test &t, AST_Annotation_Decl *test_annotation_1)
  {
    assert_node_has_annotation (t, "rw_attribute", test_annotation_1);
    assert_node_has_annotation (t, "ro_attribute", test_annotation_1);
  }

  /**
   * Common Test IDL for what the IDL4 grammer calls "export"
   */
  const std::string common_export_idl =
    // op_dcl
    "  @test_annotation_1\n"
    "  void operation();\n"
    // attr_decl
    + common_attr_dcl_idl +
    // type_dcl
    "  @test_annotation_1\n"
    "  struct struct_in_export {\n"
    "    short value;\n"
    "  };\n"
    // const_dcl
    "  @test_annotation_1\n"
    "  const short const_value = 3;\n"
    // except_dcl
    "  @test_annotation_1\n"
    "  exception exception_in_export {\n"
    "    short value;\n"
    "  };\n"
    // Use expection
    "  @test_annotation_1\n"
    "  void operation_with_exception() raises (exception_in_export);\n"
    // type_id_dcl (Doesn't work)
    // type_prefix_dcl (No grammar issues, but a type_prefix isn't something
    //  that's part of the AST, so I'm not sure how this would work).
    // "  @test_annotation_1\n"
    // "  typeprefix struct_in_export \"electric_plants\";\n"
    // import_dcl (TAO_IDL has import as a keyword, but doesn't support it in the grammer)
    // op_oneway_dcl
    "  @test_annotation_1\n"
    "  oneway void oneway_op();\n";

  void assert_common_export_idl (
    Annotation_Test &t, AST_Annotation_Decl *test_annotation_1)
  {
    assert_node_has_annotation (t, "operation", test_annotation_1);
    assert_common_attr_dcl_idl (t, test_annotation_1);
    assert_node_has_annotation (t, "struct_in_export", test_annotation_1);
    assert_node_has_annotation (t, "const_value", test_annotation_1);
    assert_node_has_annotation (t, "exception_in_export", test_annotation_1);
    assert_node_has_annotation (t, "operation_with_exception", test_annotation_1);
    assert_node_has_annotation (t, "oneway_op", test_annotation_1);
  }

  /**
   * Common Test IDL for what the IDL4 grammer calls "value_element"
   */
  const std::string common_value_element_idl =
    // export
    common_export_idl +

    // state_member
    "  @test_annotation_1\n"
    "  public short public_state_member;\n"
    "  @test_annotation_1\n"
    "  private short private_state_member;\n"

    // init_dcl
    "  @test_annotation_1\n"
    "  factory factory_thing();\n";

  void assert_common_value_element_idl (
    Annotation_Test &t, AST_Annotation_Decl *test_annotation_1)
  {
    assert_common_export_idl (t, test_annotation_1);
    assert_node_has_annotation (t, "public_state_member", test_annotation_1);
    assert_node_has_annotation (t, "private_state_member", test_annotation_1);
    assert_node_has_annotation (t, "factory_thing", test_annotation_1);
  }
}

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

void
annotation_tests ()
{
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
    t.set_scope (enum_annotation);
    AST_Annotation_Member *value =
      t.get_annotation_member (enum_annotation, "value");

    AST_EnumVal *a = t.assert_node<AST_EnumVal> ("A");
    enum_annotation_a = a->constant_value ();

    AST_EnumVal *b = t.assert_node<AST_EnumVal> ("B");
    enum_annotation_b = b->constant_value ();

    AST_EnumVal *c = t.assert_node<AST_EnumVal> ("C");
    enum_annotation_c = c->constant_value ();

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

    UTL_String test_string ("This is some text");
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
    t.set_scope (constant_annotation);
    AST_Annotation_Member *value =
      t.get_annotation_member (constant_annotation, "value");

    constant_annotation_x = t.assert_node<AST_Constant> ("X")->constant_value ();
    constant_annotation_y = t.assert_node<AST_Constant> ("Y")->constant_value ();

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
    AST_Field *member= t.assert_node<AST_Field> ("struct6::member");
    AST_Type* type = member->field_type ();

    // Assert type has enum_annotation, string_annotation, and
    // test_annotation_1.
    t.assert_annotation_appl_count (type, 3);
    t.assert_annotation_appl (type, 0, enum_annotation);
    t.assert_annotation_appl (type, 1, string_annotation);
    t.assert_annotation_appl (type, 2, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Sequence Type Parameter Annotation Application");
    AST_Field *value = t.run (
      "typedef sequence<@test_annotation_1 short, 5> test_seq_t;\n"
      "struct struct7 {\n"
      "  test_seq_t value;\n"
      "};\n"
    ).assert_node<AST_Field> ("::struct7::value");

    // Get Sequence
    AST_Typedef *typedef_node = dynamic_cast<AST_Typedef *> (value->field_type ());
    if (!typedef_node) t.failed ("Could not get AST_Typedef");
    AST_Sequence *seq = dynamic_cast<AST_Sequence *> (typedef_node->base_type ());
    if (!seq) t.failed ("Could get AST_Sequence");

    // Verify Annotation on Base Type
    AST_Annotation_Appls &annotations = seq->base_type_annotations ();
    size_t count = annotations.size ();
    if (count != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected one annotation on test_seq_t base type, ")
          ACE_TEXT ("it has %d annotations!\n"),
          t.name_.c_str (), count));
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
          t.name_.c_str (), name));
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
    AST_Union *test_union = t.run (
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
    ).assert_node<AST_Union> ("test_union");

    // Annotation On Union
    t.assert_annotation_appl_count (test_union, 1);
    t.assert_annotation_appl (test_union, 0, test_annotation_1);

    // Annotation On Discriminator
    AST_Annotation_Appls &annotations = test_union->disc_annotations ();
    size_t count = annotations.size ();
    if (count != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected one annotation on test_union discriminator, ")
          ACE_TEXT ("it has %d annotations!\n"),
          t.name_.c_str (), count));
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
          t.name_.c_str (), name));
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
    t.disable_output ();
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
    Annotation_Test t ("Optionally, Unknown Annotation Application Causes Err0r");
                // Any mention of "Error" will be picked up by scoreboard ^^^
    t.last_error (UTL_Error::EIDL_LOOKUP_ERROR).error_count (1);
    t.disable_output ();
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

    UTL_String first_string ("This is some text");
    annotation = t.assert_annotation_appl (value, 0, string_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value <UTL_String *, UTL_String *>
      (member, &first_string);

    UTL_String second_string ("Something else");
    annotation = t.assert_annotation_appl (value, 1, string_annotation);
    member = t.get_annotation_member (annotation, "value");
    t.assert_annotation_member_value <UTL_String *, UTL_String *>
      (member, &second_string);

    UTL_String third_string ("One last thing");
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
    AST_Typedef *thetypedef = t.run (
      "typedef struct12 struct12Array @test_annotation_1 [12];\n"
    ).assert_node<AST_Typedef> ("::struct12Array");
    AST_Array *struct12Array =
      dynamic_cast<AST_Array *> (thetypedef->base_type ());
    if (!struct12Array) t.failed ("Could not get AST_Array");

    // Verify Annotation on Base Type
    AST_Annotation_Appls &annotations =
      struct12Array->base_type_annotations ();
    size_t count = annotations.size ();
    if (count != 1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Annotation Test Error: %C:\n")
          ACE_TEXT ("expected one annotation on struct12Array base type, ")
          ACE_TEXT ("it has %d annotations!\n"),
          t.name_.c_str (), count));
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
          t.name_.c_str (), name));
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

  try {
    Annotation_Test t ("Annotations on and in Interfaces");
    t.run ((std::string () +
      "@test_annotation_1\n"
      "interface interface1 {\n"
      // export
      + common_export_idl +
      "};\n"
    ).c_str ());

    AST_Interface *interface1 = t.assert_node<AST_Interface> ("interface1");
    t.assert_annotation_appl_count (interface1, 1);
    t.assert_annotation_appl (interface1, 0, test_annotation_1);
    t.set_scope (interface1);
    assert_common_export_idl (t, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotations on and in Valuetypes");
    t.run ((std::string () +
      "@test_annotation_1\n"
      "valuetype valuetype1 {\n"
      // value_element
      + common_value_element_idl +
      "};\n"
    ).c_str ());

    AST_ValueType *valuetype1 = t.assert_node<AST_ValueType> ("valuetype1");
    t.assert_annotation_appl_count (valuetype1, 1);
    t.assert_annotation_appl (valuetype1, 0, test_annotation_1);
    t.set_scope (valuetype1);
    assert_common_value_element_idl (t, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotations on and in Porttypes");
    t.run ((std::string () +
      "@test_annotation_1\n"
      "porttype port_with_provides {\n"
      // port_ref
      "  @test_annotation_1\n"
      "  provides interface1 provides_value;\n"
      // port_export
      + common_attr_dcl_idl +
      "};\n"
      "\n"
      "@test_annotation_1\n"
      "porttype port_with_uses {\n"
      // port_ref
      "  @test_annotation_1\n"
      "  uses interface1 uses_value;\n"
      // port_export
      + common_attr_dcl_idl +
      "};\n"
    ).c_str ());

    AST_PortType *port_with_provides =
      t.assert_node<AST_PortType> ("port_with_provides");
    t.assert_annotation_appl_count (port_with_provides, 1);
    t.assert_annotation_appl (port_with_provides, 0, test_annotation_1);
    t.set_scope (port_with_provides);
    assert_node_has_annotation (t, "provides_value", test_annotation_1);
    assert_common_attr_dcl_idl (t, test_annotation_1);

    AST_PortType *port_with_uses =
      t.assert_node<AST_PortType> ("port_with_uses");
    t.assert_annotation_appl_count (port_with_uses, 1);
    t.assert_annotation_appl (port_with_uses, 0, test_annotation_1);
    t.set_scope (port_with_uses);
    assert_node_has_annotation (t, "uses_value", test_annotation_1);
    assert_common_attr_dcl_idl (t, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotations on and in Eventtypes");
    t.run ((std::string () +
      "@test_annotation_1\n"
      "eventtype event1 {\n"
      + common_value_element_idl +
      "};\n"
    ).c_str ());
    AST_EventType *event1 = t.assert_node<AST_EventType> ("event1");
    t.assert_annotation_appl_count (event1, 1);
    t.assert_annotation_appl (event1, 0, test_annotation_1);
    t.set_scope (event1);
    assert_common_value_element_idl (t, test_annotation_1);
  } catch (Failed const &) {}

  try {
    Annotation_Test t ("Annotations on and in Components");
    t.run ((std::string () +
      "@test_annotation_1\n"
      "component component1 {\n"
      // provides_dcl
      "  @test_annotation_1\n"
      "  provides interface1 provides_value;\n"
      // uses_dcl
      "  @test_annotation_1\n"
      "  uses interface1 uses_value;\n"
      // attr_dcl
      + common_attr_dcl_idl +
      // port_dcl
      "  @test_annotation_1\n"
      "  port port_with_uses port_value;\n"
      // emits_dcl
      "  @test_annotation_1\n"
      "  emits event1 emits_value;\n"
      // publishes_dcl
      "  @test_annotation_1\n"
      "  publishes event1 publishes_value;\n"
      // consumes_dcl
      "  @test_annotation_1\n"
      "  consumes event1 consumes_value;\n"
      "};\n"
    ).c_str ());
    AST_Component *component1 = t.assert_node<AST_Component> ("component1");
    t.assert_annotation_appl_count (component1, 1);
    t.assert_annotation_appl (component1, 0, test_annotation_1);
    t.set_scope (component1);
    assert_node_has_annotation (t, "provides_value", test_annotation_1);
    assert_node_has_annotation (t, "uses_value", test_annotation_1);
    assert_common_attr_dcl_idl (t, test_annotation_1);
    assert_node_has_annotation (t, "port_value", test_annotation_1);
    assert_node_has_annotation (t, "emits_value", test_annotation_1);
    assert_node_has_annotation (t, "publishes_value", test_annotation_1);
    assert_node_has_annotation (t, "consumes_value", test_annotation_1);
  } catch (Failed const &) {}

  /*
   * Test for https://github.com/DOCGroup/ACE_TAO/issues/997
   *
   * When the original annotation work (https://github.com/DOCGroup/ACE_TAO/pull/723)
   * was done it was assumed that when annotations didn't define the symbol
   * being used, the lookup would go up the scope stack to the current scope.
   * This turned out not the case, so this functionality was implemented just
   * for annotation parameters.
   */
  try {
    Annotation_Test t ("Passing Constant from Module");
    t.run (
      "@annotation range_test_annotation {\n"
      "  float min;\n"
      "  float max;\n"
      "};\n"
      "\n"
      "module range_test_annoation_module {\n"
      "  const float f1 = 1.;\n"
      "  const float f2 = 2.;\n"
      "\n"
      "  @range_test_annotation(min = f1, max = f2)\n"
      "  @range_test_annotation(\n"
      "    min = range_test_annoation_module::f1,\n"
      "    max = range_test_annoation_module::f2)\n"
      "  @range_test_annotation(\n"
      "    min = ::range_test_annoation_module::f1,\n"
      "    max = ::range_test_annoation_module::f2)\n"
      "  typedef float RangedFloat;\n"
      "};\n"
    );

    AST_Annotation_Decl *range_like_test_annotation =
      t.assert_annotation_decl ("::@range_test_annotation");
    AST_Decl *RangedFloat = t.assert_node (
      "::range_test_annoation_module::RangedFloat");
    t.assert_annotation_appl_count (RangedFloat, 3);
    t.assert_annotation_appl (RangedFloat, 0, range_like_test_annotation);
    t.assert_annotation_appl (RangedFloat, 1, range_like_test_annotation);
    t.assert_annotation_appl (RangedFloat, 2, range_like_test_annotation);
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
    t.disable_output ();
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
    AST_Field *member = t.assert_node<AST_Field> ("struct1::member");
    if (member->visibility () != AST_Field::vis_NA)
      {
        char buffer[100];
        ACE_OS::snprintf (&buffer[0], 100,
          "struct field visibility is %u, which is not equal to vis_NA",
          static_cast<unsigned> (member->visibility ()));
        t.failed (&buffer[0]);
      }
  } catch (Failed const &) {}

  /* -------------------------------------------------------------------------
   * Empty union cases aliasing the default case must always be evaluated
   * -------------------------------------------------------------------------
   * When the union has an enum discriminator, and one or more empty cases
   * acting as an alias to the default case the IDL compiler was failing to
   * resolve the ordinal value for these empty labels and this causes trouble
   * for at least OpenDDS.
   *
   * This test is designed to verify that the condition is corrected by
   * parsing a specially crafted union and validating the value of the
   * label aliasing the default case.
   */
  try {
    Annotation_Test t ("empty union branch label");
    AST_Union *test_union = t.run (
               "enum disc {A, B, C};\n"
               "union empty_union switch (disc) {\n"
               "case A: long along;\n"
               "case B: short bshort;\n"
               "case C:\n"
               "default: float cfloat;\n"
               "};\n").assert_node<AST_Union>("::empty_union");
    AST_Field **af = 0;
    test_union->field(af, 2);
    AST_UnionBranch *ub = dynamic_cast<AST_UnionBranch *>(*af);
    if (ub != nullptr)
      {
         AST_UnionLabel *ul = ub->label ();
         if (ul != nullptr)
           {
              if (ul->label_val()->ev()->u.ulval != 2)
                {
                  t.failed("did not get the correct label value");
                }
           }
      }
} catch (Failed const &) {}

  // Done, Print Overall Results
  Annotation_Test::results ();
}

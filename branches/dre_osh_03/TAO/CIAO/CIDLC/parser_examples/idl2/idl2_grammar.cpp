// $Id$

#ifndef IDL2_GRAMMAR_C
#define IDL2_GRAMMAR_C

#include "idl2_grammar.h"

template<typename ScannerT>
rule<ScannerT> const &
skip_grammar::definition<ScannerT>::start (void) const
{
  return this->skip;
}

template<typename ScannerT>
skip_grammar::definition<ScannerT>::definition (
    skip_grammar const & /* self */
  )
{
  this->skip =
    space_p                                   // Skip whitespace
    | "//" >> *(anychar_p - eol_p) >> eol_p
//    | "//" >> *(anychar_p - '\n') >> !eol_p   // C++ comment
//  | comment_p("//") >> !end_p               // Not on MSVC 7.1
    | comment_p("/*", "*/")                   // C comment
    | comment_p("#line")                      // TODO
    | comment_p("#pragma")            
    ;
}

// ============================================================

template<typename ScannerT>
rule<ScannerT> const &
idl2_grammar::definition<ScannerT>::start (void) const
{
  return specification; 
}

template<typename ScannerT>
idl2_grammar::definition<ScannerT>::definition (
    idl2_grammar const & /* self */
  )
 : LSHIFT("<<"), RSHIFT(">>"), SCOPE("::"),
   SEMI(';'), COMMA(','), COLON(':'), ASSIGN('='),
   MINUS('-'), PLUS('+'), MULT('*'), DIV('/'), MOD('%'),
   AND('&'), NOT('~'), OR('|'), XOR('^'),
   LPAREN('('), RPAREN(')'), LANGLE('<'), RANGLE('>'), 
   LBRACK('['), RBRACK(']'), LBRACE('{'), RBRACE('}')
{
  // IDL2 keywords.
  keywords =
    "abstract", "any", "attribute", "boolean", "case", 
    "char", "const", "context", "custom", "default",
    "double", "enum", "exception", "factory", "FALSE", 
    "fixed", "float", "in", "inout", "interface",
    "local", "long", "module", "native", "Object", 
    "octet", "oneway", "out", "private", "public", 
    "raises", "readonly", "short", "sequence", "string", 
    "struct", "supports", "switch", "TRUE", "truncatable", 
    "typedef", "union", "unsigned", "ValueBase", 
    "valuetype", "void", "wchar", "wstring";

  // IDL2 tokens.
  IDL_ABSTRACT =    strlit<>("abstract");
  IDL_ANY =         strlit<>("any");
  IDL_ATTRIBUTE =   strlit<>("attribute");
  IDL_BOOLEAN =     strlit<>("boolean");
  IDL_CASE =        strlit<>("case");
  IDL_CHAR =        strlit<>("char");
  IDL_CONST =       strlit<>("const");
  IDL_CONTEXT =     strlit<>("context");
  IDL_CUSTOM =      strlit<>("custom");
  IDL_DEFAULT =     strlit<>("default");
  IDL_DOUBLE =      strlit<>("double");
  IDL_ENUM =        strlit<>("enum");
  IDL_EXCEPTION =   strlit<>("exception");
  IDL_FACTORY =     strlit<>("factory");
  IDL_FALSE =       strlit<>("FALSE");
  IDL_FIXED =       strlit<>("fixed");
  IDL_FLOAT =       strlit<>("float");
  IDL_IN =          strlit<>("in");
  IDL_INOUT =       strlit<>("inout");
  IDL_INTERFACE =   strlit<>("interface");
  IDL_LOCAL =       strlit<>("local");
  IDL_LONG =        strlit<>("long");
  IDL_MODULE =      strlit<>("module");
  IDL_NATIVE =      strlit<>("native");
  IDL_OBJECT =      strlit<>("Object");
  IDL_OCTET =       strlit<>("octet");
  IDL_ONEWAY =      strlit<>("oneway");
  IDL_OUT =         strlit<>("out");
  IDL_PRIVATE =     strlit<>("private");
  IDL_PUBLIC =      strlit<>("public");
  IDL_RAISES =      strlit<>("raises");
  IDL_READONLY =    strlit<>("readonly");
  IDL_SHORT =       strlit<>("short");
  IDL_SEQUENCE =    strlit<>("sequence");
  IDL_STRING =      strlit<>("string");
  IDL_STRUCT =      strlit<>("struct");
  IDL_SUPPORTS =    strlit<>("supports");
  IDL_SWITCH =      strlit<>("switch");
  IDL_TRUE =        strlit<>("TRUE");
  IDL_TRUNCATABLE = strlit<>("truncatable");
  IDL_TYPEDEF =     strlit<>("typedef");
  IDL_UNION =       strlit<>("union");
  IDL_UNSIGNED =    strlit<>("unsigned");
  IDL_VALUEBASE =   strlit<>("ValueBase");
  IDL_VALUETYPE =   strlit<>("valuetype");
  IDL_VOID =        strlit<>("void");
  IDL_WCHAR =       strlit<>("wchar");
  IDL_WSTRING =     strlit<>("wstring");

  // IDL2 identifiers.
  IDENTIFIER =
    lexeme_d
      [
        ((alpha_p | '_') >> *(alnum_p | '_'))
        - (keywords >> anychar_p - (alnum_p | '_'))
      ]
    ;
    
  // Character literals.
  CHARACTER_LITERAL = 
    anychar_p
    ;
  
  // Wide character literals.
  WIDE_CHARACTER_LITERAL =
    lexeme_d
      [
        chlit<>('L') >> anychar_p
      ]
    ;

  // String literals.
  STRING_LITERAL_PART =
    lexeme_d
      [
        chlit<>('\"') >>
        *(strlit<>("\"\"") | anychar_p - chlit<>('\"')) >>
        chlit<>('\"')
      ]
    ;

  STRING_LITERAL = +STRING_LITERAL_PART;
  
  // Wide string literals.
  WIDE_STRING_LITERAL_PART =
    chlit<>('L') >> STRING_LITERAL_PART
    ;
    
  WIDE_STRING_LITERAL = +WIDE_STRING_LITERAL_PART;

  // Integer literals.
  INTEGER_LITERAL_HEX =
    lexeme_d
      [
        chlit<>('0')
        >> nocase_d[chlit<>('x')]
        >> +xdigit_p
        >> !nocase_d[chlit<>('l') | chlit<>('u')]
      ]
    ;

  INTEGER_LITERAL_OCT =
    lexeme_d
      [
        chlit<>('0')
        >> +range<>('0', '7')
        >> !nocase_d[chlit<>('l') | chlit<>('u')]
      ]
    ;

  INTEGER_LITERAL_DEC =
    lexeme_d
      [
        +digit_p
        >> !nocase_d[chlit<>('l') | chlit<>('u')]
      ]
    ;

  INTEGER_LITERAL =
        INTEGER_LITERAL_HEX
    |   INTEGER_LITERAL_OCT
    |   INTEGER_LITERAL_DEC
    ;

  // Floating point literals.  
  // 12345[eE][+-]?123[lLfF]?
  FLOATING_PT_LITERAL_1 =
    lexeme_d
      [
        +digit_p
        >> (chlit<>('e') | chlit<>('E'))
        >> !(chlit<>('+') | chlit<>('-'))
        >> +digit_p
        >> !nocase_d[chlit<>('l') | chlit<>('f')]
      ]
      ;

  // .123([eE][+-]?123)?[lLfF]?
  FLOATING_PT_LITERAL_2 =
    lexeme_d
      [
        chlit<>('.')
        >> +digit_p
        >> !(   
              (chlit<>('e') | chlit<>('E'))
              >> !(chlit<>('+') | chlit<>('-'))
              >> +digit_p
            )
        >> !nocase_d[chlit<>('l') | chlit<>('f')]
      ]
    ;

  // 12345.(123)?([eE][+-]?123)?[lLfF]?
  FLOATING_PT_LITERAL_3 =
    lexeme_d
      [
        +digit_p
        >> chlit<>('.')
        >> *digit_p
        >> !(   
              (chlit<>('e') | chlit<>('E'))
              >> !(chlit<>('+') | chlit<>('-'))
              >> +digit_p
            )
        >> !nocase_d[chlit<>('l') | chlit<>('f')]
      ]
    ;

  FLOATING_PT_LITERAL =
      FLOATING_PT_LITERAL_1
    | FLOATING_PT_LITERAL_2
    | FLOATING_PT_LITERAL_3
    ;
    
  // Boolean literals.
  BOOLEAN_LITERAL =
      IDL_TRUE
    | IDL_FALSE
    ;

  // Debug support for terminals.
  BOOST_SPIRIT_DEBUG_RULE (IDL_ABSTRACT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_ANY);
  BOOST_SPIRIT_DEBUG_RULE (IDL_ATTRIBUTE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_BOOLEAN);
  BOOST_SPIRIT_DEBUG_RULE (IDL_CASE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_CHAR);
  BOOST_SPIRIT_DEBUG_RULE (IDL_CONST);
  BOOST_SPIRIT_DEBUG_RULE (IDL_CONTEXT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_CUSTOM);
  BOOST_SPIRIT_DEBUG_RULE (IDL_DEFAULT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_DOUBLE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_ENUM);
  BOOST_SPIRIT_DEBUG_RULE (IDL_EXCEPTION);
  BOOST_SPIRIT_DEBUG_RULE (IDL_FACTORY);
  BOOST_SPIRIT_DEBUG_RULE (IDL_FALSE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_FIXED);
  BOOST_SPIRIT_DEBUG_RULE (IDL_FLOAT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_IN);
  BOOST_SPIRIT_DEBUG_RULE (IDL_INOUT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_INTERFACE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_LOCAL);
  BOOST_SPIRIT_DEBUG_RULE (IDL_LONG);
  BOOST_SPIRIT_DEBUG_RULE (IDL_MODULE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_NATIVE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_OBJECT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_OCTET);
  BOOST_SPIRIT_DEBUG_RULE (IDL_ONEWAY);
  BOOST_SPIRIT_DEBUG_RULE (IDL_OUT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_PRIVATE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_PUBLIC);
  BOOST_SPIRIT_DEBUG_RULE (IDL_RAISES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_READONLY);
  BOOST_SPIRIT_DEBUG_RULE (IDL_SHORT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_SEQUENCE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_STRING);
  BOOST_SPIRIT_DEBUG_RULE (IDL_STRUCT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_SWITCH);
  BOOST_SPIRIT_DEBUG_RULE (IDL_TRUE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_TRUNCATABLE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_TYPEDEF);
  BOOST_SPIRIT_DEBUG_RULE (IDL_UNION);
  BOOST_SPIRIT_DEBUG_RULE (IDL_UNSIGNED);
  BOOST_SPIRIT_DEBUG_RULE (IDL_VALUEBASE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_VALUETYPE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_VOID);
  BOOST_SPIRIT_DEBUG_RULE (IDL_WCHAR);
  BOOST_SPIRIT_DEBUG_RULE (IDL_WSTRING);
  BOOST_SPIRIT_DEBUG_RULE (IDENTIFIER);
  BOOST_SPIRIT_DEBUG_RULE (STRING_LITERAL);
  BOOST_SPIRIT_DEBUG_RULE (WIDE_STRING_LITERAL);
  BOOST_SPIRIT_DEBUG_RULE (CHARACTER_LITERAL);
  BOOST_SPIRIT_DEBUG_RULE (WIDE_CHARACTER_LITERAL);
  BOOST_SPIRIT_DEBUG_RULE (INTEGER_LITERAL_HEX);
  BOOST_SPIRIT_DEBUG_RULE (INTEGER_LITERAL_OCT);
  BOOST_SPIRIT_DEBUG_RULE (INTEGER_LITERAL_DEC);
  BOOST_SPIRIT_DEBUG_RULE (INTEGER_LITERAL);
  BOOST_SPIRIT_DEBUG_RULE (FLOATING_PT_LITERAL_1);
  BOOST_SPIRIT_DEBUG_RULE (FLOATING_PT_LITERAL_2);
  BOOST_SPIRIT_DEBUG_RULE (FLOATING_PT_LITERAL_3);
  BOOST_SPIRIT_DEBUG_RULE (FLOATING_PT_LITERAL);
  BOOST_SPIRIT_DEBUG_RULE (BOOLEAN_LITERAL);

  // Debug support for non-terminals.
  BOOST_SPIRIT_DEBUG_RULE (specification);
  BOOST_SPIRIT_DEBUG_RULE (declaration);
  BOOST_SPIRIT_DEBUG_RULE (type_dcl);
  BOOST_SPIRIT_DEBUG_RULE (module);
  BOOST_SPIRIT_DEBUG_RULE (const_type);
  BOOST_SPIRIT_DEBUG_RULE (integer_type);
  BOOST_SPIRIT_DEBUG_RULE (signed_int);
  BOOST_SPIRIT_DEBUG_RULE (unsigned_int);
  BOOST_SPIRIT_DEBUG_RULE (signed_short_int);
  BOOST_SPIRIT_DEBUG_RULE (signed_long_int);
  BOOST_SPIRIT_DEBUG_RULE (signed_longlong_int);
  BOOST_SPIRIT_DEBUG_RULE (unsigned_short_int);
  BOOST_SPIRIT_DEBUG_RULE (unsigned_long_int);
  BOOST_SPIRIT_DEBUG_RULE (unsigned_longlong_int);
  BOOST_SPIRIT_DEBUG_RULE (char_type);
  BOOST_SPIRIT_DEBUG_RULE (wide_char_type);
  BOOST_SPIRIT_DEBUG_RULE (boolean_type);
  BOOST_SPIRIT_DEBUG_RULE (floating_pt_type);
  BOOST_SPIRIT_DEBUG_RULE (string_type);
  BOOST_SPIRIT_DEBUG_RULE (wide_string_type);
  BOOST_SPIRIT_DEBUG_RULE (fixed_pt_const_type);
  BOOST_SPIRIT_DEBUG_RULE (scoped_name);
  BOOST_SPIRIT_DEBUG_RULE (scoped_name_helper);
  BOOST_SPIRIT_DEBUG_RULE (octet_type);
  BOOST_SPIRIT_DEBUG_RULE (const_exp);
  BOOST_SPIRIT_DEBUG_RULE (or_expr);
  BOOST_SPIRIT_DEBUG_RULE (xor_expr);
  BOOST_SPIRIT_DEBUG_RULE (and_expr);
  BOOST_SPIRIT_DEBUG_RULE (shift_expr);
  BOOST_SPIRIT_DEBUG_RULE (add_expr);
  BOOST_SPIRIT_DEBUG_RULE (mult_expr);
  BOOST_SPIRIT_DEBUG_RULE (unary_expr);
  BOOST_SPIRIT_DEBUG_RULE (unary_operator);
  BOOST_SPIRIT_DEBUG_RULE (primary_expr);
  BOOST_SPIRIT_DEBUG_RULE (literal);
  BOOST_SPIRIT_DEBUG_RULE (positive_int_const);
  BOOST_SPIRIT_DEBUG_RULE (type_declarator);
  BOOST_SPIRIT_DEBUG_RULE (struct_type);
  BOOST_SPIRIT_DEBUG_RULE (union_type);
  BOOST_SPIRIT_DEBUG_RULE (enum_type);
  BOOST_SPIRIT_DEBUG_RULE (simple_declarator);
  BOOST_SPIRIT_DEBUG_RULE (constr_forward_decl);
  BOOST_SPIRIT_DEBUG_RULE (type_spec);
  BOOST_SPIRIT_DEBUG_RULE (declarators);
  BOOST_SPIRIT_DEBUG_RULE (simple_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (constr_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (base_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (template_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (any_type);
  BOOST_SPIRIT_DEBUG_RULE (object_type);
  BOOST_SPIRIT_DEBUG_RULE (value_base_type);
  BOOST_SPIRIT_DEBUG_RULE (fixed_pt_type);
  BOOST_SPIRIT_DEBUG_RULE (sequence_type);
  BOOST_SPIRIT_DEBUG_RULE (declarator);
  BOOST_SPIRIT_DEBUG_RULE (complex_declarator);
  BOOST_SPIRIT_DEBUG_RULE (array_declarator);
  BOOST_SPIRIT_DEBUG_RULE (member_list);
  BOOST_SPIRIT_DEBUG_RULE (member);
  BOOST_SPIRIT_DEBUG_RULE (switch_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (switch_body);
  BOOST_SPIRIT_DEBUG_RULE (case_decl);
  BOOST_SPIRIT_DEBUG_RULE (case_label);
  BOOST_SPIRIT_DEBUG_RULE (element_spec);
  BOOST_SPIRIT_DEBUG_RULE (enumerator);
  BOOST_SPIRIT_DEBUG_RULE (fixed_array_size);
  BOOST_SPIRIT_DEBUG_RULE (const_exp_helper);
  BOOST_SPIRIT_DEBUG_RULE (or_expr_helper);
  BOOST_SPIRIT_DEBUG_RULE (xor_expr_helper);
  BOOST_SPIRIT_DEBUG_RULE (and_expr_helper);
  BOOST_SPIRIT_DEBUG_RULE (shift_expr_helper);
  BOOST_SPIRIT_DEBUG_RULE (add_expr_helper);
  BOOST_SPIRIT_DEBUG_RULE (mult_expr_helper);
  BOOST_SPIRIT_DEBUG_RULE (except_dcl);
  BOOST_SPIRIT_DEBUG_RULE (interface);
  BOOST_SPIRIT_DEBUG_RULE (interface_dcl);
  BOOST_SPIRIT_DEBUG_RULE (forward_dcl);
  BOOST_SPIRIT_DEBUG_RULE (interface_header);
  BOOST_SPIRIT_DEBUG_RULE (interface_body);
  BOOST_SPIRIT_DEBUG_RULE (interface_name);
  BOOST_SPIRIT_DEBUG_RULE (interface_inheritance_spec);
  BOOST_SPIRIT_DEBUG_RULE (export);
  BOOST_SPIRIT_DEBUG_RULE (attr_dcl);
  BOOST_SPIRIT_DEBUG_RULE (op_dcl);
  BOOST_SPIRIT_DEBUG_RULE (param_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (op_attribute);
  BOOST_SPIRIT_DEBUG_RULE (op_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (parameter_dcls);
  BOOST_SPIRIT_DEBUG_RULE (raises_expr);
  BOOST_SPIRIT_DEBUG_RULE (context_expr);
  BOOST_SPIRIT_DEBUG_RULE (param_dcl);
  BOOST_SPIRIT_DEBUG_RULE (param_attribute);
  BOOST_SPIRIT_DEBUG_RULE (value);
  BOOST_SPIRIT_DEBUG_RULE (value_dcl);
  BOOST_SPIRIT_DEBUG_RULE (value_abs_dcl);
  BOOST_SPIRIT_DEBUG_RULE (value_box_dcl);
  BOOST_SPIRIT_DEBUG_RULE (value_forward_dcl);
  BOOST_SPIRIT_DEBUG_RULE (value_inheritance_spec);
  BOOST_SPIRIT_DEBUG_RULE (value_name);
  BOOST_SPIRIT_DEBUG_RULE (value_header);
  BOOST_SPIRIT_DEBUG_RULE (value_element);
  BOOST_SPIRIT_DEBUG_RULE (state_member);
  BOOST_SPIRIT_DEBUG_RULE (init_dcl);
  BOOST_SPIRIT_DEBUG_RULE (init_param_decls);
  BOOST_SPIRIT_DEBUG_RULE (init_param_decl);
  BOOST_SPIRIT_DEBUG_RULE (init_param_attribute);
  
  // Rules.
  
  // Parser start symbol.
  specification
    = +declaration
    ;

  declaration
    = type_dcl >> SEMI
    | const_dcl >> SEMI
    | except_dcl >> SEMI
    | interface >> SEMI
    | module >> SEMI
    | value >> SEMI
    ;
    
  module
    = IDL_MODULE >> IDENTIFIER 
      >> LBRACE >> +declaration >> RBRACE
    ;
    
  interface
    = longest_d
        [
          interface_dcl
          | forward_dcl
        ]
    ;
    
  interface_dcl
    = interface_header >> LBRACE >> interface_body >> RBRACE
    ;
    
  forward_dcl
    = !(IDL_ABSTRACT | IDL_LOCAL) >> IDL_INTERFACE >> IDENTIFIER
    ;
    
  interface_header
    = !(IDL_ABSTRACT | IDL_LOCAL) 
      >> IDL_INTERFACE >> IDENTIFIER
      >> !interface_inheritance_spec
    ;
    
  interface_body
    = *export
    ;
    
  export
    = type_dcl >> SEMI
    | const_dcl >> SEMI
    | except_dcl >> SEMI
    | attr_dcl >> SEMI
    | op_dcl >> SEMI
    ;

  interface_inheritance_spec
    = COLON >> interface_name >> *(COMMA >> interface_name)
    ;
    
  interface_name
    = scoped_name
    ;
     
  scoped_name
    = !SCOPE >> IDENTIFIER >> scoped_name_helper
    ;
    
  scoped_name_helper
    = SCOPE >> IDENTIFIER >> scoped_name_helper
    | epsilon_p
    ;
    
  value
    = longest_d
        [
          value_dcl
          | value_abs_dcl
          | value_box_dcl
          | value_forward_dcl
        ]
    ;
    
  value_forward_dcl
    = !IDL_ABSTRACT >> IDL_VALUETYPE >> IDENTIFIER
    ;
    
  value_box_dcl
    = IDL_VALUETYPE >> IDENTIFIER >> type_spec
    ;
    
  value_abs_dcl
    = IDL_ABSTRACT >> IDL_VALUETYPE >> IDENTIFIER
      >> !value_inheritance_spec >> LBRACE
      >> *export >> RBRACE
    ;
    
  value_dcl
    = value_header >> LBRACE >> *value_element >> RBRACE
    ;
    
  value_header
    = !IDL_CUSTOM >> IDL_VALUETYPE >> IDENTIFIER
      >> !value_inheritance_spec
    ;

  value_inheritance_spec
    = !(COLON >> !IDL_TRUNCATABLE >> value_name
        >> *(COMMA >> value_name))
      >> !(IDL_SUPPORTS >> interface_name
           >> *(COMMA >> interface_name ))
    ;
    
  value_name
    = scoped_name
    ;
      
  value_element
    = export
    | state_member
    | init_dcl
    ;
    
  state_member
    = (IDL_PUBLIC | IDL_PRIVATE) 
      >> type_spec >> declarators >> SEMI
    ;
    
  init_dcl
    = IDL_FACTORY >> IDENTIFIER >> LPAREN
      >> !init_param_decls >> RPAREN >> SEMI
    ;
    
  init_param_decls
    = init_param_decl >> *(COMMA >> init_param_decl)
    ;
    
  init_param_decl
    = init_param_attribute >> param_type_spec >> simple_declarator
    ;
    
  init_param_attribute
    = IDL_IN
    ;

  const_dcl
    = IDL_CONST >> const_type
      >> IDENTIFIER >> ASSIGN >> const_exp
    ;
    
  const_type
    = integer_type
    | char_type
    | wide_char_type
    | boolean_type
    | floating_pt_type
    | string_type
    | wide_string_type
    | fixed_pt_const_type
    | scoped_name
    | octet_type
    ;

  const_exp
    = or_expr
    ;
    
  or_expr
    = xor_expr >> or_expr_helper
    ;
    
  or_expr_helper
    = OR >> xor_expr
    | epsilon_p
    ;
       
  xor_expr
    = and_expr >> xor_expr_helper
    ;
    
  xor_expr_helper
    = XOR >> xor_expr
    | epsilon_p
    ;
    
  and_expr
    = shift_expr >> and_expr_helper
    ;
    
  and_expr_helper
    = AND >> and_expr
    | epsilon_p
    ;
    
  shift_expr
    = add_expr >> shift_expr_helper
    ;
    
  shift_expr_helper
    = LSHIFT >> shift_expr
    | RSHIFT >> shift_expr
    | epsilon_p
    ;
    
  add_expr
    = mult_expr >> add_expr_helper
    ;
    
  add_expr_helper
    = PLUS >> add_expr
    | MINUS >> add_expr
    | epsilon_p
    ;
    
  mult_expr
    = unary_expr >> mult_expr_helper
    ;
    
  mult_expr_helper
    = MULT >> mult_expr
    | DIV >> mult_expr
    | MOD >> mult_expr
    | epsilon_p
    ;
    
  unary_expr
    = !unary_operator >> primary_expr
    ;
    
  unary_operator
    = PLUS 
    | MINUS 
    | NOT
    ;
    
  primary_expr
    = longest_d
        [
          scoped_name
          | literal
          | LPAREN >> const_exp >> RPAREN
        ]
    ;
    
  literal
    = longest_d 
        [
          INTEGER_LITERAL
          | STRING_LITERAL
          | WIDE_STRING_LITERAL
          | CHARACTER_LITERAL
          | WIDE_CHARACTER_LITERAL
//            | FIXED_PT_LITERAL
          | FLOATING_PT_LITERAL
          | BOOLEAN_LITERAL
        ]
    ;
    
  positive_int_const
    = const_exp
    ;
    
  type_dcl
    = IDL_TYPEDEF >> type_declarator
    | struct_type
    | union_type
    | enum_type
    | IDL_NATIVE >> simple_declarator
    | constr_forward_decl
    ;
    
  type_declarator
    = type_spec >> declarators
    ;
    
  type_spec
    = simple_type_spec
    | constr_type_spec
    ;
    
  simple_type_spec
    = base_type_spec
    | template_type_spec
    | scoped_name
    ;
    
  base_type_spec
    = floating_pt_type
    | integer_type
    | char_type
    | wide_char_type
    | boolean_type
    | octet_type
    | any_type
    | object_type
    | value_base_type
    ;
    
  template_type_spec
    = sequence_type
    | string_type
    | wide_string_type
    | fixed_pt_type
    ;
    
  constr_type_spec
    = struct_type
    | union_type
    | enum_type
    ;
    
  declarators
    = declarator >> *(COMMA >> declarator)
    ;

  declarator
    = longest_d
        [
          simple_declarator
          | complex_declarator
        ]
    ;
    
  complex_declarator
    = array_declarator
    ;
            
  simple_declarator
    = IDENTIFIER
    ;
    
    
  floating_pt_type
    = IDL_FLOAT
    | IDL_DOUBLE
    | IDL_LONG >> IDL_DOUBLE
    ;
    
  integer_type
    = signed_int
    | unsigned_int
    ;
    
  signed_int
    = longest_d
        [
          signed_short_int
          | signed_long_int
          | signed_longlong_int
        ]
    ;
    
  unsigned_int
    = longest_d
        [
          unsigned_short_int
          | unsigned_long_int
          | unsigned_longlong_int
        ]
    ;
    
  signed_short_int
    = IDL_SHORT
    ;
    
  signed_long_int
    = IDL_LONG
    ;
    
  signed_longlong_int
    = IDL_LONG >> IDL_LONG
    ;
    
  unsigned_short_int
    = IDL_UNSIGNED >> IDL_SHORT
    ;
    
  unsigned_long_int
    = IDL_UNSIGNED >> IDL_LONG
    ;
    
  unsigned_longlong_int
    = IDL_UNSIGNED >> IDL_LONG >> IDL_LONG
    ;
    
  char_type
    = IDL_CHAR
    ;
    
  wide_char_type
    = IDL_WCHAR
    ;
    
  boolean_type
    = IDL_BOOLEAN
    ;
    
  octet_type
    = IDL_OCTET
    ;
    
  any_type
    = IDL_ANY
    ;
    
  object_type
    = IDL_OBJECT
    ;
    
  struct_type
    = IDL_STRUCT >> IDENTIFIER 
      >> LBRACE >> member_list >> RBRACE
    ;
    
  member_list
    = +member
    ;
    
  member
    = type_spec >> declarators >> SEMI
    ;
    
  union_type
    = IDL_UNION >> IDENTIFIER >> IDL_SWITCH
      >> LPAREN >> switch_type_spec >> RPAREN
      >> LBRACE >> switch_body >> RBRACE
    ;
    
  switch_type_spec
    = integer_type
    | char_type
    | boolean_type
    | enum_type
    | scoped_name
    ;
    
  switch_body
    = +case_decl
    ;
      
  case_decl
    = +case_label >> element_spec >> SEMI
    ;
    
  case_label
    = IDL_CASE >> const_exp >> COLON
    | IDL_DEFAULT >> COLON
    ;
    
  element_spec
    = type_spec >> declarator
    ;
    
  enum_type
    = IDL_ENUM >> IDENTIFIER 
      >> LBRACE >> enumerator 
      >> *(COMMA >> enumerator) >> RBRACE
    ;
    
  enumerator
    = IDENTIFIER
    ;
    
  sequence_type
    = IDL_SEQUENCE >> LANGLE >> simple_type_spec
      >> !(COMMA >> positive_int_const) >> RANGLE
    ;
    
  string_type
    = IDL_STRING >> !(LANGLE >> positive_int_const >> RANGLE)
    ;
    
  wide_string_type
    = IDL_WSTRING
    ;
    
  array_declarator
    = IDENTIFIER >> +fixed_array_size
    ;
    
  fixed_array_size
    = LBRACK >> positive_int_const >> RBRACK
    ;
    
  attr_dcl
    = !IDL_READONLY >> IDL_ATTRIBUTE >> param_type_spec
      >> simple_declarator >> *(COMMA >> simple_declarator)
    ;
    
  except_dcl
    = IDL_EXCEPTION >> IDENTIFIER 
      >> LBRACE >> *member >> RBRACE
    ;
    
  op_dcl
    = !op_attribute >> op_type_spec >> IDENTIFIER
      >> parameter_dcls >> !raises_expr >> !context_expr
    ;
    
  op_attribute
    = IDL_ONEWAY
    ;
    
  op_type_spec
    = param_type_spec
    | IDL_VOID
    ;
    
  parameter_dcls
    = LPAREN >> !(param_dcl >> *(COMMA >> param_dcl)) >> RPAREN
    ;
    
  param_dcl
    = param_attribute >> param_type_spec >> simple_declarator
    ;
    
  param_attribute
    = longest_d
        [
          IDL_IN
          | IDL_INOUT
        ]
    | IDL_OUT
    ;
    
  raises_expr
    = IDL_RAISES >> LPAREN >> scoped_name 
      >> *(COMMA >> scoped_name) 
      >> RPAREN
    ;
    
  context_expr
    = IDL_CONTEXT >> LPAREN >> STRING_LITERAL
      >> *(COMMA >> STRING_LITERAL)
      >> RPAREN
    ;
    
  param_type_spec
    = base_type_spec
    | string_type
    | wide_string_type
    | scoped_name
    ;
    
  fixed_pt_type
    = IDL_FIXED >> LANGLE 
      >> positive_int_const >> COMMA >> positive_int_const
      >> RANGLE
    ;
    
  fixed_pt_const_type
    = IDL_FIXED
    ;
    
  value_base_type
    = IDL_VALUEBASE
    ;
}

#endif /* IDL2_GRAMMAR_C */

// $Id$
//
// IDL2 Grammar checker implemented with Spirit (http://spirit.sourceforge.net/)
//
#ifndef IDL2_GRAMMAR_H
#define IDL2_GRAMMAR_H

#include <iostream>
#include <fstream>
#include <vector>

// This is caused by using 'this' in a base member initialization.
#pragma warning (disable: 4355)

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/symbols.hpp>

using namespace boost::spirit;
using namespace std;

// Global parsing utility function.
template<typename GrammarT>
void
parse (GrammarT const & g, 
       char const * filename)
{
  ifstream in (filename);

  if (!in)
    {
      cerr << "Could not open input file: " << filename << endl;
      return;
    }

  // Turn off white space skipping on the stream.
  in.unsetf (ios::skipws);

  vector<char> vec;
  std::copy (istream_iterator<char> (in),
             istream_iterator<char> (),
             std::back_inserter(vec));

  vector<char>::const_iterator start = vec.begin ();
  vector<char>::const_iterator end = vec.end ();

  skip_grammar skip;

  parse_info<vector<char>::const_iterator> result = parse (start, 
                                                           end, 
                                                           g, 
                                                           skip);

  if (result.full)
    {
      cerr << filename << " Parses OK" << endl;
    }
  else 
    {
      cerr << filename << " Fails Parsing" << endl;
      
      for (int i = 0; i < 50; i++)
        {
          if (result.stop == end)
            {
              break;
            }
            
          cerr << *result.stop++;
        }
        
      cerr << endl;
    }
}

// We use this to skip whitespace, comments, and whatever else.
struct skip_grammar : public grammar<skip_grammar>
{
  template <typename ScannerT>
  struct definition
  {
    definition (skip_grammar const & self);
    rule<ScannerT> const & start (void) const;
    
    rule<ScannerT> skip;
  };
};

struct idl2_grammar : public grammar<idl2_grammar>
{
  template <typename ScannerT>
  struct definition
  {
    definition (idl2_grammar const & /* self */);
    rule<ScannerT> const & start (void) const;

    // Keyword table.
    symbols<> keywords;

    // Non-alphanumeric terminals.
    strlit<>
      LSHIFT, RSHIFT, SCOPE;
    chlit<>
      SEMI, COMMA, COLON, ASSIGN, MINUS, PLUS, MULT, DIV, MOD,
      AND, NOT, OR, XOR, LPAREN, RPAREN, LANGLE, RANGLE,
      LBRACK, RBRACK, LBRACE, RBRACE;

    // IDL2 keywords.
    rule<ScannerT>
      IDL_ABSTRACT, IDL_ANY, IDL_ATTRIBUTE, IDL_BOOLEAN, 
      IDL_CASE, IDL_CHAR, IDL_CONST, IDL_CONTEXT, IDL_CUSTOM, 
      IDL_DEFAULT, IDL_DOUBLE, IDL_ENUM, IDL_EXCEPTION,
      IDL_FACTORY, IDL_FALSE, IDL_FIXED, IDL_FLOAT, 
      IDL_IN, IDL_INOUT, IDL_INTERFACE, IDL_LOCAL, IDL_LONG, 
      IDL_MODULE, IDL_NATIVE, IDL_OBJECT, IDL_OCTET,
      IDL_ONEWAY, IDL_OUT, IDL_PRIVATE, IDL_PUBLIC,
      IDL_RAISES, IDL_READONLY, IDL_SHORT, IDL_SEQUENCE, 
      IDL_STRING, IDL_STRUCT, IDL_SUPPORTS, IDL_SWITCH, 
      IDL_TRUE, IDL_TRUNCATABLE, IDL_TYPEDEF, IDL_UNION,
      IDL_UNSIGNED, IDL_VALUEBASE, IDL_VALUETYPE, IDL_VOID, 
      IDL_WCHAR, IDL_WSTRING;
    
    // Identifier and literals.
    rule<ScannerT> 
      IDENTIFIER, 
      CHARACTER_LITERAL, WIDE_CHARACTER_LITERAL, STRING_LITERAL_PART,
      STRING_LITERAL, WIDE_STRING_LITERAL_PART, WIDE_STRING_LITERAL,
      INTEGER_LITERAL_HEX, INTEGER_LITERAL_OCT, INTEGER_LITERAL_DEC, 
      INTEGER_LITERAL, FLOATING_PT_LITERAL_1, FLOATING_PT_LITERAL_2, 
      FLOATING_PT_LITERAL_3, FLOATING_PT_LITERAL, BOOLEAN_LITERAL;

    // IDL2 non-terminals.
    rule<ScannerT>
      specification, declaration, const_dcl, module, const_type, type_dcl,
      integer_type, signed_int, unsigned_int, signed_short_int,
      signed_long_int, signed_longlong_int, unsigned_short_int,
      unsigned_long_int, unsigned_longlong_int,
      char_type, wide_char_type, boolean_type, floating_pt_type, 
      string_type, wide_string_type, fixed_pt_const_type, scoped_name, 
      scoped_name_helper, octet_type, const_exp, or_expr, xor_expr,
      and_expr, shift_expr, add_expr, mult_expr, unary_expr, 
      unary_operator, primary_expr, literal, fixed_pt_literal,
      positive_int_const, type_declarator, struct_type, union_type,
      enum_type, simple_declarator, constr_forward_decl, type_spec,
      declarators, simple_type_spec, constr_type_spec, base_type_spec,
      template_type_spec, any_type, object_type, value_base_type,
      fixed_pt_type, sequence_type, declarator, complex_declarator,
      array_declarator, member_list, member, switch_type_spec,
      switch_body, case_decl, case_label, element_spec, enumerator,
      fixed_array_size, const_exp_helper, or_expr_helper, 
      xor_expr_helper, and_expr_helper, shift_expr_helper, add_expr_helper, 
      mult_expr_helper, except_dcl, interface, interface_dcl,
      forward_dcl, interface_header, interface_body, interface_name,
      interface_inheritance_spec, export, attr_dcl, op_dcl, 
      param_type_spec, op_attribute, op_type_spec, parameter_dcls,
      raises_expr, context_expr, param_dcl, param_attribute, value,
      value_dcl, value_abs_dcl, value_box_dcl, value_forward_dcl,
      value_inheritance_spec, value_name, value_header, value_element,
      state_member, init_dcl, init_param_decls, init_param_decl,
      init_param_attribute;
  };
};

#include "idl2_grammar.cpp"
  
#endif /* IDL2_GRAMMAR_H */

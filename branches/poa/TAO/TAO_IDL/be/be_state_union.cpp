// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_union.cpp
//
// = DESCRIPTION
//    state based code generation for unions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

be_state_union_disctypedefn_ch::be_state_union_disctypedefn_ch (void)
{
}

// generate code for union discriminant type in client header
int
be_state_union_disctypedefn_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union *bu;

  bu = be_union::narrow_from_decl (d); // downcast to union type
  if (!bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_disctypedefn_ch::"
                         "gen_code - "
                         "bad union node\n"), -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  os = cg->client_header (); // get client header stream

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
    case AST_Decl::NT_except: // type is an exception
      // all these cases are syntax errors which the front-end should have
      // flagged as error cases
      return -1;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
      {
        // if the discriminant is an enum, first generate the enum
        // definition. However, check that we are not inside a recursive call
        if (bt->node_type () == AST_Decl::NT_enum)
          if (bt->gen_client_header () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_disctypedefn_ch::"
                                 "gen_code - "
                                 "codegen for discrim failed\n"), -1);
            }

        os->indent ();
        // the set method
        *os << "void _d (" << bt->nested_type_name (bu) << ");" << nl;
        // the get method
        *os << bt->nested_type_name (bu) << " _d (void) const;\n\n";
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        // a scoped name must result in one of the allowed types. Hopefully the
        // front-end has done a good job of ensuring this.
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);
        if (!t)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_state_union_disctypedefn_ch::"
                               "gen_code - "
                               "bad typedef node\n"), -1);
          }

        temp = t->primitive_base_type ();
        return this->gen_code (t, d, temp);
      }
    } // end of switch
  return 0;
}

be_state_union_disctypedefn_ci::be_state_union_disctypedefn_ci (void)
{
}

// generate code for union discriminant type in client inline
int
be_state_union_disctypedefn_ci::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union *bu;

  bu = be_union::narrow_from_decl (d); // downcast to union type
  if (!bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_disctypedefn_ci::"
                         "gen_code - "
                         "bad union node\n"), -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  os = cg->client_inline (); // get client inline stream

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
    case AST_Decl::NT_except: // type is an exception
      // all these cases are syntax errors which the front-end should have
      // flagged as error
      return -1;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_typedef: // type is a typedef
      {
        os->indent ();
        // the set method
        *os << "// accessor to set the discriminant" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::_d (" << bt->name () <<
          " discval)" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "this->disc_ = discval;\n";
        os->decr_indent ();
        *os << "}" << nl;
        // the get method
        *os << "// accessor to get the discriminant" << nl;
        *os << "ACE_INLINE " << bt->name () << nl;
        *os << bu->name () << "::_d (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->disc_;\n";
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    } // end of switch
  return 0;
}

be_state_union_public_ch::be_state_union_public_ch (void)
{
}

// generate code for union branch members in client header.  This involves
// generating the set/get methods corresponding to the members
int
be_state_union_public_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;      // enclosing union

  os = cg->client_header (); // get client header stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_public_ch::"
                         "gen_code - "
                         "bad union member\n"), -1);
    }

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_public_ch::"
                         "gen_code - "
                         "bad union node\n"), -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // codegen based on node type and the state we are in
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        os->indent (); // start from current indentation
        *os << "void " << ub->local_name () << " (" << bt->nested_type_name
          (bu, "_ptr") << ");// set" << nl;
        *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name () <<
          " (void) const; // get method\n\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);
        if (!bpd)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_state_union_public_ch::"
                               "gen_code - "
                               "bad predefined type\n"), -1);
          }

        if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            os->indent (); // start from current indentation
            *os << "void " << ub->local_name () << " (" << bt->nested_type_name
              (bu) << "_ptr);// set" << nl;
            *os << bt->nested_type_name (bu) << "_ptr " << ub->local_name () <<
              " (void) const; // get method\n\n";
          }
        else
          {
            os->indent (); // start from current indentation
            *os << "void " << ub->local_name () << " (" << bt->nested_type_name
              (bu) << ");// set" << nl;
            *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
              " (void) const; // get method\n\n";
          }
      }
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        // XXXASG - TODO - what if we have a pseudo obj?
        // if the type is an enum, we generate its defn first
        if (bt->node_type () == AST_Decl::NT_enum)
          if (bt->gen_client_header () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_public_ch::"
                                 "gen_code - "
                                 "bad union member\n"), -1);
            }

        os->indent (); // start from current indentation
        *os << "void " << ub->local_name () << " (" << bt->nested_type_name (bu)
          << ");// set" << nl;
        *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
          " (void) const; // get method\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            // three methods to set the string value
            *os << "void " << ub->local_name () << " (" << bt->nested_type_name
              (bu) << "); // set" << nl;
            *os << "void " << ub->local_name () << " (const " <<
              bt->nested_type_name (bu) << "); // set"
                << nl;
            *os << "void " << ub->local_name () <<
              " (const " << bt->nested_type_name (bu, "_var") << " &); // set"  <<
              nl;
            *os << "const " << bt->nested_type_name (bu) << " " <<
              ub->local_name () << " (void) const; // get method\n\n";
          }
        else
          {
            // three methods to set the string value
            *os << "void " << ub->local_name () << " (char *); // set" << nl;
            *os << "void " << ub->local_name () << " (const char *); // set"
                << nl;
            *os << "void " << ub->local_name () <<
              " (const CORBA::String_var&); // set"  << nl;
            *os << "const char *" << ub->local_name () <<
              " (void) const; // get method\n\n";
          }
      }
      break;
    case AST_Decl::NT_array: // type is an array
      {
        // generate code for the array. So let the array handle code
        // generation. Check if this is not a recursive call
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_public_ch::"
                                 "gen_code - "
                                 "codegen for array failed\n"), -1);
            }

        os->indent ();
        *os << "void " << ub->local_name () << " (" << bt->nested_type_name
          (bu) << ");// set" << nl;
        *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name () <<
          " (void) const; // get method\n\n";

      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        // generate defn for this aggregate unless we are recursively called
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_public_ch::"
                                 "gen_code - "
                                 "codegen for type failed\n"), -1);
            }

        os->indent ();
        *os << "void " << ub->local_name () << " (const " <<
          bt->nested_type_name (bu) << " &);// set" << nl;
        *os << "const " << bt->nested_type_name (bu) << " &" << ub->local_name
          () << " (void) const; // get method (read only)" << nl;
        *os << bt->nested_type_name (bu) << " &" << ub->local_name () <<
          " (void); // get method (read/write only)\n\n";
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, ub, temp);
      }
    } // end of switch

  // enclosing union is variable if the member is variable
  bu->size_type (type->size_type ());

  return 0;
}

be_state_union_public_ci::be_state_union_public_ci (void)
{
}

// generate code for union branch members in client inline.  This involves
// generating the set/get methods corresponding to the members
int
be_state_union_public_ci::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os;   // output stream
  TAO_NL  nl;          // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;        // enclosing union

  os = cg->client_inline (); // get client inline stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_public_ci::"
                         "gen_code - "
                         "bad union member\n"), -1);
    }

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_public_ci::"
                         "gen_code - "
                         "bad union node\n"), -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  switch (type->node_type ())
    {
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_interface: // type is an obj reference
      {
        // for array, generate inline methods  if any
        // We first need to generate code for this aggregate type. Check
        // if we are not called recursively thru a typedef
        if (bt->node_type () == AST_Decl::NT_array
            && !bt->imported ())
          if (bt->gen_client_inline () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_public_ci::"
                                 "gen_code - "
                                 "codegen for array failed\n"), -1);
            }

        // set method
        os->indent (); // start from current indentation
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (" << bt->name ();
        if (type->node_type () == AST_Decl::NT_interface)
          {
            // if it is an interface node, we use _ptr as a suffix for the type
            *os << "_ptr";
          }
        *os << " val)// set" << nl;
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            if (type->node_type () == AST_Decl::NT_interface)
              {
                // the private data member is a pointer type
                *os << "*this->" << ub->local_name () << "_ = val;\n";
              }
            else
              {
                *os << "this->" << ub->local_name () << "_ = val;\n";
              }
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // get method
        *os << "// retrieve the member" << nl;
        *os << "ACE_INLINE " << bt->name ();
        if (type->node_type () == AST_Decl::NT_interface)
          {
            // for interface type, return a _ptr type
            *os << "_ptr";
          }
        else if (type->node_type () == AST_Decl::NT_array)
          {
            // for an array, return a _slice* type
            *os << "_slice *";
          }
        *os << nl;
        *os << bu->name () << "::" << ub->local_name () <<
          " (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        if (type->node_type () == AST_Decl::NT_interface)
          {
            *os << "return *this->" << ub->local_name () << "_;\n";
          }
        else
          {
            *os << "return this->" << ub->local_name () << "_;\n";
          }
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        // three methods to set the string value

        // (1) set method from char*
        os->indent (); // start from current indentation
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bu->name () << "::" << ub->local_name () << " (" << bt->name
              () << " val)" << nl;
          }
        else
          {
            *os << bu->name () << "::" << ub->local_name () << " (char *val)"
                << nl;
          }
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "if (!this->" << ub->local_name () << "_) // does not exist"
                << nl ;
            *os << "\tthis->" << ub->local_name () <<
              "_ = new CORBA::String_var (val);" << nl;
            *os << "else" << nl;
            *os << "\t*this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // (2) set method from const char *
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bu->name () << "::" << ub->local_name () << " (const " <<
              bt->name () << " val)" << nl;
          }
        else
          {
            *os << bu->name () << "::" << ub->local_name () <<
              " (const char *val)" << nl;
          }
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "if (!this->" << ub->local_name () << "_) // does not exist"
                << nl ;
            *os << "\tthis->" << ub->local_name () <<
              "_ = new CORBA::String_var (val);" << nl;
            *os << "else" << nl;
            *os << "\t*this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // (3) set from const String_var&
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bu->name () << "::" << ub->local_name () << " (const " <<
              bt->name () << "_var &val)" << nl;
          }
        else
          {
            *os << bu->name () << "::" << ub->local_name () <<
              " (const CORBA::String_var &val)" << nl;
          }
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "if (!this->" << ub->local_name () << "_) // does not exist"
                << nl ;
            *os << "\tthis->" << ub->local_name () <<
              "_ = new CORBA::String_var (val);" << nl;
            *os << "else" << nl;
            *os << "\t*this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << "ACE_INLINE const " << bt->name () << nl;
          }
        else
          {
            *os << "ACE_INLINE const char *" << nl;
          }
        *os << bu->name () << "::" << ub->local_name () <<
          " (void) const // get method" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return *this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_union: // type is a union
      {
        // We first need to generate code for this aggregate type. Check
        // if we are not called recursively thru a typedef
        if (bt->node_type () != AST_Decl::NT_typedef
            && !bt->imported ())
          if (bt->gen_client_inline () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_public_ci::"
                                 "gen_code - "
                                 "codegen for type failed\n"), -1);
            }

        os->indent ();
        // This case has to be handled differently from structs because the data
        // member is a pointer

        // (1) set from a const
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () <<
          " (const " << bt->name () << " &val)" << nl;
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "*this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // readonly get method
        *os << "// readonly get method " << nl;
        *os << "ACE_INLINE const " << bt->name () << " &" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return *this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}" << nl;

        // read/write get method
        *os << "// read/write get method " << nl;
        *os << "ACE_INLINE " << bt->name () << " &" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (void)" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return *this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}" << nl;

      }
      break;
    case AST_Decl::NT_struct: // type is a struct
      {
        // We first need to generate code for this aggregate type. Check
        // if we are not called recursively thru a typedef
        if (bt->node_type () != AST_Decl::NT_typedef
            && !bt->imported ())
          if (bt->gen_client_inline () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_union_public_ci::"
                                 "gen_code - "
                                 "codegen for struct failed\n"), -1);
            }

        os->indent ();

        // (1) set from a const
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () <<
          " (const " << bt->name () << " &val)" << nl;
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // readonly get method
        *os << "// readonly get method " << nl;
        *os << "ACE_INLINE const " << bt->name () << " &" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}" << nl;

        // read/write get method
        *os << "// read/write get method " << nl;
        *os << "ACE_INLINE " << bt->name () << " &" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (void)" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}" << nl;

      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, ub, temp);
      }
    } // end of switch

  return 0;
}

be_state_union_public_cs::be_state_union_public_cs (void)
{
}

// generate code for union branch members in client header.  This involves
// generating the set/get methods corresponding to the members
int
be_state_union_public_cs::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;      // enclosing union

  os = cg->client_stubs (); // get client stubs stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_public_cs::"
                         "gen_code - "
                         "bad union member\n"), -1);
    }

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (!bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_public_cs::"
                         "gen_code - "
                         "bad union member\n"), -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      {
        switch (type->node_type ())
          {
          case AST_Decl::NT_interface: // type is an obj reference
          case AST_Decl::NT_pre_defined: // type is predefined type
          case AST_Decl::NT_string: // type is a string
            {
              // nothing
            }
            break;
          case AST_Decl::NT_enum: // type is an enum
            {
              if (bt->node_type () == AST_Decl::NT_enum)
                if (bt->gen_client_stubs () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_union_public_cs::"
                                       "gen_code - "
                                       "codegen for type failed\n"), -1);
                  }
            }
            break;
          case AST_Decl::NT_array: // type is an array
            {
              // generate code for the array. So let the array handle code
              // generation. Check if this is not a recursive call
              if (bt->node_type () != AST_Decl::NT_typedef)
                if (bt->gen_client_stubs () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_union_public_cs::"
                                       "gen_code - "
                                       "codegen for type failed\n"), -1);
                  }

            }
            break;
          case AST_Decl::NT_sequence: // type is a sequence
          case AST_Decl::NT_struct: // type is a struct
          case AST_Decl::NT_union: // type is a union
            {
              // generate defn for this aggregate unless we are recursively called
              if (bt->node_type () != AST_Decl::NT_typedef)
                if (bt->gen_client_stubs () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_union_public_cs::"
                                       "gen_code - "
                                       "codegen for type failed\n"), -1);
                  }
            }
            break;
          case AST_Decl::NT_except: // type is an exception
            {
              // XXXASG: Is this case valid ???
            }
            break;
          case AST_Decl::NT_typedef: // type is a typedef
            {
              be_type *temp; // most primitive base type
              be_typedef *t = be_typedef::narrow_from_decl (bt);

              if (!t)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_union_private_cs::"
                                     "gen_code - "
                                     "bad typedef\n"), -1);
                }

              temp = t->primitive_base_type ();
              // make a recursive call
              return this->gen_code (t, ub, temp);
            }
          } // end of switch node type
      }
      break;
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
      {
        // This state is used when we are generating the copy ctor and
        // assignment operator for the union.
        // Individual assignment of the members takes place inside a case
        // statement because the type of member assigned is based on the value
        // of the discriminant
        os->indent ();
        if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
          {
            *os << "case " << ub->label ()->label_val ()->n ()  << ":\n";
          }
        else
          {
            *os << "case " << ub->label ()->label_val () << ":\n";
          }
        os->incr_indent ();
        *os << "this->" << ub->local_name () << "_ = u." << ub->local_name ()
            << "_;" << nl;
        *os << "break;\n";
        os->decr_indent (0);
      }
      break;
    }

  return 0;
}

be_state_union_private_ch::be_state_union_private_ch (void)
{
}

int
be_state_union_private_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;      // enclosing union

  os = cg->client_header (); // get client header stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_private_ch::"
                         "gen_code - "
                         "bad union member\n"), -1);
    }

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (!bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_union_private_ch::"
                         "gen_code - "
                         "bad union\n"), -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        os->indent (); // start from current indentation
        // C++ does not allow an object declaration inside a union. Hence we
        // must have a pointer. This changes some of the methods
        *os << bt->nested_type_name (bu, "_var") << " *" << ub->local_name () <<
          "_;\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);
        if (!bpd)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_state_union_private_ch::"
                               "gen_code - "
                               "bad predefined type\n"), -1);
          }

        if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            os->indent (); // start from current indentation
            *os << bt->nested_type_name (bu) << "_var " << ub->local_name () <<
              "_;\n";
          }
        else
          {
            os->indent (); // start from current indentation
            *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
              "_;\n";
          }
      }
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
          "_;\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        // C++ does not allow an object declaration inside a union. Hence we
        // must have a pointer. This changes some of the methods
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bt->nested_type_name (bu, "_var") << " *" << ub->local_name () <<
              "_;\n";
          }
        else
          {
            *os << "CORBA::String_var *" << ub->local_name () << "_;\n";
          }
      }
      break;
    case AST_Decl::NT_array: // type is an array
      {
        os->indent ();
        *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name () <<
          "_;\n";
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_union: // type is a union
      {
        os->indent ();
        // C++ doesn't allow object instances inside unions, so we need a
        // pointer
        *os << bt->nested_type_name (bu) << " *" << ub->local_name () << "_;\n";
      }
      break;
    case AST_Decl::NT_struct: // type is a struct
      {
        os->indent ();
        *os << bt->nested_type_name (bu) << " " << ub->local_name () << "_;\n";
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, ub, temp);
      }
    } // end of switch
  return 0;
}

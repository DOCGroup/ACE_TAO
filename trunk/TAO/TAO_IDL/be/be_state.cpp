// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_state.cpp
//
// = DESCRIPTION
//    state based code generation.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// constructor
be_state::be_state (void)
{
}

// destructor
be_state::~be_state (void)
{
}

// ==== all subclasses of be_state ======

be_state_struct_ch::be_state_struct_ch (void)
{
}

// generate code for structure member
int
be_state_struct_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_field *f;       // field node
  be_structure *bs;  // enclosing structure node

  os = cg->client_header (); // get client header stream
  f = be_field::narrow_from_decl (d); // downcast to field node
  if (!f)
    return -1;

  bs = be_structure::narrow_from_scope (f->defined_in ());
  if (bs == NULL)
    return -1;

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
        *os << bt->name () << "_var " << f->local_name () << ";\n\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        os->indent (); // start from current indentation
        *os << bt->name () << " " << f->local_name () << ";\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bt->name () << "_var " << f->local_name () << ";\n\n";
          }
        else
          {
            *os << "CORBA::String_var " << f->local_name () << ";\n\n";
          }
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        // We first need to generate code for this aggregate type. Check if we
        // are not called recursively thru a typedef
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent (); 
        *os << bt->name () << " " << f->local_name () << ";\n\n";
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, f, temp);
      } // end of switch
      break;
    }
  // the enclosing structure will be variable length the field is variable
  // length
  bs->size_type (type->size_type ());
  return 0;
}

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
            return -1;

        os->indent ();
        // the set method
        *os << "void _d (" << bt->name () << ");" << nl;
        // the get method
        *os << bt->name () << " _d (void) const;\n\n";
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        // a scoped name must result in one of the allowed types. Hopefully the
        // front-end has done a good job of ensuring this.
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);
        if (!t)
          return -1;
        temp = t->primitive_base_type ();
        return this->gen_code (t, d, temp);
      }
      break;
    } // end of switch
  return 0;
}

be_state_union_disctypedefn_ci::be_state_union_disctypedefn_ci (void)
{
}

// generate code for union discriminant type in client inline
int
be_state_union_disctypedefn_ci::gen_code (be_type *t, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union *bu;

  bu = be_union::narrow_from_decl (d); // downcast to union type
  if (!bu)
    return -1;

  os = cg->client_inline (); // get client inline stream

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (t->node_type ())
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
        *os << bu->name () << "::_d (" << t->name () << " discval)" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "this->disc_ = discval;\n";
        os->decr_indent ();
        *os << "}" << nl;
        // the get method
        *os << "// accessor to get the discriminant" << nl;
        *os << "ACE_INLINE " << t->name () << nl;
        *os << bu->name () << " _d (void) const" << nl;
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
    return -1;

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    return -1;

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
        *os << "void " << ub->local_name () << " (" << bt->name () << 
          "_ptr);// set" << nl;
        *os << bt->name () << "_ptr " << ub->local_name () << 
          " (void) const; // get method\n\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
      {
        // if the type is an enum, we generate its defn first
        if (bt->node_type () == AST_Decl::NT_enum)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent (); // start from current indentation
        *os << "void " << ub->local_name () << " (" << bt->name () << 
          ");// set" << nl;
        *os << bt->name () << " " << ub->local_name () << 
          " (void) const; // get method\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        // three methods to set the string value
        *os << "void " << ub->local_name () << " (char *); // set" << nl;
        *os << "void " << ub->local_name () << " (const char *); // set" 
            << nl; 
        *os << "void " << ub->local_name () << 
          " (const CORBA::String_var&); // set"  << nl;
        *os << "const char *" << ub->local_name () << 
          " (void) const; // get method\n\n";

      }
      break;
    case AST_Decl::NT_array: // type is an array
      {
        // generate code for the array. So let the array handle code
        // generation. Check if this is not a recursive call
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent ();
        *os << "void " << ub->local_name () << " (" << bt->name () << 
          ");// set" << nl;
        *os << bt->name () << "_slice *" << ub->local_name () << 
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
            return -1;
        os->indent ();
        *os << "void " << ub->local_name () << " (const " << bt->name () << 
          " &);// set" << nl;
        *os << "const " << bt->name () << " &" << ub->local_name () << 
          " (void) const; // get method (read only)" << nl;
        *os << bt->name () << " &" << ub->local_name () << 
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
      break;
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
    return -1;

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    return -1;

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
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_array: // type is an array
      {
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
            *os << "this->" << ub->local_name () << "_ = val;\n";
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
        *os << "return this->" << ub->local_name () << "_;\n";
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
        *os << bu->name () << "::" << ub->local_name () << " (char *val)" << nl;
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

        // (2) set method from const char *
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (const char *val)" <<
          nl; 
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

        // (3) set from const String_var&
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () << 
          " (char CORBA::String_var &val)" << nl;
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

        // return const char*
        *os << "ACE_INLINE const char *" << nl;
        *os << bu->name () << "::" << ub->local_name () << 
          " (void) const // get method" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
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
      break;
    } // end of switch

  return 0;
}

be_state_union_private_ch::be_state_union_private_ch (void)
{
}

int
be_state_union_private_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;      // enclosing union

  os = cg->client_header (); // get client header stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    return -1;

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    return -1;

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
        *os << bt->name () << "_var " << ub->local_name () << 
          "_;\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
      {
        os->indent (); // start from current indentation
        *os << bt->name () << " " << ub->local_name () << 
          "_;\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        *os << "CORBA::String_var " << ub->local_name () << 
          "_;\n";
      }
      break;
    case AST_Decl::NT_array: // type is an array
      {
        os->indent ();
        *os << bt->name () << "_slice *" << ub->local_name () << 
          "_;\n";
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        os->indent ();
        *os << bt->name () << " " << ub->local_name () << 
          "_;\n";
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
      break;
    } // end of switch
  return 0;
}

// return type for operation
be_state_operation::be_state_operation (void)
{
}

// generate code for return type of operation
int
be_state_operation::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_OPERATION_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_OPERATION_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_OPERATION_SS:
      os = cg->server_skeletons ();
      break;
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
        *os << bt->name () << "_ptr ";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (bt);

        *os << bt->name ();
        // check if the type is an any
        if (bpd->pt () == AST_PredefinedType::PT_any)
          {
            // if it is an any, return a pointer to it
            *os << " *";
          }
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            // pseudo object, return a pointer
            *os << "_ptr";
          }
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        *os << "char *";
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
      // return a pointer to slice
      *os << bt->name () << "_slice *";
      break;
    case AST_Decl::NT_sequence: // type is a sequence
      // return type is a pointer to sequence
      *os << bt->name () << " *";
      break;
    case AST_Decl::NT_enum: // type is an enum
      *os << bt->name ();
      break;
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        *os << bt->name ();
        // check if we are fixed size or variable sized. Depending on that we
        // return a pointer or the aggregate itself
        if (type->size_type () == be_decl::VARIABLE)
          *os << " *";
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      break;
    }
  return 0;
}

// return type for argument
be_state_argument::be_state_argument (void)
{
}

// generate code for return type of argument
int
be_state_argument::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_argument *arg;  // argument node

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ARGUMENT_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SS:
      os = cg->server_skeletons ();
      break;
    }

  arg = be_argument::narrow_from_decl (d);

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // find the direction of the argument. Depending on the direction and the
  // type, generate the mapping
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            *os << bt->name () << "_ptr ";
            break;
          case AST_Argument::dir_INOUT:
            *os << bt->name () << "_ptr &";
            break;
          case AST_Argument::dir_OUT:
            *os << bt->name () << "_ptr &";
            break;
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (bt);

        // check if the type is an any
        if (bpd->pt () == AST_PredefinedType::PT_any)
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << "const " << bt->name () << " &";
                break;
              case AST_Argument::dir_INOUT:
                *os << bt->name () << " &";
                break;
              case AST_Argument::dir_OUT:
                *os << bt->name () << "_out ";
                break;
              }
          }
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << bt->name () << "_ptr ";
                break;
              case AST_Argument::dir_INOUT:
                *os << bt->name () << "_ptr &";
                break;
              case AST_Argument::dir_OUT:
                *os << bt->name () << "_ptr &";
                break;
              }
          }
        else
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << bt->name ();
                break;
              case AST_Argument::dir_INOUT:
                *os << bt->name () << " &";
                break;
              case AST_Argument::dir_OUT:
                *os << bt->name () << "_out";
                break;
              }
          }
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            *os << "const char *";
            break;
          case AST_Argument::dir_INOUT:
            *os << "char *&";
            break;
          case AST_Argument::dir_OUT:
            *os << bt->name () << "_out ";
            break;
          }
      }
      break;
    case AST_Decl::NT_array: // type is an array
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const " << bt->name ();
          break;
        case AST_Argument::dir_INOUT:
          *os << bt->name ();
          break;
        case AST_Argument::dir_OUT:
          *os << bt->name () << "_out";
          break;
        }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const " << bt->name () << " &";
          break;
        case AST_Argument::dir_INOUT:
          *os << bt->name () << " &";
          break;
        case AST_Argument::dir_OUT:
          *os << bt->name () << "_out";
          break;
        }
      break;
    case AST_Decl::NT_enum: // type is an enum
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            *os << bt->name ();
            break;
          case AST_Argument::dir_INOUT:
            *os << bt->name () << " &";
            break;
          case AST_Argument::dir_OUT:
            *os << bt->name () << "_out";
            break;
          }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      break;
    }
  return 0;
}

// return type for typedef
be_state_typedef::be_state_typedef (void)
{
}

// generate code for structure member
int
be_state_typedef::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_typedef *tdef;  // typedef node

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        os = cg->client_header (); // set the stream to be the client header
        tdef = be_typedef::narrow_from_decl (d); // downcast to typedef node
        if (!tdef)
          return -1;

        if (!type) // not a recursive call
          type = bt;
        else // recursively called thru a typedef. "type" will have the most primitive
          // base class of the typedef
          ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

        switch (type->node_type ())
          {
          case AST_Decl::NT_interface: // type is an obj reference
            {
              os->indent (); // start from current indentation
              // typedef of an interface results in 3 typedefs as shown below
              *os << "typedef " << bt->name () << " " << d->local_name () << ";" << nl;
              *os << "typedef " << bt->name () << "_ptr " << d->local_name () << "_ptr;" << nl;
              *os << "typedef " << bt->name () << "_var " << d->local_name () << "_var;" << nl;
              *os << "typedef " << bt->name () << "_out " << d->local_name () << "_out;\n\n";
            }
            break;
          case AST_Decl::NT_pre_defined: // type is predefined type
            {
              be_predefined_type *pd = be_predefined_type::narrow_from_decl (bt);

              if (!pd)
                return -1;
              os->indent (); // start from current indentation
              *os << "typedef " << bt->name () << " " << d->local_name () << ";";
              // if the predefined type is an ANY, we also define a typedef to _var
              if (pd->pt () == AST_PredefinedType::PT_any)
                {
                  *os << nl;
                  *os << "typedef " << bt->name () << "_var " << d->local_name () << "_var;";
                }
              *os << "typedef " << bt->name () << "_out " << d->local_name () << "_out;\n\n";
            }
            break;
          case AST_Decl::NT_string: // type is a string
            {
              os->indent (); // start from current indentation
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << "typedef " << bt->name () << " " << d->local_name () << nl;
                  *os << "typedef " << bt->name () << "_var " << d->local_name
                    () << "_var;\n\n"; 
                }
              else
                {
                  *os << "typedef CORBA::String " << d->local_name () << nl;
                  *os << "typedef CORBA::String_var " << d->local_name
                    () << "_var;\n\n"; 
                }
            }
            break;
          case AST_Decl::NT_enum: // type is an enum
            {
              os->indent (); // start from current indentation
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    return -1;
                }
              *os << "typedef " << bt->name () << " " << d->local_name () << ";" << nl;
              *os << "typedef " << bt->name () << "_out " << d->local_name () << "_out;\n\n";
            }
            break;
            // these are all anonymous types
          case AST_Decl::NT_array: // type is an array
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    return -1;
                }
              os->indent ();
              *os << "typedef " << bt->name () << " " << d->local_name () <<
                ";" << nl;
              *os << "typedef " << bt->name () << "_forany " << d->local_name () <<
                "_forany;" << nl; 
              // typedefs for the auxiliary methods. If we are nested inside
              // some scope, these methods become 
              if (d->is_nested ())
                *os << "static ";
              *os << d->name () << "_slice* " << d->local_name () << 
                "_alloc (void);" << nl; 
              if (d->is_nested ())
                *os << "static ";
              *os << d->name () << "_slice* " << d->local_name () << "_dup (" 
                  << d->name () << "_slice* " << ");" << nl;
              if (d->is_nested ())
                *os << "static ";
              *os << "void " << d->name () << "_free (" << d->name () <<
                "_slice *);\n\n";
            }
            break;
          case AST_Decl::NT_sequence: // type is a sequence
          case AST_Decl::NT_struct:   // type is a struct
          case AST_Decl::NT_union:    // type is a union
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    return -1;
                }
              os->indent (); // start from current indentation
              *os << "typedef " << bt->name () << " " << d->local_name () << ";" << nl;
              *os << "typedef " << bt->name () << "_var " << d->local_name () << "_var;" << nl;
              *os << "typedef " << bt->name () << "_out " << d->local_name () << "_out;\n\n";
            }
            break;
          case AST_Decl::NT_except: // type is an exception
            {
              // XXXASG TODO: is this allowed ???
            }
            break;
          case AST_Decl::NT_typedef: // type is a typedef
            {
              be_type *temp;
              be_typedef *t = be_typedef::narrow_from_decl (bt);
        
              if (!t)
                return -1;
        
              temp = t->primitive_base_type ();
              // make a recursive call
              return this->gen_code (t, tdef, temp);
            } // end of case
            break;
          } // end of switch
      } // end of case TYPEDEF_CH
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      os = cg->client_inline ();
      break;
    } // end of outermost switch

  return 0;
}

// return type for array
be_state_array::be_state_array (void)
{
}

// generate code for array type
int
be_state_array::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // get the appropriate stream
  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ARRAY_DEFN_CH:
    case TAO_CodeGen::TAO_ARRAY_OTHER_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ARRAY_DEFN_CI:
      os = cg->client_inline ();
      break;
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
        *os << bt->name () << "_var ";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        *os << bt->name ();
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        *os << "CORBA::String_var ";
      }
      break;
    case AST_Decl::NT_array: 
      // type is an array. This is possible only if we
      // are here thru the typedef node
      {
        *os << bt->name () << "_slice *";
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_enum:     // type is an enum
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct:   // type is a struct
    case AST_Decl::NT_union:    // type is a union
      {
        // based on what state we are in, we may have to generate the definition
        // of the type first
        if (cg->state () == TAO_CodeGen::TAO_ARRAY_DEFN_CH)
          if (bt->gen_client_header () == -1)  // generate the defn
            return -1;

        *os << bt->name ();
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      break;
    }
  return 0;
}

// return type for sequence
be_state_sequence::be_state_sequence (void)
{
}

// generate code for sequence type
int
be_state_sequence::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_sequence *seq;

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
      os = cg->client_header (); // get client header stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
      os = cg->client_inline (); // get client inline stream
      break;
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // for sequences, all we do is generate the type
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
      {
        *os << bt->name () << "_var";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        *os << bt->name ();
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
        return this->gen_code (t, d, temp);
      } 
      break;
    } // end of switch
  return 0;
}

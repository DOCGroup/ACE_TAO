// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_type.cpp
//
// = DESCRIPTION
//    Extension of class AST_Type that provides additional means for C++
//    mapping.
//
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

/*
 * BE_Type
 */

be_type::be_type (void)
{
}

be_type::be_type (AST_Decl::NodeType nt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (nt, n, p)
{
}

// Code generation methods

int
be_type::gen_client_header (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // set the out stream we are working on
  cg->outstream (cg->client_header ());

  // switch on the basis of the node type. This is the first dimension on which
  // we demultiplex. The other dimension is the code generation state inside
  // weach individual function
  switch (this->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      return this->gen_predefined_type ();
    case AST_Decl::NT_string:
      return this->gen_string ();
    case AST_Decl::NT_array:
      return this->gen_array ();
    case AST_Decl::NT_sequence:
      return this->gen_sequence ();
    case AST_Decl::NT_enum:
      return this->gen_enum ();
    case AST_Decl::NT_struct:
      return this->gen_struct ();
    case AST_Decl::NT_union:
      return this->gen_union ();
    case AST_Decl::NT_typedef:
      return this->gen_typedef ();
    case AST_Decl::NT_interface:
      return this->gen_interface ();
    }
  return 0;
}

int
be_type::gen_client_stubs (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // set the out stream we are working on
  cg->outstream (cg->client_stubs ());

  // switch on the basis of the node type. This is the first dimension on which
  // we demultiplex. The other dimension is the code generation state inside
  // weach individual function
  switch (this->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      return this->gen_predefined_type ();
    case AST_Decl::NT_string:
      return this->gen_string ();
    case AST_Decl::NT_array:
      return this->gen_array ();
    case AST_Decl::NT_sequence:
      return this->gen_sequence ();
    case AST_Decl::NT_enum:
      return this->gen_enum ();
    case AST_Decl::NT_struct:
      return this->gen_struct ();
    case AST_Decl::NT_union:
      return this->gen_union ();
    case AST_Decl::NT_typedef:
      return this->gen_typedef ();
    case AST_Decl::NT_interface:
      return this->gen_interface ();
    }
  return 0;
}

int
be_type::gen_server_header (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // set the out stream we are working on
  cg->outstream (cg->server_header ());

  // switch on the basis of the node type. This is the first dimension on which
  // we demultiplex. The other dimension is the code generation state inside
  // weach individual function
  switch (this->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      return this->gen_predefined_type ();
    case AST_Decl::NT_string:
      return this->gen_string ();
    case AST_Decl::NT_array:
      return this->gen_array ();
    case AST_Decl::NT_sequence:
      return this->gen_sequence ();
    case AST_Decl::NT_enum:
      return this->gen_enum ();
    case AST_Decl::NT_struct:
      return this->gen_struct ();
    case AST_Decl::NT_union:
      return this->gen_union ();
    case AST_Decl::NT_typedef:
      return this->gen_typedef ();
    case AST_Decl::NT_interface:
      return this->gen_interface ();
    }
  return 0;
}

int
be_type::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int 
be_type::gen_client_inline (void)
{
  // nothing to be done
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // set the out stream we are working on
  cg->outstream (cg->client_header ());

  // switch on the basis of the node type. This is the first dimension on which
  // we demultiplex. The other dimension is the code generation state inside
  // weach individual function
  switch (this->node_type ())
    {
    case AST_Decl::NT_pre_defined:
    case AST_Decl::NT_string:
    case AST_Decl::NT_enum:
    case AST_Decl::NT_interface:
    case AST_Decl::NT_typedef:
      return 0;
    case AST_Decl::NT_array:
      return be_array::narrow_from_decl (this)->gen_client_inline ();
    case AST_Decl::NT_sequence:
      return be_sequence::narrow_from_decl (this)->gen_client_inline ();
    case AST_Decl::NT_struct:
      return be_structure::narrow_from_decl (this)->gen_client_inline ();
    case AST_Decl::NT_union:
      return be_union::narrow_from_decl (this)->gen_client_inline ();
    }
  return 0;
}

// Generates the server-side inline
int 
be_type::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_type::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->outstream ();
  cs->indent (); // start from the current indentation level
  // switch on the basis of the node type.
  switch (this->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      {
        be_predefined_type *bpd;

        bpd = be_predefined_type::narrow_from_decl (this);
        switch (bpd->pt ())
          {
          case AST_PredefinedType::PT_void:
            *cs << "CORBA::tk_void,\n\n";
            break;
          case AST_PredefinedType::PT_short:
            *cs << "CORBA::tk_short,\n\n";
            break;
          case AST_PredefinedType::PT_ushort:
            *cs << "CORBA::tk_ushort,\n\n";
            break;
          case AST_PredefinedType::PT_long:
            *cs << "CORBA::tk_long,\n\n";
            break;
          case AST_PredefinedType::PT_ulong:
            *cs << "CORBA::tk_ulong,\n\n";
            break;
          case AST_PredefinedType::PT_longlong:
            *cs << "CORBA::tk_longlong,\n\n";
            break;
          case AST_PredefinedType::PT_ulonglong:
            *cs << "CORBA::tk_ulonglong,\n\n";
            break;
          case AST_PredefinedType::PT_float:
            *cs << "CORBA::tk_float,\n\n";
            break;
          case AST_PredefinedType::PT_double:
            *cs << "CORBA::tk_double,\n\n";
            break;
          case AST_PredefinedType::PT_longdouble:
            *cs << "CORBA::tk_longdouble,\n\n";
            break;
          case AST_PredefinedType::PT_boolean:
            *cs << "CORBA::tk_boolean,\n\n";
            break;
          case AST_PredefinedType::PT_char:
            *cs << "CORBA::tk_char,\n\n";
            break;
          case AST_PredefinedType::PT_octet:
            *cs << "CORBA::tk_octet,\n\n";
            break;
          case AST_PredefinedType::PT_any:
            *cs << "CORBA::tk_any,\n\n";
            break;
          case AST_PredefinedType::PT_wchar:
            *cs << "CORBA::tk_wchar,\n\n";
            break;
          }
        return 0;
      }
      break;
    case AST_Decl::NT_string:
      *cs << "CORBA::tk_string, " << nl;
      break;
    case AST_Decl::NT_array:
      *cs << "CORBA::tk_array, " << nl;
      break;
    case AST_Decl::NT_sequence:
      *cs << "CORBA::tk_sequence, " << nl;
      break;
    case AST_Decl::NT_enum:
      *cs << "CORBA::tk_enum, " << nl;
      break;
    case AST_Decl::NT_struct:
      *cs << "CORBA::tk_struct, " << nl;
      break;
    case AST_Decl::NT_union:
      *cs << "CORBA::tk_union, " << nl;
      break;
    case AST_Decl::NT_typedef: // tricky, find its elementary base type
      {
        be_decl *d;
        be_type *bt;
        d = be_decl::narrow_from_decl (cg->node ());
        while (d->node_type () == AST_Decl::NT_typedef)
          {
            // go down the chain to determine the real type
            be_typedef *temp; // temporary

            temp = be_typedef::narrow_from_decl (d);
            d = be_type::narrow_from_decl (temp->base_type ());
          }
        bt = be_type::narrow_from_decl (d);
        // call gen_typecode on bt
        if (bt->be_type::gen_typecode () == -1)
          {
            return -1;
          }
      }
      break;
    case AST_Decl::NT_interface:
      *cs << "CORBA::tk_objref, " << nl;
    }

  // emit our encapsulation length
  *cs << this->tc_encap_len () << ", // encapsulation length\n";
  // use dynamic binding to call the appropriate method
  cs->incr_indent (0);
  if (this->gen_typecode () == -1)
    {

    }
  cs->decr_indent ();
}

long
be_type::tc_encap_len (void)
{
  long encap_len;

  encap_len = 4; // to hold the enum value
  switch (this->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      // they do not have any encapsulation
      break;
    default:
      // everything else has an encapsulation
      encap_len += 4; // to accomodate the size of the encapsulation
      encap_len += this->tc_encap_len ();
    }
  return encap_len;
}

//
// ****************************************************************************
//
// helpers: These are the most important code generation functions.
//
// The helper functions are arranged in a 2-dimensional pattern. The first is
// the suffix of the helper function that identifies the actual type of
// the AST node e.g., predefined type, typedef, so on. The other dimension is
// captured in the form of a switch statement inside each helper. Each case of
// the switch identifies the code generation state e.g., operation, argument,
// so on. The actual node that represents an operation, or an argument (or
// others) can be retrieved from the singleton TAO_CodeGen object via its "node
// ()" method. The assumption is that the be_operation and other classes have
// passed themselves in their code generation methods. For an example see
// be_field::gen_client_header or other similar methods on how to do this.
// ****************************************************************************
//

// generate a PREDEFINED type
int
be_type::gen_predefined_type (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_predefined_type *bd; // predefined type node

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream (); // get the currently active out stream

  bd = be_predefined_type::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type is
                                        // a predefined type
          {
            be_typedef *bt;

            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            os->indent ();
            // special handling for CORBA::Any
            if (bd->pt () == AST_PredefinedType::PT_any)
              {
                *os << "typedef " << bd->name () << " " << bt->local_name () <<
                  ";" << nl;
                *os << "typedef " << bd->name () << "_var " << bt->local_name
                  () << "_var;" << nl;
                *os << "typedef " << bd->name () << "_out " << bt->local_name
                  () << "_out;" << nl;
              }
            else
              *os << "typedef " << bd->name () << " " << bt->local_name () <<
                ";\n\n"; 
          }
        break;
        case TAO_CodeGen::TAO_STRUCT: // generating a struct member whose base
                                      // type is a predefined type
          {
            be_field *f;
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            os->indent (); 
            *os << bd->name () << " " << f->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a sequence base
                                             // whose type is a predefined
                                             // type
          // nothing to be done
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY: // generating a sequence body
                                             // whose base type is a predefined
                                             // type
          {
             // we are here trying to generate code for sequence members. All we
            // need here is the name
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // generating a union
                                                  // discriminant defn whose
                                                  // base type is a predefined
                                                  // type 
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch members of
                                            // predefined type
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (" << bd->name () << 
              ");// set" << nl;
            *os << bd->name () << " " << ub->local_name () << 
              " (void) const; // get method\n\n";
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // union branch private data
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << bd->name () << " " << ub->local_name () << "_;\n\n";
          }
        break;
        case TAO_CodeGen::TAO_OPERATION: // generate operation return type
                                         // which is a predefined type
          {
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_ARGUMENT: // argument of predefined type
          {
            // retrieve the argument node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << bd->name () << " " << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_INOUT:
                *os << bd->name () << " &" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_OUT:
                *os << bd->name () << "_out " << arg->local_name () << ", ";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // attribute of predefined type
          {
            // retrieve the attribute node
            be_attribute *attr = be_attribute::narrow_from_decl (cg->node ());

            os->indent ();
            *os << "virtual " << bd->name () << " " << attr->local_name () <<
              "(void);" << nl;
            if (!attr->readonly ()) // not a readonly attribute
              {
                *os << "virtual void " << attr->local_name () << "(" <<
                  bd->name () << ");" << nl;
              }
            *os << "\n";
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element type
                                     // is a predefined type
          {
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            os->indent ();
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name ();
          break;
        }
    }
  return 0;
}

// generate an ARRAY type
int
be_type::gen_array (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_array *bd;

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_array::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_STRUCT: // generating a struct member whose type
                                      // is an array
          {
            // let the array class take over code generation
            if (bd->gen_client_header () == -1)
              return -1;
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // sequence base of array type is
                                             // not possible
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY:
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // discriminant of array type
                                                  // is not possible
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC:
          {
            be_union_branch *ub;

            // generate the array defn
            if (bd->gen_client_header () == -1)
              return -1;

            // now declare the fields
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (" << bd->name () << 
              ");// set" << nl;
            *os << bd->name () << "_slice *" << ub->local_name () << 
              " (void) const; // get method\n\n";
            
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE:
          {

          }
        break;
        case TAO_CodeGen::TAO_OPERATION: // anonymous array return type not allowed
          break;
        case TAO_CodeGen::TAO_ARGUMENT:  // anonymous array return type for
                                         // argument not allowed 
          break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // array return type for attribute not
                                         // allowed
          break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element type
                                     // itself is an array - syntactically
                                     // impossible unless it is a typedef
          break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // nothing to be done
          break;
        case TAO_CodeGen::TAO_TYPEDEF: // we are generating a TYPEDEF whose
                                       // base type is an array
          {
            os->indent ();
            *os << "typedef ";
            // let the array class take over code generation
            if (bd->gen_client_header () == -1)
              return -1;
          }
        break;
        }
    }
  return 0;
}

int
be_type::gen_string (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_string *bd;     // a string node

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_string::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type
                                        // is a string 
          {
            be_typedef *bt;
            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            os->indent ();
            *os << "typedef CORBA::String " << bt->local_name () << ";" << nl;
            *os << "typedef CORBA::String_var " << bt->local_name () <<
              "_var;" << nl;
            *os << "typedef CORBA::String_out " << bt->local_name () <<
              "_out;\n\n"; 
          }
        break;
        case TAO_CodeGen::TAO_STRUCT: // generating a struct member whose base
                                      // type is a string
          {
            be_field *f;
            be_structure *bs;
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            os->indent (); 
            *os << "CORBA::String_var " << f->local_name () << ";\n\n";
            // this field is a string type.
            // The enclosing structure is thus of variable size
            bs = be_structure::narrow_from_scope (f->defined_in ());
            if (bs == NULL)
              return -1;
            // flag the structure as being a variable sized struct. This is
            // useful for the parameter passing rules
            bs->size_type (be_decl::VARIABLE);
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a seq base whose
                                             // type is string
          {
            // A special class is to be defined here
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY: // generating seq body whose element
                                             // type is a string
          {
            *os << "CORBA::StringSeqElem";
          }
        break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union disc cannot be
                                                  // string type
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch of type string
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
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
        case TAO_CodeGen::TAO_UNION_PRIVATE: // union branch private member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << "CORBA::String_var " << ub->local_name () << "_;\n\n";
          }
        break;
        case TAO_CodeGen::TAO_OPERATION: // operation return type of type
                                         // string 
          {
            *os << "char *";
          }
        break;
        case TAO_CodeGen::TAO_ARGUMENT: // string type for argument
          {
            // retrieve the argument node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << "const char *" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_INOUT:
                *os << "char *&" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_OUT:
                *os << "CORBA::String_out" << arg->local_name () << ", ";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_ATTRIBUTE:  // string type for attribute
          {
            // retrieve the attribute node
            be_attribute *attr = be_attribute::narrow_from_decl (cg->node ());

            os->indent ();
            *os << "virtual char *" << attr->local_name () <<
              "(void);" << nl;
            if (!attr->readonly ()) // not a readonly attribute
              {
                *os << "virtual void " << attr->local_name () <<
                  "(const char *);" << nl;
              }
            *os << "\n";
          }
        break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element type
                                     // is a string
          {
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            os->indent ();
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << "CORBA::String_var";
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name ();
          break;
        }
    }
  return 0;
}

int
be_type::gen_sequence (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_sequence *bd;   // a sequence node

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_sequence::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type is
                                        // a sequence type
          {
            be_typedef *bt;
            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            // generate the definition of this sequence if not done before
            bd->gen_client_header ();
            os->indent (); 
            *os << "typedef " << bd->name () << " " << bt->local_name () <<
              ";" << nl;  
            *os << "typedef " << bd->name () << "_var " << bt->local_name () <<
              "_var;\n\n";  
          }
        break;
        case TAO_CodeGen::TAO_STRUCT: // generating a struct member whose base
                                      // type is a sequence
          {
            // we are here trying to generate code for struct members
            be_field *f;
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            // generate the definition of this sequence if not done before
            bd->gen_client_header ();

            os->indent (); 
            *os << bd->name () << " " << f->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a sequence base
                                             // whose type happens to be a
                                             // sequence
          {
            bd->gen_client_header ();
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY: // generating seq body whose element
                                             // type is a seq
          {
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union disc cannot be a
                                                  // sequence type
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch of type sequence
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            // since this is a sequence, we need to generate its definition
            if (bd->gen_client_header () == -1)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (const " << bd->name () << 
              " &);// set" << nl;
            *os << "const " << bd->name () << " &" << ub->local_name () << 
              " (void) const; // get method (read only)" << nl;
            *os << bd->name () << " &" << ub->local_name () << 
              " (void); // get method (read/write only)\n\n";
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // private union branch member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << bd->name () << " " << ub->local_name () << "_;\n\n";
          }
        break;
        case TAO_CodeGen::TAO_OPERATION: // operation return type cannot use
                                         // anonymous sequence 
          break;
        case TAO_CodeGen::TAO_ARGUMENT:  // anonymous sequence not allowed
          break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // anonymous sequence not allowed
          break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element type
                                     // is a sequence
          {
            // first generate the sequence definition
            if (bd->gen_client_header () == -1)
              return -1;
            os->indent ();
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            os->indent ();
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name ();
          break;
        } // end of switch
    } // end of else
  return 0;
}

int
be_type::gen_enum (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_enum *bd;

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_enum::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type is
                                        // an enum
          {
            be_typedef *bt;
            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            bd->gen_client_header ();
            os->indent ();
            *os << "typedef " << bd->name () << " " << bt->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_STRUCT:
          {
            // we are here trying to generate code for struct members
            be_field *f;
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            // generate the enum definition if not done before
            bd->gen_client_header ();
            os->indent (); 
            *os << bd->name () << " " << f->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a seq base whose
                                             // type is an enum
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY: // generate seq body whose element
                                             // type is an enum
          {
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union discriminant is an
                                                  // enum. 
          {
            // generate the enum declaration
            if (bd->gen_client_header () == -1)
                return -1;
          }
        break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch member of type enum
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            // since this is a sequence, we need to generate its definition
            if (bd->gen_client_header () == -1)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (" << bd->name () << 
              ");// set" << nl;
            *os << bd->name () << ub->local_name () << 
              " (void) const; // get method (read only)\n\n";
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // private union branch member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << bd->name () << " " << ub->local_name () << "_;\n\n";
          }
        break;

        case TAO_CodeGen::TAO_OPERATION: // enum as the operation return type
          *os << bd->name ();
          break;
        case TAO_CodeGen::TAO_ARGUMENT:  // argument of type enum
          {
            // retrieve the argument node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << bd->name () << " " << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_INOUT:
                *os << bd->name () << " &" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_OUT:
                *os << bd->name () << "_out " << arg->local_name () << ", ";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // enum decl as attribute type not
                                         // allowed
          break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element type
                                     // is an enum definition
          {
            // generate the enum definition
            if (bd->gen_client_header () == -1)
              break;
            os->indent ();
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name ();
          break;
        } // end of switch
    } // end of else
  return 0;
}

int
be_type::gen_struct (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_structure *bd;

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_structure::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type
                                        // is a struct 
          {
            be_typedef *bt;
            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            // generate the base type defn if not done before
            bd->gen_client_header ();
            os->indent ();
            *os << "typedef " << bd->name () << " " << bt->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_STRUCT: // generating a struct member whose base
                                      // type itself is a struct
          {
            be_field *f;

            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            // generate the struct definition if not done before
            bd->gen_client_header ();

            os->indent (); 
            *os << bd->name () << " " << f->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a seq base whose
                                             // type is a struct
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY:
          break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union disc cannot be a
                                                  // struct 
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch member of type
                                            // struct 
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            // since this is a struct, we need to generate its definition
            if (bd->gen_client_header () == -1)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (const " << bd->name () << 
              " &);// set" << nl;
            *os << "const " << bd->name () << " &" << ub->local_name () << 
              " (void) const; // get method (read only)" << nl;
            *os << bd->name () << " &" << ub->local_name () << 
              " (void); // get method (read/write only)\n\n";
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // private union branch member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << bd->name () << " " << ub->local_name () << "_;\n\n";
          }
        break;
        case TAO_CodeGen::TAO_OPERATION: //  return type which is a struct
          // if the struct is variable in size, return a pointer to it
          if (bd->size_type () == be_decl::VARIABLE)
            *os << bd->name () << " *";
          else
            *os << bd->name ();
          break;
        case TAO_CodeGen::TAO_ARGUMENT:  // struct type argument
          {
            // retrieve the argument node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << "const " << bd->name () << " " << arg->local_name () <<
                  ", ";
                break;
              case AST_Argument::dir_INOUT:
                *os << bd->name () << " &" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_OUT:
                *os << bd->name () << "_out " << arg->local_name () << ", ";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // struct defn as attribute type not
                                         // allowed
          break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element is a
                                     // struct definition
          {
            // generate the definition for the struct
            if (bd->gen_client_header () == -1)
              return -1;
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            os->indent ();
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name ();
          break;
        } // end of swicth
    } // end of e;se
  return 0;
}

int
be_type::gen_union (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_union *bd;

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_union::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type
                                        // is a union 
          {
            be_typedef *bt;
            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            // generate the base type defn if not done before
            bd->gen_client_header ();
            os->indent ();
            *os << "typedef " << bd->name () << " " << bt->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_STRUCT:
          {
            // we are here trying to generate code for struct members
            be_field *f;
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            // generate the union definition if not done before
            bd->gen_client_header ();
            os->indent (); 
            *os << bd->name () << " " << f->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a seq base whose
                                             // type is a union
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY:
          break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union disc cannot be a
                                                  // union 
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch member of type
                                            // union 
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            // since this is a union, we need to generate its definition
            if (bd->gen_client_header () == -1)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (const " << bd->name () << 
              " &);// set" << nl;
            *os << "const " << bd->name () << " &" << ub->local_name () << 
              " (void) const; // get method (read only)" << nl;
            *os << bd->name () << " &" << ub->local_name () << 
              " (void); // get method (read/write only)\n\n";
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // private union branch member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << bd->name () << " " << ub->local_name () << "_;\n\n";
          }
        break;
        case TAO_CodeGen::TAO_OPERATION: // union return type
          // if the size is variable, return a pointer to the union
          if (bd->size_type () == be_decl::VARIABLE)
            *os << bd->name () << " *";
          else
            *os << bd->name ();
          break;
        case TAO_CodeGen::TAO_ARGUMENT: // union type argument
          {
            // retrieve the argument node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << bd->name () << " " << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_INOUT:
                *os << bd->name () << " &" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_OUT:
                *os << bd->name () << "_out " << arg->local_name () << ", ";
                break;
              }
          }
        break;
          break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // union type attribute
          break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN:
          {
            // generate the definition for the union
            if (bd->gen_client_header () == -1)
              return -1;
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            os->indent ();
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << bd->name ();
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name ();
          break;
        } // end of switch
    } // end of else
  return 0;
}

int
be_type::gen_typedef (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_typedef *bd;    // typedef node
  be_type    *d;     // temporary

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_typedef::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type is
                                        // a typedef
          {
            be_typedef *bt;   // the Typedef node for which we are generating
                              // the code

            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            // generate the base type defn if not done before
            bd->gen_client_header ();
            os->indent ();
            *os << "typedef " << bd->name () << " " << bt->local_name () <<
              ";" << nl;
            // if the real base type is an interface, string, sequence, or
            // array then additional var and/or ptr variables are also generated
            d = bt;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                // go down the chain to determine the real type
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }
            switch (d->node_type ())
              {
              case AST_Decl::NT_interface:
                *os << "typedef " << bd->name () << "_ptr " << bt->local_name
                  () << "_ptr;" << nl;
                *os << "typedef " << bd->name () << "_var " << bt->local_name
                  () << "_var;" << nl;
                *os << "typedef " << bd->name () << "_out " << bt->local_name
                  () << "_out;" << nl;
                break;
              case AST_Decl::NT_string:
              case AST_Decl::NT_sequence:
                *os << "typedef " << bd->name () << "_var " << bt->local_name
                  () << "_var;" << nl;
                *os << "typedef " << bd->name () << "_out " << bt->local_name
                  () << "_out;" << nl;
                break;
              case AST_Decl::NT_array:
                {
                  AST_Decl *scope;  // enclosing scope

                  *os << "typedef " << bd->name () << "_var " << bt->local_name
                    () << "_var;" << nl;
                  *os << "typedef " << bd->name () << "_slice " <<
                    bt->local_name () << "_slice;" << nl;
                  *os << "typedef " << bd->name () << "_out " << bt->local_name
                    () << "_out;" << nl;
                  *os << "typedef " << bd->name () << "_forany " <<
                    bt->local_name () << "_forany;" << nl;
                  // generate the _alloc, _dup, and _free
                  scope = ScopeAsDecl (bt->defined_in ());
                  if (scope && scope->node_type () != AST_Decl::NT_root)
                    {
                      // we are not in the outermost scope or root scope, then
                      // we become static members
                      *os << "static " << bt->local_name () << "_slice *" <<
                        bt->local_name () << "_alloc (void);" << nl;
                      *os << "static " << bt->local_name () << "_slice *" <<
                        bt->local_name () << "_dup (" << bt->local_name () <<
                        "_slice *);" << nl;
                      *os << "static void " << bt->local_name () << "_free ("
                          << bt->local_name () << "_slice *);" << nl; 
                    }
                  else
                    {
                      // we are in the outermost scope or root scope, then
                      // we become simple inline methods
                      *os << bt->local_name () << "_slice *" <<
                        bt->local_name () << "_alloc (void);" << nl;
                      *os << bt->local_name () << "_slice *" <<
                        bt->local_name () << "_dup (" << bt->local_name () <<
                        "_slice *);" << nl;
                      *os << "void " << bt->local_name () << "_free ("
                          << bt->local_name () << "_slice *);" << nl; 
                    }
                }
              }
            *os << "\n";
          }
        break;
        case TAO_CodeGen::TAO_STRUCT: // generating struct members whose base
                                      // type is a typedef
          {
            // we are here trying to generate code for struct members
            be_field *f;
            be_structure *bs;
            
            // retrieve the field node
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;

            // retrieve the struct node
            bs = be_structure::narrow_from_scope (f->defined_in ());
            if (bs == NULL)
              return -1;

            // generate the typedef. If the typedef's base class is a string,
            // generate a var definition. As well as flag the structure as a
            // variable struct.
            os->indent ();
            *os << bd->name ();
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }
            if ((d->node_type () == AST_Decl::NT_string) || 
                (d->node_type () == AST_Decl::NT_interface))
              {
                *os << "_var";
                bs->size_type (be_decl::VARIABLE); // flag it as being
                                                          // variable 
              }
            *os << " " << f->local_name () << ";\n\n";
          }
        break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union disc can be a
                                                  // typedef if it typedefs to
                                                  // simple types. The front
                                                  // end takes care of this
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch member of type
                                            // typedef. We need to understand
                                            // what is the real type
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;

            // find what is the actual type of this TYPEDEF. We may have to
            // traverse a chain of such typedefs (chain of aliases)
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }

            os->indent ();
            switch (d->node_type ())
              {
              case AST_Decl::NT_pre_defined:
              case AST_Decl::NT_enum:
                *os << "void " << ub->local_name () << " (" << bd->name
                  () << ");// set" << nl;
                *os << bd->name () << ub->local_name () <<
                  " (void) const; // get method (read only)" << nl;
                break;
              case AST_Decl::NT_string:
                // three methods to set the string value
                *os << "void " << ub->local_name () << " (char *); // set" 
                    << nl; 
                *os << "void " << ub->local_name () << 
                  " (const char *); // set" << nl; 
                *os << "void " << ub->local_name () << 
                  " (const CORBA::String_var&); // set"  << nl;
                *os << "const char *" << ub->local_name () << 
                  " (void) const; // get method\n\n";
                break;
              case AST_Decl::NT_sequence:
              case AST_Decl::NT_struct:
              case AST_Decl::NT_union:
                *os << "void " << ub->local_name () << " (const " << bd->name
                  () << " &);// set" << nl;
                *os << "const " << bd->name () << " &" << ub->local_name () <<
                  " (void) const; // get method (read only)" << nl;
                *os << bd->name () << " &" << ub->local_name () << 
                  " (void); // get method (read/write only)\n\n";
                break;
              case AST_Decl::NT_interface:
                *os << "void " << ub->local_name () << " (" << bd->name () <<
                  "_ptr);// set" << nl;
                *os << bd->name () << "_ptr " << ub->local_name () << 
                  " (void) const; // get method (read only)\n\n";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // private union branch member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;

            // find what is the actual type of this TYPEDEF. We may have to
            // traverse a chain of such typedefs (chain of aliases)
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }

            os->indent ();
            switch (d->node_type ())
              {
              case AST_Decl::NT_pre_defined:
              case AST_Decl::NT_enum:
              case AST_Decl::NT_sequence:
              case AST_Decl::NT_struct:
              case AST_Decl::NT_union:
                *os << bd->name () << " " << ub->local_name () << "_;\n\n";
                break;
              case AST_Decl::NT_string:
                *os << bd->name () << "_var " << ub->local_name () << "_;\n\n";
                break;
              case AST_Decl::NT_interface:
                *os << bd->name () << "_ptr " << ub->local_name () << "_;\n\n";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_OPERATION: // operation return type which is a
                                         // typedef. Find the actual type
          {
            // find what is the actual type of this TYPEDEF. We may have to
            // traverse a chain of such typedefs (chain of aliases)
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }

            os->indent ();
            switch (d->node_type ())
              {
              case AST_Decl::NT_pre_defined:
              case AST_Decl::NT_enum:
                *os << bd->name ();
                break;
              case AST_Decl::NT_sequence:
                *os << bd->name () << " *";
                break;
              case AST_Decl::NT_array:
                *os << bd->name () << "_slice *";
                break;
              case AST_Decl::NT_struct:
                // depending on whether it is fixed or variable
                {
                  be_structure *bs = be_structure::narrow_from_decl (d);
                  *os << bd->name ();
                  if (bs->size_type () == be_decl::VARIABLE)
                    *os << " *";
                }
                break;
              case AST_Decl::NT_union:
                // depending on whether it is fixed or variable
                {
                  be_union *bu = be_union::narrow_from_decl (d);
                  *os << bd->name ();
                  if (bu->size_type () == be_decl::VARIABLE)
                    *os << " *";
                }
              break;
              case AST_Decl::NT_string:
                // just a char* or CORBA::String
                *os << bd->name ();
                break;
              case AST_Decl::NT_interface:
                *os << bd->name () << "_ptr";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_ARGUMENT: // typedefed argument type
          {
            // retrieve the attribute node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());

            // find what is the actual type of this TYPEDEF. We may have to
            // traverse a chain of such typedefs (chain of aliases)
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }

            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                {
                  switch (d->node_type ())
                    {
                    case AST_Decl::NT_pre_defined:
                      {
                        be_predefined_type *bpd =
                          be_predefined_type::narrow_from_decl (d);

                        // special mapping for CORBA::Any
                        if (bpd->pt () == AST_PredefinedType::PT_any)
                          *os << "const " << bd->name () << " &" <<
                            arg->local_name () << ", "; 
                        else
                          *os << bd->name () << " " << arg->local_name () <<
                            ","; 
                      }
                      break;
                    case AST_Decl::NT_enum:
                      *os << bd->name () << " " << arg->local_name () << ", ";
                      break;
                    case AST_Decl::NT_sequence:
                      *os << "const " << bd->name () << " &" << arg->local_name
                        () << ", "; 
                      break;
                    case AST_Decl::NT_array:
                      *os << "const " << bd->name () << " " << arg->local_name
                        () << ", ";
                      break;
                    case AST_Decl::NT_struct:
                      *os << "const " << bd->name () << " " << arg->local_name
                        () << ", ";
                      break;
                    case AST_Decl::NT_union:
                      *os << "const " << bd->name () << " " << arg->local_name
                        () << ", ";
                      break;
                    case AST_Decl::NT_string:
                      *os << "const " << bd->name () << " " << arg->local_name
                        () << ", "; 
                      break;
                    case AST_Decl::NT_interface:
                      *os << bd->name () << "_ptr" << arg->local_name () << ", ";
                      break;
                    } // end of nested switch
                }
              break;
              case AST_Argument::dir_INOUT:
                {
                  switch (d->node_type ())
                    {
                    case AST_Decl::NT_pre_defined:
                      {
                        be_predefined_type *bpd;

                        bpd = be_predefined_type::narrow_from_decl (d);
                        // special mapping for CORBA::Any
                        if (bpd->pt () == AST_PredefinedType::PT_any)
                          *os << bd->name () << " &" <<
                            arg->local_name () << ", "; 
                        else
                          *os << bd->name () << " &" << arg->local_name () <<
                            ",";
                      }
                      break;
                    case AST_Decl::NT_enum:
                      *os << bd->name () << " &" << arg->local_name () << ", ";
                      break;
                    case AST_Decl::NT_sequence:
                      *os << bd->name () << " &" << arg->local_name () << ", ";
                      break;
                    case AST_Decl::NT_array:
                      *os << bd->name () << " " << arg->local_name () << ", ";
                      break;
                    case AST_Decl::NT_struct:
                      *os << bd->name () << " &" << arg->local_name () << ", ";
                      break;
                    case AST_Decl::NT_union:
                      *os << bd->name () << " &" << arg->local_name () << ", ";
                      break;
                    case AST_Decl::NT_string:
                      *os << bd->name () << " &" << arg->local_name () << ", "; 
                      break;
                    case AST_Decl::NT_interface:
                      *os << bd->name () << "_ptr&" << arg->local_name () << 
                        ","; 
                      break;
                    } // end of nested switch
                } // end case for inout
                break;
              case AST_Argument::dir_OUT:
                {
                  *os << bd->name () << "_out " << arg->local_name () << ", ";
                } // end of case
                break;
              } // end switch on direction type
          } // end case Argument
        break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // typedefed attribute type
          {
            // retrieve the attribute node
            be_attribute *attr = be_attribute::narrow_from_decl (cg->node ());

            os->indent ();
            // find what is the actual type of this TYPEDEF. We may have to
            // traverse a chain of such typedefs (chain of aliases)
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }

            os->indent ();
            switch (d->node_type ())
              {
              case AST_Decl::NT_pre_defined:
              case AST_Decl::NT_enum:
                *os << "virtual " << bd->name () << " " << attr->local_name ()
                    << "(void);" << nl;
                if (!attr->readonly ()) // not a readonly attribute
                  {
                    *os << "virtual void " << attr->local_name () << "(" <<
                      bd->name () << ");" << nl;
                  }
                break;
              case AST_Decl::NT_sequence:
                *os << bd->name () << " *";
                break;
              case AST_Decl::NT_struct:
                // depending on whether it is fixed or variable
                {
                  be_structure *bs = be_structure::narrow_from_decl (d);
                  *os << bd->name ();
                  if (bs->size_type () == be_decl::VARIABLE)
                    *os << " *";
                }
                break;
              case AST_Decl::NT_union:
                // depending on whether it is fixed or variable
                {
                  be_union *bu = be_union::narrow_from_decl (d);
                  *os << bd->name ();
                  if (bu->size_type () == be_decl::VARIABLE)
                    *os << " *";
                }
              break;
              case AST_Decl::NT_string:
                *os << bd->name () << "_var";
                break;
              case AST_Decl::NT_interface:
                *os << bd->name () << "_ptr";
                break;
              } // end of nested switch
            *os << "\n";
          } // end of case
          break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating a seq base whose
                                             // type is a typedef
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY:
        case TAO_CodeGen::TAO_ARRAY_DEFN: // generating an array whose element
                                          // type is a typedefed quantify
        case TAO_CodeGen::TAO_ARRAY_OTHER:
          {
            // find what is the actual type of this TYPEDEF. We may have to
            // traverse a chain of such typedefs (chain of aliases)
            d = bd;
            while (d->node_type () == AST_Decl::NT_typedef)
              {
                be_typedef *temp; // temporary

                temp = be_typedef::narrow_from_decl (d);
                d = be_type::narrow_from_decl (temp->base_type ());
              }

            os->indent ();
            switch (d->node_type ())
              {
              case AST_Decl::NT_string:
                *os << bd->name () << "_var";
                break;
              case AST_Decl::NT_interface:
                *os << bd->name () << "_ptr";
                break;
              default:
                *os << bd->name ();
                break;
              } // end of nested switch
          } // end of case
          break;
        } // end of switch
    } // end of else
  return 0;
}

int
be_type::gen_interface (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope
  be_interface *bd;

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  os = cg->outstream ();

  bd = be_interface::narrow_from_decl (this);
  if (bd == NULL)
    return -1;
  else
    {
      switch (cg->state ())
        {
        case TAO_CodeGen::TAO_TYPEDEF:  // generating a typedef whose base type is
                                        // a union
          {
            be_typedef *bt;
            bt = be_typedef::narrow_from_decl (cg->node ());
            if (bt == NULL)
              return -1;
            os->indent ();
            *os << "typedef " << bd->name () << " " << bt->local_name () << ";"
                << nl;
            *os << "typedef " << bd->name () << "_ptr " << bt->local_name () <<
              "_ptr;" << nl; 
            *os << "typedef " << bd->name () << "_var " << bt->local_name () <<
              "_var;\n\n"; 
          }
        break;
        case TAO_CodeGen::TAO_STRUCT: // generate code for struct members whose
                                      // base type is an interface
          {
            be_structure *bs;
            be_field *f;
            f = be_field::narrow_from_decl (cg->node ());
            if (f == NULL)
              return -1;
            os->indent (); 
            // this field is an obj reference type. We cannot simply generate
            // the name of the interface since it cannot be instantiated or
            // copied. So we generate a _var. In addition, this causes the
            // enclosing structure to become a variable length struct
            *os << bd->name () << "_var " << f->local_name () << ";\n\n";
            bs = be_structure::narrow_from_scope (f->defined_in ());
            if (bs == NULL)
              return -1;
            bs->size_type (be_decl::VARIABLE);
          }
        break;
        case TAO_CodeGen::TAO_SEQUENCE_BASE: // generating seq base whose type
                                             // is an interface
          break;
        case TAO_CodeGen::TAO_SEQUENCE_BODY:
          break;
        case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN: // union disc cannot be an
                                                  // interface type
          break;
        case TAO_CodeGen::TAO_UNION_PUBLIC: // union branch whose type is an
                                            // interface 
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << "void " << ub->local_name () << " (" << bd->name () << 
              "_ptr);// set" << nl;
            *os << bd->name () << "_ptr " << ub->local_name () << 
              " (void) const; // get method (read only)\n\n";
          }
          break;
        case TAO_CodeGen::TAO_UNION_PRIVATE: // private union branch member
          {
            be_union_branch *ub;
            ub = be_union_branch::narrow_from_decl (cg->node ());
            if (ub == NULL)
              return -1;
            os->indent ();
            *os << bd->name () << "_ptr " << ub->local_name () << "_;\n\n";
          }
        break;

        case TAO_CodeGen::TAO_OPERATION: // interface as operation return type
          {
            *os << bd->name () << "_ptr";
          }
        break;
        case TAO_CodeGen::TAO_ARGUMENT: // argument whose type is an object
                                        // reference 
          {
            // retrieve the argument node
            be_argument *arg = be_argument::narrow_from_decl (cg->node ());
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                *os << bd->name () << "_ptr " << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_INOUT:
                *os << bd->name () << "_ptr &" << arg->local_name () << ", ";
                break;
              case AST_Argument::dir_OUT:
                *os << bd->name () << "_out " << arg->local_name () << ", ";
                break;
              }
          }
        break;
        case TAO_CodeGen::TAO_ATTRIBUTE: // attribute whose type is an obj
                                         // reference 
          {
            // retrieve the attribute node
            be_attribute *attr = be_attribute::narrow_from_decl (cg->node ());

            os->indent ();
            *os << "virtual " << bd->name () << " " << attr->local_name () <<
              "(void);" << nl;
            if (!attr->readonly ()) // not a readonly attribute
              {
                *os << "virtual void " << attr->local_name () << "(" <<
                  bd->name () << ");" << nl;
              }
            *os << "\n";
          }
        break;
        case TAO_CodeGen::TAO_ENUM:
          break;
        case TAO_CodeGen::TAO_ARRAY_DEFN: // an array of interfaces
          {
            // retrieve the node passed in thru the code gen object. This node
            // may be a typedef
            be_decl *d = be_decl::narrow_from_decl (cg->node ());
            os->indent ();
            if (d->node_type () == AST_Decl::NT_typedef)
              *os << "typedef ";
            *os << bd->name () << "_var";
          }
        break;
        case TAO_CodeGen::TAO_ARRAY_OTHER: // rest of the array defn
          *os << bd->name () << "_var";
          break;
        } // end of switch
    } // end of else
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_type, AST_Type, be_decl)
IMPL_NARROW_FROM_DECL (be_type)

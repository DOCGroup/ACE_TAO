// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_array.h
//
// = DESCRIPTION
//    Extension of class AST_Array that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (BE_ARRAY_H)
#define BE_ARRAY_H

class TAO_OutStream;

/*
 * BE_Array
 */
class be_array : public virtual AST_Array,
                 public virtual be_type
{
public:
  // =Operations

  be_array (void);
  // default constructor

  be_array (UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims);
  // constructor

  ~be_array (void);

  virtual int gen_client_header (void);
  // generate client header for array

  virtual int gen_client_stubs (void);
  // generate client side stubs for array

  virtual int gen_server_header (void);
  // generate server header for array

  virtual int gen_server_skeletons (void);
  // generate server skeletons for array

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the array

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the array

  virtual int gen_var_defn (void);
  // generate the _var class definition

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  virtual int gen_forany_defn (void);
  // the forany class

  virtual int gen_forany_impl (void);
  // the forany impl

  virtual int gen_dimensions (TAO_OutStream *os, unsigned short slice=0);
  // generate dimensions. If slice == 1, generate dimensions for the slice
  // definition

  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  virtual int write_as_return (TAO_OutStream *stream,
			       be_type *type);
  // write the mapping as a return value on the stream.

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_array, AST_Array, be_type);
  DEF_NARROW_FROM_DECL (be_array);

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown

  virtual int create_name (void);
  // create a name for us

  const char* tao_name (void);

private:
  char* tao_name_;
  // We need a name to generate typecodes and other objects, but it
  // must be different from the typedef name....
};

#endif

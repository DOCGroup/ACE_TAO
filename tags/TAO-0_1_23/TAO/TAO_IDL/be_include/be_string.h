// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_string.h
//
// = DESCRIPTION
//    Extension of class AST_String that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (BE_STRING_H)
#define BE_STRING_H

/*
 * BE_String
 */
class be_string : public virtual AST_String,
                  public virtual be_type
{
public:
  // Operations

  be_string (void);
  //  default constructor

  be_string (AST_Expression *v);
  // constructor

  be_string (AST_Expression *v, long wide);
  // constructor

  virtual int gen_client_header (void);
  // generate client header for string

  virtual int gen_client_stubs (void);
  // generate client side stubs for string

  virtual int gen_server_header (void);
  // generate server header for string

  virtual int gen_server_skeletons (void);
  // generate server skeletons for string

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the string

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the string

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
  DEF_NARROW_METHODS2 (be_string, AST_String, be_type);
  DEF_NARROW_FROM_DECL (be_string);

protected:
  virtual void compute_tc_name (void);
  // overridden method to compute typecode name
};

#endif

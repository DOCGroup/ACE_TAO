// Id
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_native.h
//
// = DESCRIPTION
//    The native IDL type
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================


#if !defined (BE_NATIVE_H)
#define BE_NATIVE_H

// Representation of "native" IDL type added by the POA spec

class be_native : public virtual AST_Native, public virtual be_type
{
public:
  // =Operations

  // Constructor(s)
  be_native (void);
  // default constructor

  be_native(UTL_ScopedName *n, UTL_StrList *p);
  // constructor that initializes its scoped name

  virtual ~be_native (void) {}
  // destructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the type

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the type

  virtual int gen_server_header (void);
  // Generates the server-side header information for the type

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the type

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the type

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the type

  virtual int gen_typecode (void);
  // generate the typecode description

  virtual long tc_size (void);
  // return typecode size

  // Narrowing
  DEF_NARROW_METHODS2(be_native, AST_Native, be_type);
  DEF_NARROW_FROM_DECL(be_native);
};

#endif /* BE_NATIVE_H */

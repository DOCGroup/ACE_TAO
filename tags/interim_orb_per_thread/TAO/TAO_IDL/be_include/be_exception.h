#if !defined(BE_EXCEPTION_H)
#define BE_EXCEPTION_H

/*
 * BE_Exception
 */
class be_exception : public virtual AST_Exception,
                     public virtual be_scope,
                     public virtual be_decl
                       
{
public:
  // Operations
  be_exception (void);
  // default constructor

  be_exception (UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  // =code generation

  virtual int gen_client_header (void);
  // generate client header

  virtual int gen_client_inline (void);
  // generate client inline

  virtual int gen_client_stubs (void);
  // generate client stubs

  virtual int gen_server_header (void);
  // generate server header

  virtual int gen_server_inline (void);
  // generate server inline

  virtual int gen_server_skeletons (void);
  // generate server skeletons

  virtual int gen_typecode (void);
  // generate the typecode

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

  virtual int member_count (void);
  // return the count of members

  // Narrowing
  DEF_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_decl);
  DEF_NARROW_FROM_DECL (be_exception);
  DEF_NARROW_FROM_SCOPE (be_exception);

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int member_count_;
  // number of members
};

#endif

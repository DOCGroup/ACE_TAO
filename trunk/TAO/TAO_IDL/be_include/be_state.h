/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_state.h
//
// = DESCRIPTION
//   The Code generator state class and its subclasses
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_BE_STATE_H)
#define TAO_BE_STATE_H

class be_state
{
  // =TITLE
  // be_state
  // =DESCRIPTION
  // Emits code based on current state. This is an abstract base class. The
  // task of code generation is handled by subclasses.
public:
  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0) = 0;
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef

  virtual ~be_state (void);
  // destructor

protected:
  be_state (void);
  // constructor
};

// ============== subclasses ====================

class be_state_struct_ch: public be_state
{
  // =TITLE
  // be_state_struct_ch
  // =DESCRIPTION
  // struct generation in client header
public:
  
  be_state_struct_ch (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_struct_ch, ACE_SYNCH_MUTEX> TAO_BE_STATE_STRUCT_CH; 

class be_state_union_disctypedefn_ch: public be_state
{
  // =TITLE
  // be_state_union_disctypedefn_ch
  // =DESCRIPTION
  // union discriminant defn generation in client header
public:
  
  be_state_union_disctypedefn_ch (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_union_disctypedefn_ch, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_UNION_DISCTYPEDEFN_CH;  

class be_state_union_disctypedefn_ci: public be_state
{
  // =TITLE
  // be_state_union_disctypedefn_ci
  // =DESCRIPTION
  // union discriminant defn generation in client inline file
public:
  
  be_state_union_disctypedefn_ci (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_union_disctypedefn_ci, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_UNION_DISCTYPEDEFN_CI;  

class be_state_union_public_ch: public be_state
{
  // =TITLE
  // be_state_union_public_ch
  // =DESCRIPTION
  // union discriminant defn generation in client header file
public:
  
  be_state_union_public_ch (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_union_public_ch, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_UNION_PUBLIC_CH;  

class be_state_union_public_ci: public be_state
{
  // =TITLE
  // be_state_union_public_ci
  // =DESCRIPTION
  // union discriminant defn generation in client inline file
public:
  
  be_state_union_public_ci (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_union_public_ci, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_UNION_PUBLIC_CI;  

class be_state_union_private_ch: public be_state
{
  // =TITLE
  // be_state_union_private_ch
  // =DESCRIPTION
  // union discriminant defn generation in client header file
public:
  
  be_state_union_private_ch (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_union_private_ch, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_UNION_PRIVATE_CH;  

class be_state_operation: public be_state
{
  // =TITLE
  // be_state_operation
  // =DESCRIPTION
  // operation return type
public:
  
  be_state_operation (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_operation, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_OPERATION;

class be_state_argument: public be_state
{
  // =TITLE
  // be_state_argument
  // =DESCRIPTION
  // argument return type
public:
  
  be_state_argument (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_argument, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_ARGUMENT;

class be_state_typedef: public be_state
{
  // =TITLE
  // be_state_typedef
  // =DESCRIPTION
  // typedef code generation
public:
  
  be_state_typedef (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_typedef, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_TYPEDEF;

class be_state_array: public be_state
{
  // =TITLE
  // be_state_array
  // =DESCRIPTION
  // array code generation
public:
  
  be_state_array (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_array, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_ARRAY;

class be_state_sequence: public be_state
{
  // =TITLE
  // be_state_sequence
  // =DESCRIPTION
  // sequence code generation
public:
  
  be_state_sequence (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_sequence, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_SEQUENCE;

class be_state_attribute: public be_state
{
  // =TITLE
  // be_state_attribute
  // =DESCRIPTION
  // attribute code generation
public:
  
  be_state_attribute (void);
  // constructor

  virtual int gen_code (be_type *bt, be_decl *d, be_type *type=0);
  // code generation for node "d" whose type is "bt". The third parameter is
  // used for recursive invocation involving a typedef
};
typedef ACE_Singleton<be_state_attribute, ACE_SYNCH_MUTEX>
                                    TAO_BE_STATE_ATTRIBUTE;

#endif // end of be_state


/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    varout.h
//
// = DESCRIPTION
//     Templates for _var and _out types
//
// = AUTHOR
//
//   Aniruddha Gokhale 
// ============================================================================

#if !defined (TAO_VAROUT_H)
#define TAO_VAROUT_H

// *****************************************************
// _var for fixed length struct and union data types
// *****************************************************

template <class T>
class TAO_Fixed_var
{
  // =TITLE
  //  template<class T> TAO_Fixed_var
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for fixed length data
  //  structs and unions.
public:
  // =Operations

  TAO_Fixed_var (void);
  // default constructor

  TAO_Fixed_var (T *);
  // constructor from a pointer to T

  TAO_Fixed_var (const TAO_Fixed_var<T> &);
  // copy constructor

  ~TAO_Fixed_var (void);
  // destructor

  TAO_Fixed_var<T> &operator= (T *);
  // assignment from a pointer to T

  TAO_Fixed_var<T> &operator= (const TAO_Fixed_var<T> &);
  // assignment from a TAO_Fixed_var

  T *operator-> (void);
  // smart pointer

  const T *operator-> (void) const;
  // smart pointer (const)

  operator const T *&() const;
  // cast operator

  operator T *&();
  // cast operator

  // operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T &out (void);
  // for out parameter

  T _retn (void);
  // for return type
private:
  T *ptr_;
  // instance
};

// *****************************************************
// _var for variable length struct and union data types
// *****************************************************

template <class T>
class TAO_Variable_var
{
  // =TITLE
  //  template<class T> TAO_Variable_var
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for Variable length data
  //  structs and unions.
public:
  // =Operations

  TAO_Variable_var (void);
  // default constructor

  TAO_Variable_var (T *);
  // constructor from a pointer to T

  TAO_Variable_var (const TAO_Variable_var<T> &);
  // copy constructor

  ~TAO_Variable_var (void);
  // destructor

  TAO_Variable_var<T> &operator= (T *);
  // assignment from a pointer to T

  TAO_Variable_var<T> &operator= (const TAO_Variable_var<T> &);
  // assignment from a TAO_Variable_var

  T *operator-> (void);
  // smart pointer

  const T *operator-> (void) const;
  // smart pointer (const)

  operator const T *&() const;
  // cast operator

  operator T *&();
  // cast operator

  // operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T *&out (void);
  // for out parameter

  T *_retn (void);
  // for return type

private:
  T *ptr_;
};

// *****************************************************
// _var and _out types for sequences. CORBA sequences are considered to be
// always variable length
// *****************************************************

template <class T>
class TAO_Sequence_var
{
  // =TITLE
  //  template<class T> TAO_Sequence_var
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for Sequence data type
public:
  // =Operations

  TAO_Sequence_var (void);
  // default constructor

  TAO_Sequence_var (T *);
  // constructor from a pointer to T

  TAO_Sequence_var (const TAO_Sequence_var &);
  // copy constructor

  ~TAO_Sequence_var (void);
  // destructor

  TAO_Sequence_var<T> &operator= (T *);
  // assignment from a pointer to T

  TAO_Sequence_var<T> &operator= (const TAO_Sequence_var &);
  // assignment from a TAO_Sequence_var

  T *operator-> (void);
  // smart pointer

  const T *operator-> (void) const;
  // smart pointer (const)

  operator const T *&() const;
  // cast operator

  operator T *&();
  // cast operator

  // additional operators for sequence

  T::ElemType &operator[] (CORBA::ULong);
  // operator [] that passes the argument to the [] operator of T

  // =operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T *&out (void);
  // for out parameter

  T *_retn (void);
  // for return type
private:
  T *ptr_;
  // seq instance
};

// ***************************************
// _var class for object references
// ***************************************

template <class T_ptr>
class TAO_Object_var
{
  // =TITLE
  //  template<class T, class T_ptr> TAO_Object_var
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for obj references
public:
  // =Operations

  TAO_Object_var (void);
  // default constructor

  TAO_Object_var (T_ptr);
  // constructor from a pointer to T

  TAO_Object_var (const TAO_Object_var<T_ptr> &);
  // copy constructor

  ~TAO_Object_var (void);
  // destructor

  TAO_Object_var<T_ptr> &operator= (T_ptr);
  // assignment from a pointer to T

  TAO_Object_var<T_ptr> &operator= (const TAO_Object_var<T_ptr> &);
  // assignment from a TAO_Object_var

  T_ptr operator-> (void) const;
  // smart pointer (const)

  operator const T_ptr &() const;
  // cast operator

  operator T_ptr &();
  // cast operator

  // operations for parameter passing modes

  const T_ptr &in (void);
  // for in type parameter

  T_ptr &inout (void);
  // for inout type parameter

  T_ptr &out (void);
  // for out parameter

  T_ptr _retn (void);
  // for return type
private:
  T_ptr ptr_;
};

// *****************************************************
// _var class for fixed arrays
// *****************************************************

template <class T, class T_slice>
class TAO_FixedArray_var
{
  // =TITLE
  //  template<class T, class T_slice> TAO_FixedArray_var
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for obj references
public:
  // =Operations

  TAO_FixedArray_var (void);
  // default constructor

  TAO_FixedArray_var (T_slice *);
  // constructor from a pointer to T

  TAO_FixedArray_var (const TAO_FixedArray_var<T, T_slice> &);
  // copy constructor

  ~TAO_FixedArray_var (void);
  // destructor

  TAO_FixedArray_var<T, T_slice> &operator= (T_slice *);
  // assignment from a pointer to T

  TAO_FixedArray_var<T, T_slice> &operator= (const TAO_FixedArray_var<T, T_slice> &);
  // assignment from a TAO_FixedArray_var

  T_slice &operator[] (CORBA::ULong);
  // index

  const T_slice &operator[] (CORBA::ULong) const;
  // index

  operator const T_slice &() const;
  // cast operator

  operator T_slice &();
  // cast operator


  // operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T &out (void);
  // for out parameter

  T_slice *_retn (void);
  // for return type
private:
  T_slice *ptr_;
};

// *****************************************************
// _var class for variable arrays
// *****************************************************

template <class T, class T_slice>
class TAO_VariableArray_var
{
  // =TITLE
  //  template<class T, class T_slice> TAO_VariableArray_var
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for obj references
public:
  // =Operations

  TAO_VariableArray_var (void);
  // default constructor

  TAO_VariableArray_var (T_slice *);
  // constructor from a pointer to T

  TAO_VariableArray_var (const TAO_VariableArray_var<T, T_slice> &);
  // copy constructor

  ~TAO_VariableArray_var (void);
  // destructor

  TAO_VariableArray_var<T, T_slice> &operator= (T_slice *);
  // assignment from a pointer to T

  TAO_VariableArray_var<T, T_slice> &operator= (const TAO_VariableArray_var<T, T_slice> &);
  // assignment from a TAO_VariableArray_var

  T_slice &operator[] (CORBA::ULong);
  // index

  const T_slice &operator[] (CORBA::ULong) const;
  // index

  operator const T_slice &() const;
  // cast operator

  operator T_slice &();
  // cast operator

  // =operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T_slice *&out (void);
  // for out parameter

  T_slice *_retn (void);
  // for return type
private:
  T_slice *ptr_;
};

// *****************************************************
// _forany class for fixed arrays
// *****************************************************

template <class T, class T_slice>
class TAO_FixedArray_forany
{
  // =TITLE
  //  template<class T, class T_slice> TAO_FixedArray_forany
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for obj references
public:
  // =Operations

  TAO_FixedArray_forany (void);
  // default constructor

  TAO_FixedArray_forany (T_slice *, CORBA::Boolean nocopy=0);
  // constructor from a pointer to T slice

  TAO_FixedArray_forany (const TAO_FixedArray_forany<T, T_slice> &);
  // copy constructor

  ~TAO_FixedArray_forany (void);
  // destructor

  TAO_FixedArray_forany<T, T_slice> &operator= (T_slice *);
  // assignment from a pointer to T

  TAO_FixedArray_forany<T, T_slice> &operator= (const TAO_FixedArray_forany<T, T_slice> &);
  // assignment from a TAO_FixedArray_forany

  T_slice &operator[] (CORBA::ULong);
  // index

  const T_slice &operator[] (CORBA::ULong) const;
  // index

  operator const T_slice &() const;
  // cast operator

  operator T_slice &();
  // cast operator

  // =operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T &out (void);
  // for out parameter

  T_slice *_retn (void);
  // for return type
private:
  T_slice *ptr_;
};

// *****************************************************
// _forany class for variable arrays
// *****************************************************

template <class T, class T_slice>
class TAO_VariableArray_forany
{
  // =TITLE
  //  template<class T, class T_slice> TAO_VariableArray_forany
  // =DESCRIPTION
  //  Provides a parametrized version of the _var class for obj references
public:
  // =Operations

  TAO_VariableArray_forany (void);
  // default constructor

  TAO_VariableArray_forany (T_slice *, CORBA::Boolean nocopy=0);
  // constructor from a pointer to T slice

  TAO_VariableArray_forany (const TAO_VariableArray_forany<T, T_slice> &);
  // copy constructor

  ~TAO_VariableArray_forany (void);
  // destructor

  TAO_VariableArray_forany<T, T_slice> &operator= (T_slice *);
  // assignment from a pointer to T

  TAO_VariableArray_forany<T, T_slice> &operator= (const TAO_VariableArray_forany<T, T_slice> &);
  // assignment from a TAO_VariableArray_forany

  T_slice &operator[] (CORBA::ULong);
  // index

  const T_slice &operator[] (CORBA::ULong) const;
  // index

  operator const T_slice &() const;
  // cast operator

  operator T_slice &();
  // cast operator

  // =operations for parameter passing modes

  const T &in (void);
  // for in type parameter

  T &inout (void);
  // for inout type parameter

  T_slice *&out (void);
  // for out parameter

  T_slice *_retn (void);
  // for return type
private:
  T_slice *ptr_;
};

// *****************************************************
// there is no _out class for a fixed length types
// *****************************************************

// *****************************************************
// _out class for variable length struct and union
// *****************************************************

template <class T, class T_var>
class TAO_Variable_out
{
  // =TITLE
  // TAO_Variable_out
  // =Description
  // _out class definition for variable size struct/union
public:
  TAO_Variable_out (T *&);
  // constructor

  TAO_Variable_out (T_var &);
  // constructor

  TAO_Variable_out (TAO_Variable_out <T,T_var> &);
  // copy constructor

  TAO_Variable_out<T,T_var> &operator= (TAO_Variable_out<T,T_var> &);
  // assignment operator

  TAO_Variable_out<T,T_var> &operator= (T *);
  // assignment

  operator T *&();
  // cast

  T *&ptr (void);
  // ptr method, returns a pointer to class T

  T *operator-> (void);
  // arrow operator (smart pointer)

private:
  T *&ptr_;
};

// *****************************************************
// _out class for sequence
// *****************************************************

template <class T, class T_var>
class TAO_Sequence_out
{
  // =TITLE
  // TAO_Sequence_out
  // =Description
  // _out class definition for Sequence
public:
  TAO_Sequence_out (T *&);
  // constructor

  TAO_Sequence_out (T_var &);
  // constructor

  TAO_Sequence_out (TAO_Sequence_out <T,T_var> &);
  // copy constructor

  TAO_Sequence_out<T,T_var> &operator= (TAO_Sequence_out<T,T_var> &);
  // assignment operator

  TAO_Sequence_out<T,T_var> &operator= (T *);
  // assignment

  operator T *&();
  // cast

  T *&ptr (void);
  // ptr method, returns a pointer to class T

  T *operator-> (void);
  // arrow operator (smart pointer)

  T::ElemType operator[] (CORBA::ULong);
  // return the element at index i

private:
  T *&ptr_;
};

// *****************************************************
// _out class for obj ref
// *****************************************************

template <class T_ptr, class T_var>
class TAO_Object_out
{
  // =TITLE
  // TAO_Object_out
  // =Description
  // _out class definition for Object
public:
  TAO_Object_out (T *&);
  // constructor

  TAO_Object_out (T_var &);
  // constructor

  TAO_Object_out (TAO_Object_out <T,T_var> &);
  // copy constructor

  TAO_Object_out<T,T_var> &operator= (TAO_Object_out<T,T_var> &);
  // assignment operator

  TAO_Object_out<T,T_var> &operator= (T *);
  // assignment

  operator T *&();
  // cast

  T *&ptr (void);
  // ptr method, returns a pointer to class T

  T *operator-> (void);
  // arrow operator (smart pointer)

  T::ElemType operator[] (CORBA::ULong);
  // return the element at index i

private:
  T *&ptr_;
};

// *****************************************************
// _out class for arrays
// *****************************************************

#endif // end of if !defined (...) 


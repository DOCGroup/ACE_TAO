// $Id$

#include <stdio.h>
#include "marshalC.h"

ACE_RCSID(TAO, marshalC, "$Id$")

// constructor
Marshal::Marshal(STUB_Object *refdata)
  : CORBA_Object(refdata)
{
}

// static duplicate method
Marshal_ptr Marshal::_duplicate(Marshal_ptr obj)
{
  void *dupl;

  if (obj)
    {
      obj->AddRef();
    }
  return obj;
}

// static _narrow method
Marshal_ptr Marshal::_narrow(CORBA_Object_ptr obj)
{
  CORBA_Environment env;
  if (obj->_is_a("IDL:Marshal:1.0", env)){
    STUB_Object *istub;
    if (obj->QueryInterface(IID_STUB_Object, (void **)&istub) != NOERROR){
      return Marshal::_nil();
    }
    Marshal_ptr new_obj = new Marshal(istub);
    return Marshal::_duplicate(new_obj);
  }
  return Marshal::_nil();
}

Marshal_ptr Marshal::_nil()
{
  return (Marshal_ptr)NULL;
}

Marshal::Marshal_Union::Marshal_Union(const Marshal::Marshal_Union& obj) 
{
  _disc = obj._d();
  switch (_disc) {
  case e_0th:
    __s = obj.__s;
    break;
  case e_1st:
    __l = obj.__l;
    break;
  case e_2nd:
    __c = obj.__c;
    break;
  case e_3rd:
    __o = obj.__o;
    break;
  case e_4th:
    __d = obj.__d;
    break;
  case e_5th:
  default:
    __ms = obj.__ms;
    break;
  }
}

Marshal::Marshal_Union& Marshal::Marshal_Union::operator=(const Marshal::Marshal_Union& obj)
{
  _disc = obj._d();
  switch (_disc) {
  case e_0th:
    __s = obj.__s;
    break;
  case e_1st:
    __l = obj.__l;
    break;
  case e_2nd:
    __c = obj.__c;
    break;
  case e_3rd:
    __o = obj.__o;
    break;
  case e_4th:
    __d = obj.__d;
    break;
  case e_5th:
  default:
    __ms = obj.__ms;
    break;
  }
  return *this;
}

// AnySeq methods
Marshal::AnySeq::AnySeq (CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(0)
{
  if (this->_maximum > 0){
    this->_buffer = new CORBA_Any [this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

Marshal::AnySeq::AnySeq (CORBA_ULong max, CORBA_ULong length,
				  CORBA_Any *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

Marshal::AnySeq::~AnySeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != 0)
    delete this->_buffer;
}

void Marshal::AnySeq::length (CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    CORBA_Any *temp = new CORBA_Any [newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof (CORBA_Any)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(CORBA_Any));
  this->_length = newlen;
}

CORBA_ULong Marshal::AnySeq::length() const
{
  return this->_length;
}

CORBA_Any &Marshal::AnySeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const CORBA_Any& Marshal::AnySeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}


/***************** TypeCodes **************/
// In the typecode, all repoIDs and names are omitted by providing their length
// as 1 that stands for the NULL terminator

// encapsulated parameters for struct Marshal_Struct typecode
static const CORBA_Long _oc_Marshal_Struct [] = {
  MY_BYTE_SEX,   // byte order
  1, 0, // repoID omitted (16, "IDL:Marshal/Marshal:1.0")
  1, 0, // struct name (15, "Marshal_Struct") omitted
  5,     // 5 struct elements

  // short
  1, 0,  // should be 2, "s",  
  tk_short,

  // long
  1, 0, // should be 2, "l",
  tk_long,

  // char
  1, 0, // should be 2, "c",
  tk_char,

  // octet
  1, 0, // should be 2, "o",
  tk_octet,

  // double
  1, 0, // should be 2, "d",
  tk_double,
};


static CORBA_TypeCode _tc__tc_Marshal_Struct (tk_struct, 
					      sizeof _oc_Marshal_Struct, 
					      (unsigned char *) &_oc_Marshal_Struct,
					      0);
CORBA_TypeCode_ptr Marshal::_tc_Marshal_Struct = &_tc__tc_Marshal_Struct;


static const CORBA_Long _oc_discrim [] = {
  MY_BYTE_SEX,          // byte order flag (TRICKY)
  1, 0, // should be 20, "IDL:Marshal/discrim",
  1, 0, // should be   8, "discrim",   
  7,          // 7 elements in the enum
  
  1, 0, // 6, "e_0th",       // omitted member name, "e_0th"
  1, 0, // 6, "e_1st",       // omitted member name, "e_1st"
  1, 0, // 6, "e_2nd",       // omitted member name, "e_2nd"
  1, 0, // 6, "e_3rd",       // omitted member name, "e_3rd" 
  1, 0, // 6, "e_4th",       // omitted member name, "e_4th" 
  1, 0, // 6, "e_5th",       // omitted member name, "e_5th"
  1, 0  //6, "e_6th"       // omitted member name, "e_6th"
};

static CORBA_TypeCode _tc__tc_discrim (tk_enum, 
				       sizeof _oc_discrim, 
				       (unsigned char *) &_oc_discrim,
				       0);

CORBA_TypeCode_ptr Marshal::_tc_discrim = &_tc__tc_discrim;

// Union
static const CORBA_Long _oc_Marshal_Union [] = 
{
   MY_BYTE_SEX,          // byte order flag (TRICKY)
   1, 0, // 26, "IDL:Marshal/Marshal_Union",       // omitted repository/type ID
   1, 0, // 14, "Marshal_Union",       // union name
   
   //
   // discriminant typecode:
   //

   tk_enum,    // tk_enum
   80,         // encapsulation length
     MY_BYTE_SEX,
     1, 0,         // repoID
     1, 0,         // name
     7,                  // 7 elements in the enum
   
     1, 0,       // e_Oth
     1, 0,
     1, 0,
     1, 0,
     1, 0,
     1, 0,
     1, 0,       // e_6th

   5,       	// default member index (zero based)
   6,       	// number of union members
   
   // the 1st union branch arm
   Marshal::e_0th,       // member label value
   1, 0,       // omitted member name, "s"
   tk_short,   // member typecode
   
   // the 2nd union branch arm
   Marshal::e_1st,      // member label value
   1, 0,       // omitted member name, "l"
   tk_long,    // member typecode

   // the 3rd union branch arm
   Marshal::e_2nd,      // member label value
   1, 0,        // omitted member name, "c"
   tk_char,     // member typecode
   
   // the 4th union branch arm
   Marshal::e_3rd,      // member label value
   1, 0,        // omitted member name, "o"
   tk_octet,    // member typecode
   
   // the 5th union branch arm
   Marshal::e_4th,      // member label value
   1, 0,       // omitted member name, "d"
   tk_double,

   // the 6th union branch
   Marshal::e_5th, // member label value
   1, 0,           // omitted member name, "ms"
   tk_struct,  // tk_struct
   84,  // encapsulation length
   
     MY_BYTE_SEX,   // byte order
     1, 0, // repoID omitted (16, "IDL:Marshal/Marshal:1.0")
     1, 0, // struct name (15, "Marshal_Struct") omitted
     5,     // 5 struct elements

     // short
     1, 0,  // should be 2, "s",  
     tk_short,

     // long
     1, 0, // should be 2, "l",
     tk_long,

     // char
     1, 0, // should be 2, "c",
     tk_char,

     // octet
     1, 0, // should be 2, "o",
     tk_octet,

     // double
     1, 0, // should be 2, "d",
     tk_double,
 
   // default case
   6,    // 7th member, default index val
   1, 0, // struct name "ms"
   ~0,   // indirected typecode
   -108  // has to point to tk_struct in this stream
};

static CORBA_TypeCode _tc__tc_Marshal_Union (tk_union, 
					     sizeof _oc_Marshal_Union, 
					     (unsigned char *) &_oc_Marshal_Union,
					     0);

CORBA_TypeCode_ptr Marshal::_tc_Marshal_Union = &_tc__tc_Marshal_Union;

// AnySeq
static const CORBA_Long _oc_AnySeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_any,  // sequence of any
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_AnySeq (tk_sequence,
					      sizeof
					      _oc_AnySeq, 
					      (unsigned char *)
					      &_oc_AnySeq,
					      0);

CORBA_TypeCode_ptr Marshal::_tc_AnySeq = &_tc__tc_AnySeq; 

/***************** All the methods ********************/
// test_short
static const TAO_Param_Data marshal_test_short_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {_tc_CORBA_Short, PARAM_IN, 0},
  {_tc_CORBA_Short, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_short_calldata = {
  "test_short",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_short_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_short (CORBA_Short s1, CORBA_Short &s2, CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_short_calldata, 0, &s1, &s2);
}

// test_long
static const TAO_Param_Data marshal_test_long_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {_tc_CORBA_Long, PARAM_IN, 0},
  {_tc_CORBA_Long, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_long_calldata = {
  "test_long",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_long_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_long (CORBA_Long l1, CORBA_Long &l2, CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_long_calldata, 0, &l1, &l2);
}

// test_octet
static const TAO_Param_Data marshal_test_octet_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {_tc_CORBA_Octet, PARAM_IN, 0},
  {_tc_CORBA_Octet, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_octet_calldata = {
  "test_octet",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_octet_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_octet (CORBA_Octet o1, CORBA_Octet &o2, CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_octet_calldata, 0, &o1, &o2);
}

// test_char
static const TAO_Param_Data marshal_test_char_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {_tc_CORBA_Char, PARAM_IN, 0},
  {_tc_CORBA_Char, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_char_calldata = {
  "test_char",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_char_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_char (CORBA_Char c1, CORBA_Char &c2, CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_char_calldata, 0, &c1, &c2);
}

// test_double
static const TAO_Param_Data marshal_test_double_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {_tc_CORBA_Double, PARAM_IN, 0},
  {_tc_CORBA_Double, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_double_calldata = {
  "test_double",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_double_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_double (CORBA_Double d1, CORBA_Double &d2, CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call (env, &marshal_test_double_calldata, 0, &d1, &d2);
}

// test_struct
static const TAO_Param_Data marshal_test_struct_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {Marshal::_tc_Marshal_Struct, PARAM_IN, 0},
  {Marshal::_tc_Marshal_Struct, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_struct_calldata = {
  "test_struct",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_struct_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_struct (const Marshal::Marshal_Struct &ms1, Marshal::Marshal_Struct
			   &ms2, CORBA_Environment &env) 
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_struct_calldata, 0, &ms1, &ms2);
}

// test_union
static const TAO_Param_Data marshal_test_union_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {Marshal::_tc_Marshal_Union, PARAM_IN, 0},
  {Marshal::_tc_Marshal_Union, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_union_calldata = {
  "test_union",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_union_paramdata [0], //
  0, 0
};

// The stub
void Marshal::test_union (const Marshal_Union &u1, Marshal::Marshal_Union
			   &u2, CORBA_Environment &env) 
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call (env, &marshal_test_union_calldata, 0, &u1, &u2);
}

// test_any
static const TAO_Param_Data marshal_test_any_paramdata [] = {
  {_tc_CORBA_Void, PARAM_RETURN, 0},
  {_tc_CORBA_Any, PARAM_IN, 0},
  {_tc_CORBA_Any, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_any_calldata = {
  "test_any",  // op_name
  1,  // twoway, although it returns void
  3,             // param count
  & marshal_test_any_paramdata[0], //
  0, 0
};

// The stub
void Marshal::test_any (const CORBA_Any &a1, CORBA_Any *&a2, CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_any_calldata, 0, &a1, &a2);
}

// test_sequence
static const TAO_Param_Data marshal_test_sequence_paramdata [] = {
  { _tc_CORBA_Void, PARAM_RETURN, 0},
  { Marshal::_tc_AnySeq, PARAM_IN, 0},
  { Marshal::_tc_AnySeq, PARAM_OUT, 0}
};

static const TAO_Call_Data marshal_test_sequence_calldata = {
  "test_sequence",  // opname
  1,   // twoway
  2,               // param count
  &marshal_test_sequence_paramdata [0],
  0, 0
};

// stub
void Marshal::test_sequence (const Marshal::AnySeq &as1, Marshal::AnySeq *&as2,
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
  // twoway
  istub->do_call(env, &marshal_test_sequence_calldata, 0, &as1, &as2);
}


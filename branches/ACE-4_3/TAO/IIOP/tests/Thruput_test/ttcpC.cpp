#include <stdio.h>
#include "ttcpC.h"

// constructor
ttcp_sequence::ttcp_sequence(STUB_Object *refdata)
  : CORBA_Object(refdata)
{
}

// static duplicate method
ttcp_sequence_ptr ttcp_sequence::_duplicate(ttcp_sequence_ptr obj)
{
  void *dupl;

  if (obj)
    {
      obj->AddRef();
    }
  return obj;
}

// static _narrow method
ttcp_sequence_ptr ttcp_sequence::_narrow(CORBA_Object_ptr obj)
{
  CORBA_Environment env;
  if (obj->_is_a("IDL:ttcp_sequence:1.0", env)){
    STUB_Object *istub;
    if (obj->QueryInterface(IID_STUB_Object, (void **)&istub) != NOERROR){
      return ttcp_sequence::_nil();
    }
    ttcp_sequence_ptr new_obj = new ttcp_sequence(istub);
    return ttcp_sequence::_duplicate(new_obj);
  }
  return ttcp_sequence::_nil();
}

ttcp_sequence_ptr ttcp_sequence::_nil()
{
  return (ttcp_sequence_ptr)NULL;
}

ttcp_sequence::ShortSeq::ShortSeq()
  : _maximum(0),
    _length(0),
    _buffer(0),
    _release(CORBA_B_FALSE)
{
}

ttcp_sequence::ShortSeq::ShortSeq(CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(CORBA_B_FALSE)
{
  if (this->_maximum > 0){
    this->_buffer = new CORBA_Short[this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

ttcp_sequence::ShortSeq::ShortSeq(CORBA_ULong max, CORBA_ULong length,
				  CORBA_Short *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

ttcp_sequence::ShortSeq::~ShortSeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != CORBA_B_FALSE)
    delete this->_buffer;
}

void ttcp_sequence::ShortSeq::length(CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    CORBA_Short *temp = new CORBA_Short[newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof(CORBA_Short)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(CORBA_Short));
  this->_length = newlen;
}

CORBA_ULong ttcp_sequence::ShortSeq::length() const
{
  return this->_length;
}

CORBA_Short& ttcp_sequence::ShortSeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const CORBA_Short& ttcp_sequence::ShortSeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

// LongSeq
ttcp_sequence::LongSeq::LongSeq()
  : _maximum(0),
    _length(0),
    _buffer(0),
    _release(CORBA_B_FALSE)
{
}

ttcp_sequence::LongSeq::LongSeq(CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(CORBA_B_FALSE)
{
  if (this->_maximum > 0){
    this->_buffer = new CORBA_Long[this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

ttcp_sequence::LongSeq::LongSeq(CORBA_ULong max, CORBA_ULong length,
				  CORBA_Long *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

ttcp_sequence::LongSeq::~LongSeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != CORBA_B_FALSE)
    delete this->_buffer;
}

void ttcp_sequence::LongSeq::length(CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    CORBA_Long *temp = new CORBA_Long[newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof(CORBA_Long)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(CORBA_Long));
  this->_length = newlen;
}

CORBA_ULong ttcp_sequence::LongSeq::length() const
{
  return this->_length;
}

CORBA_Long& ttcp_sequence::LongSeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const CORBA_Long& ttcp_sequence::LongSeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

// DoubleSeq
ttcp_sequence::DoubleSeq::DoubleSeq()
  : _maximum(0),
    _length(0),
    _buffer(0),
    _release(CORBA_B_FALSE)
{
}

ttcp_sequence::DoubleSeq::DoubleSeq(CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(CORBA_B_FALSE)
{
  if (this->_maximum > 0){
    this->_buffer = new CORBA_Double[this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

ttcp_sequence::DoubleSeq::DoubleSeq(CORBA_ULong max, CORBA_ULong length,
				  CORBA_Double *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

ttcp_sequence::DoubleSeq::~DoubleSeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != CORBA_B_FALSE)
    delete this->_buffer;
}

void ttcp_sequence::DoubleSeq::length(CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    CORBA_Double *temp = new CORBA_Double[newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof(CORBA_Double)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(CORBA_Double));
  this->_length = newlen;
}

CORBA_ULong ttcp_sequence::DoubleSeq::length() const
{
  return this->_length;
}

CORBA_Double& ttcp_sequence::DoubleSeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const CORBA_Double& ttcp_sequence::DoubleSeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

// Char seq
ttcp_sequence::CharSeq::CharSeq()
  : _maximum(0),
    _length(0),
    _buffer(0),
    _release(CORBA_B_FALSE)
{
}

ttcp_sequence::CharSeq::CharSeq(CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(CORBA_B_FALSE)
{
  if (this->_maximum > 0){
    this->_buffer = new CORBA_Char[this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

ttcp_sequence::CharSeq::CharSeq(CORBA_ULong max, CORBA_ULong length,
				  CORBA_Char *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

ttcp_sequence::CharSeq::~CharSeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != CORBA_B_FALSE)
    delete this->_buffer;
}

void ttcp_sequence::CharSeq::length(CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    CORBA_Char *temp = new CORBA_Char[newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof(CORBA_Char)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(CORBA_Char));
  this->_length = newlen;
}

CORBA_ULong ttcp_sequence::CharSeq::length() const
{
  return this->_length;
}

CORBA_Char& ttcp_sequence::CharSeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const CORBA_Char& ttcp_sequence::CharSeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

//Octet Seq
ttcp_sequence::OctetSeq::OctetSeq()
  : _maximum(0),
    _length(0),
    _buffer(0),
    _release(CORBA_B_FALSE)
{
}

ttcp_sequence::OctetSeq::OctetSeq(CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(CORBA_B_FALSE)
{
  if (this->_maximum > 0){
    this->_buffer = new CORBA_Octet[this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

ttcp_sequence::OctetSeq::OctetSeq(CORBA_ULong max, CORBA_ULong length,
				  CORBA_Octet *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

ttcp_sequence::OctetSeq::~OctetSeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != CORBA_B_FALSE)
    delete this->_buffer;
}

void ttcp_sequence::OctetSeq::length(CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    CORBA_Octet *temp = new CORBA_Octet[newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof(CORBA_Octet)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(CORBA_Octet));
  this->_length = newlen;
}

CORBA_ULong ttcp_sequence::OctetSeq::length() const
{
  return this->_length;
}

CORBA_Octet& ttcp_sequence::OctetSeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const CORBA_Octet& ttcp_sequence::OctetSeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}


//struct seq
ttcp_sequence::StructSeq::StructSeq()
  : _maximum(0),
    _length(0),
    _buffer(0),
    _release(CORBA_B_FALSE)
{
}

ttcp_sequence::StructSeq::StructSeq(CORBA_ULong max)
  : _maximum(max),
    _length(0),
    _release(CORBA_B_FALSE)
{
  if (this->_maximum > 0){
    this->_buffer = new PerfStruct[this->_maximum];
  } else {
    this->_buffer = 0;
  }
}

ttcp_sequence::StructSeq::StructSeq(CORBA_ULong max, CORBA_ULong length,
				  PerfStruct *value, CORBA_Boolean release)
  : _maximum(max),
    _length(length),
    _buffer(value),
    _release(release)
{
}

ttcp_sequence::StructSeq::~StructSeq()
{
  this->_maximum = this->_length = 0;
  if (this->_release != CORBA_B_FALSE)
    delete this->_buffer;
}

void ttcp_sequence::StructSeq::length(CORBA_ULong newlen)
{
  if (newlen > this->_maximum) {
    PerfStruct *temp = new PerfStruct[newlen];
    ::memcpy(temp, this->_buffer, (size_t(this->_length) * sizeof(PerfStruct)));
    if (this->_release)
      delete this->_buffer;
    else
      this->_release = 1;

    this->_maximum = newlen;
    this->_buffer = temp;
  }
  if (newlen > this->_length)
    memset(&this->_buffer[this->_length], 0, size_t(newlen - this->_length) *
	   sizeof(PerfStruct));
  this->_length = newlen;
}

CORBA_ULong ttcp_sequence::StructSeq::length() const
{
  return this->_length;
}

PerfStruct& ttcp_sequence::StructSeq::operator[](CORBA_ULong index)
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}

const PerfStruct& ttcp_sequence::StructSeq::operator[](CORBA_ULong index) const
{
        if (index >= this->_length)
	  exit(1);
        return this->_buffer[index];
}


/***************** TypeCodes **************/

// encapsulated parameters for struct PerfStruct typecode
static const CORBA_Long _oc_PerfStruct [] = {
  // strings are encoded as length followed by the string. A length of 1 stands
  // for the NULL character
  MY_BYTE_SEX,   // byte order
  1, 0,  // empty string, repoID
  1, 0,  // empty string, struct name
  6,     // 6 struct elements

  // short
  1, 0,  // empty string
  tk_short,

  // char
  1, 0,
  tk_char,

  // long
  1, 0,
  tk_long,

  // octet
  1, 0,
  tk_octet,

  // double
  1, 0,
  tk_double,

  // padding
  1, 0,
  tk_array,
  12, // encapsulation length
    MY_BYTE_SEX,
    tk_octet,
    8
};


static CORBA_TypeCode _tc__tc_PerfStruct(tk_struct, 
					  sizeof _oc_PerfStruct, 
					  (unsigned char *) &_oc_PerfStruct,
					  CORBA_B_FALSE);
CORBA_TypeCode_ptr _tc_PerfStruct = &_tc__tc_PerfStruct;

// Typecodes for all the sequences

// StructSeq
static const CORBA_Long _oc_ttcp_sequence_StructSeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_struct,  // sequence of struct
  112,  // encapsulaton length
    MY_BYTE_SEX,   // byte order
    1, 0,  // empty string, repoID
    1, 0,  // empty string, struct name
    6,     // 6 struct elements

    // short
    1, 0,  // empty string
    tk_short,

    // char
    1, 0,
    tk_char,

    // long
    1, 0,
    tk_long,

    // octet
    1, 0,
    tk_octet,

    // double
    1, 0,
    tk_double,

    // padding
    1, 0,
    tk_array, // array
    12, //encapsulation length
      MY_BYTE_SEX,
      tk_octet,
      8,
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_ttcp_sequence_StructSeq(tk_sequence,
						      sizeof
						      _oc_ttcp_sequence_StructSeq, 
						      (unsigned char *)
						      &_oc_ttcp_sequence_StructSeq,
						      CORBA_B_FALSE);
CORBA_TypeCode_ptr ttcp_sequence::_tc_StructSeq = &_tc__tc_ttcp_sequence_StructSeq; 

// ShortSeq
static const CORBA_Long _oc_ttcp_sequence_ShortSeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_short,  // sequence of short
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_ttcp_sequence_ShortSeq(tk_sequence,
						      sizeof
						      _oc_ttcp_sequence_ShortSeq, 
						      (unsigned char *)
						      &_oc_ttcp_sequence_ShortSeq, 
						      CORBA_B_FALSE);
CORBA_TypeCode_ptr ttcp_sequence::_tc_ShortSeq = &_tc__tc_ttcp_sequence_ShortSeq; 

// LongSeq
static const CORBA_Long _oc_ttcp_sequence_LongSeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_long,  // sequence of long
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_ttcp_sequence_LongSeq(tk_sequence,
						      sizeof
						      _oc_ttcp_sequence_LongSeq, 
						      (unsigned char *)
						      &_oc_ttcp_sequence_LongSeq, 
						      CORBA_B_FALSE);
CORBA_TypeCode_ptr ttcp_sequence::_tc_LongSeq = &_tc__tc_ttcp_sequence_LongSeq; 

// DoubleSeq
static const CORBA_Long _oc_ttcp_sequence_DoubleSeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_double,  // sequence of doubles
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_ttcp_sequence_DoubleSeq(tk_sequence,
						      sizeof
						      _oc_ttcp_sequence_DoubleSeq, 
						      (unsigned char *)
						      &_oc_ttcp_sequence_DoubleSeq, 
						      CORBA_B_FALSE);
CORBA_TypeCode_ptr ttcp_sequence::_tc_DoubleSeq = &_tc__tc_ttcp_sequence_DoubleSeq;

// CharSeq
static const CORBA_Long _oc_ttcp_sequence_CharSeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_char,  // sequence of chars
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_ttcp_sequence_CharSeq(tk_sequence,
						      sizeof
						      _oc_ttcp_sequence_CharSeq, 
						      (unsigned char *)
						      &_oc_ttcp_sequence_CharSeq, 
						      CORBA_B_FALSE);
CORBA_TypeCode_ptr ttcp_sequence::_tc_CharSeq = &_tc__tc_ttcp_sequence_CharSeq;

//OctetSeq
static const CORBA_Long _oc_ttcp_sequence_OctetSeq [] = {
  MY_BYTE_SEX,  // byte order
  tk_octet,  // sequence of octets
  0,   // a length of 0 => unbounded sequence
};
static CORBA_TypeCode _tc__tc_ttcp_sequence_OctetSeq(tk_sequence,
						      sizeof
						      _oc_ttcp_sequence_OctetSeq, 
						      (unsigned char *)
						      &_oc_ttcp_sequence_OctetSeq, 
						      CORBA_B_FALSE);
CORBA_TypeCode_ptr ttcp_sequence::_tc_OctetSeq = &_tc__tc_ttcp_sequence_OctetSeq;



/***************** All the methods ********************/

//sendShortSeq
static const TAO_Param_Data ttcp_sequence_sendShortSeq_params [] = {
#ifndef ONEWAY
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  },
#endif
  { ttcp_sequence::_tc_ShortSeq,
    PARAM_IN,
    0
  }
};

static const TAO_Call_Data ttcp_sequence_sendShortSeq_TAO_Call_Data = {
  "sendShortSeq",  // opname
#ifndef ONEWAY
  CORBA_B_TRUE,   // twoway
  2,               // param count
#else
  CORBA_B_FALSE,  // oneway
  1,
#endif
  &ttcp_sequence_sendShortSeq_params [0],
  0, 0
};

// stub
void ttcp_sequence::sendShortSeq(const ttcp_sequence::ShortSeq &ttcp_seq, 
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_sendShortSeq_TAO_Call_Data, 0, &ttcp_seq);
#else
  istub->do_call(env, &ttcp_sequence_sendShortSeq_TAO_Call_Data, &ttcp_seq);
#endif
}

//sendLongSeq
static const TAO_Param_Data ttcp_sequence_sendLongSeq_params [] = {
#ifndef ONEWAY
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  },
#endif
  { ttcp_sequence::_tc_LongSeq,
    PARAM_IN,
    0
  }
};

static const TAO_Call_Data ttcp_sequence_sendLongSeq_TAO_Call_Data = {
  "sendLongSeq",  // opname
#ifndef ONEWAY
  CORBA_B_TRUE,   // twoway
  2,               // param count
#else
  CORBA_B_FALSE,  // oneway
  1,
#endif
  &ttcp_sequence_sendLongSeq_params [0],
  0, 0
};

// stub
void ttcp_sequence::sendLongSeq(const ttcp_sequence::LongSeq &ttcp_seq, 
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_sendLongSeq_TAO_Call_Data, 0, &ttcp_seq);
#else
  istub->do_call(env, &ttcp_sequence_sendLongSeq_TAO_Call_Data, &ttcp_seq);
#endif
}

//sendDoubleSeq
static const TAO_Param_Data ttcp_sequence_sendDoubleSeq_params [] = {
#ifndef ONEWAY
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  },
#endif
  { ttcp_sequence::_tc_DoubleSeq,
    PARAM_IN,
    0
  }
};

static const TAO_Call_Data ttcp_sequence_sendDoubleSeq_TAO_Call_Data = {
  "sendDoubleSeq",  // opname
#ifndef ONEWAY
  CORBA_B_TRUE,   // twoway
  2,               // param count
#else
  CORBA_B_FALSE,  // oneway
  1,
#endif
  &ttcp_sequence_sendDoubleSeq_params [0],
  0, 0
};

// stub
void ttcp_sequence::sendDoubleSeq(const ttcp_sequence::DoubleSeq &ttcp_seq, 
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_sendDoubleSeq_TAO_Call_Data, 0, &ttcp_seq);
#else
  istub->do_call(env, &ttcp_sequence_sendDoubleSeq_TAO_Call_Data, &ttcp_seq);
#endif
}

//sendCharSeq
static const TAO_Param_Data ttcp_sequence_sendCharSeq_params [] = {
#ifndef ONEWAY
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  },
#endif
  { ttcp_sequence::_tc_CharSeq,
    PARAM_IN,
    0
  }
};

static const TAO_Call_Data ttcp_sequence_sendCharSeq_TAO_Call_Data = {
  "sendCharSeq",  // opname
#ifndef ONEWAY
  CORBA_B_TRUE,   // twoway
  2,               // param count
#else
  CORBA_B_FALSE,  // oneway
  1,
#endif
  &ttcp_sequence_sendCharSeq_params [0],
  0, 0
};

// stub
void ttcp_sequence::sendCharSeq(const ttcp_sequence::CharSeq &ttcp_seq, 
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_sendCharSeq_TAO_Call_Data, 0, &ttcp_seq);
#else
  istub->do_call(env, &ttcp_sequence_sendCharSeq_TAO_Call_Data, &ttcp_seq);
#endif
}

//sendOctetSeq
static const TAO_Param_Data ttcp_sequence_sendOctetSeq_params [] = {
#ifndef ONEWAY
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  },
#endif
  { ttcp_sequence::_tc_OctetSeq,
    PARAM_IN,
    0
  }
};

static const TAO_Call_Data ttcp_sequence_sendOctetSeq_TAO_Call_Data = {
  "sendOctetSeq",  // opname
#ifndef ONEWAY
  CORBA_B_TRUE,   // twoway
  2,               // param count
#else
  CORBA_B_FALSE,  // oneway
  1,
#endif
  &ttcp_sequence_sendOctetSeq_params [0],
  0, 0
};

// stub
void ttcp_sequence::sendOctetSeq(const ttcp_sequence::OctetSeq &ttcp_seq, 
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_sendOctetSeq_TAO_Call_Data, 0, &ttcp_seq);
#else
  istub->do_call(env, &ttcp_sequence_sendOctetSeq_TAO_Call_Data, &ttcp_seq);
#endif
}

//sendStructSeq
static const TAO_Param_Data ttcp_sequence_sendStructSeq_params [] = {
#ifndef ONEWAY
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  },
#endif
  { ttcp_sequence::_tc_StructSeq,
    PARAM_IN,
    0
  }
};

static const TAO_Call_Data ttcp_sequence_sendStructSeq_TAO_Call_Data = {
  "sendStructSeq",  // opname
#ifndef ONEWAY
  CORBA_B_TRUE,   // twoway
  2,               // param count
#else
  CORBA_B_FALSE,  // oneway
  1,
#endif
  &ttcp_sequence_sendStructSeq_params [0],
  0, 0
};

// stub
void ttcp_sequence::sendStructSeq(const ttcp_sequence::StructSeq &ttcp_seq, 
				 CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_sendStructSeq_TAO_Call_Data, 0, &ttcp_seq);
#else
  istub->do_call(env, &ttcp_sequence_sendStructSeq_TAO_Call_Data, &ttcp_seq);
#endif
}

// start timer
#ifndef ONEWAY
static const TAO_Param_Data ttcp_sequence_start_timer_params [] = {
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  }
};
#endif

static const TAO_Call_Data ttcp_sequence_start_timer_TAO_Call_Data = {
  "start_timer", 
#ifndef ONEWAY
  CORBA_B_TRUE,  // twoway
  1,
  &ttcp_sequence_start_timer_params [0],
#else
  CORBA_B_FALSE,  // oneway
  0,
  0,
#endif
  0, 0
};

//stub
void ttcp_sequence::start_timer(CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_start_timer_TAO_Call_Data);
#else
  istub->do_call(env, &ttcp_sequence_start_timer_TAO_Call_Data);
#endif
}

// stop timer
#ifndef ONEWAY
static const TAO_Param_Data ttcp_sequence_stop_timer_params [] = {
  { _tc_CORBA_Void,
    PARAM_RETURN,
    0
  }
};
#endif

static const TAO_Call_Data ttcp_sequence_stop_timer_TAO_Call_Data = {
  "stop_timer", 
#ifndef ONEWAY
  CORBA_B_TRUE,
  1,
  &ttcp_sequence_stop_timer_params[0],
#else
  CORBA_B_FALSE,  // oneway
  0,
  0,
#endif
  0, 0
};

//stub
void ttcp_sequence::stop_timer(CORBA_Environment &env)
{
  STUB_Object *istub;
  if (this->QueryInterface (IID_STUB_Object, (void **) &istub) != NOERROR){
    env.exception (new CORBA_DATA_CONVERSION (COMPLETED_NO));
    return;
  }
#ifndef ONEWAY
  // twoway
  istub->do_call(env, &ttcp_sequence_stop_timer_TAO_Call_Data);
#else
  istub->do_call(env, &ttcp_sequence_stop_timer_TAO_Call_Data);
#endif
}

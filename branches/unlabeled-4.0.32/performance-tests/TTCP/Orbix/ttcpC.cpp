
// @(#)ttcpC.cpp	1.1	10/18/96

#include "ttcp.hh"


#ifndef _IDL_SEQUENCE_char_Ops
#define _IDL_SEQUENCE_char_Ops

_IDL_SEQUENCE_char &_IDL_SEQUENCE_char:: operator= (const IONANC__IDL_SEQUENCE_char& IT_p) {
    this->operator= (*(_IDL_SEQUENCE_char*) &IT_p);
    return (*this);
}

_IDL_SEQUENCE_char:: operator IONANC__IDL_SEQUENCE_char () {
    IONANC__IDL_SEQUENCE_char tmp;
    memset (&tmp, 0, sizeof(tmp));
    ((_IDL_SEQUENCE_char *) &tmp)->operator= (*this);
    return tmp;
}

_IDL_SEQUENCE_char:: operator const IONANC__IDL_SEQUENCE_char () const {
    IONANC__IDL_SEQUENCE_char tmp;
    memset (&tmp, 0, sizeof(tmp));
    ((_IDL_SEQUENCE_char *) &tmp)->operator= (*this);
    return tmp;
}

_IDL_SEQUENCE_char& _IDL_SEQUENCE_char:: operator= (const _IDL_SEQUENCE_char &IT_s){
   if (this == &IT_s) return *this;
   if (_buffer) delete [] _buffer;
   _length = IT_s._length;
   _maximum = IT_s._maximum;
    if (_maximum) {
       _buffer = new char [ _maximum ];
      for (unsigned long IT_j=0; IT_j < _length; IT_j++)
        _buffer [IT_j] = IT_s._buffer[IT_j];
   }
    else
        _buffer = NULL;
   return *this;
}

_IDL_SEQUENCE_char:: _IDL_SEQUENCE_char (unsigned long IT_size) {
    if (IT_size)
        _buffer = new char [ IT_size ];
    else
        _buffer = NULL;
    _length = 0;
    _maximum = IT_size;
    }

_IDL_SEQUENCE_char:: _IDL_SEQUENCE_char (const _IDL_SEQUENCE_char &IT_s) {
   _length = IT_s._length;
   _maximum = IT_s._maximum;
    if (_maximum) {
        _buffer = new char [ _maximum ];
       for (unsigned long IT_j=0; IT_j < _length; IT_j++)
        _buffer [IT_j] = IT_s._buffer[IT_j];
    }
    else
    _buffer = NULL;
}

void _IDL_SEQUENCE_char:: decodeInOutOp (CORBA::Request &IT_r) {
    unsigned long IT_max;
    IT_r >> IT_max;
    if (IT_max > _maximum)
        IT_r.makeRuntimeException3 ();
    else {
        IT_r >> _length;
        if (_length > _maximum)
            IT_r.makeRuntimeException4 ();    
        else
    if (_maximum) {
        char* IT_arr = (char*) _buffer;
        IT_r.decodeCharArray (IT_arr, _length);
    }
    }
}

void _IDL_SEQUENCE_char:: encodeOp (CORBA::Request &IT_r) const {
    IT_r << _maximum;
    IT_r << _length;
if (_maximum) {
    char* IT_arr = (char*) _buffer;
    IT_r.encodeCharArray (IT_arr, _length);
}
}

void _IDL_SEQUENCE_char:: decodeOp (CORBA::Request &IT_r) {
    IT_r >> _maximum;
    IT_r >> _length;
    if (_maximum) {
          _buffer = new char [ _maximum ];
        {
            char* IT_arr = (char*) _buffer;
            IT_r.decodeCharArray (IT_arr, _length);
        }
    }
    else
      _buffer = NULL;
}

char &IONANC__IDL_SEQUENCE_char:: operator [](unsigned long IT_i) const {
   return  _buffer[IT_i];
}

IONANC__IDL_SEQUENCE_char:: operator _IDL_SEQUENCE_char () {
    return (*((_IDL_SEQUENCE_char *) this));
}

IONANC__IDL_SEQUENCE_char:: operator const _IDL_SEQUENCE_char () const {
    return (*((const _IDL_SEQUENCE_char *) this));
}


#endif

ttcp_sequence::ttcp_sequence (char *IT_OR) {
      m_pptr = new ttcp_sequence_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef ttcp_sequenceForwC
#define ttcp_sequenceForwC
CORBA::ObjectRef ttcp_sequence_getBase(void *IT_p){
    return (ttcp_sequence*)IT_p;}

void ttcp_sequence_release (void *IT_p, CORBA::Environment &IT_env) {
    ((ttcp_sequence*)IT_p)->_release(IT_env);}

ttcp_sequence* ttcp_sequence_duplicate (void *IT_p, CORBA::Environment &IT_env) {
    return ((ttcp_sequence*)IT_p)->_duplicate(IT_env); }
#endif



ttcp_sequence* ttcp_sequence:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       ttcp_sequence*IT_p =
 (ttcp_sequence*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		ttcp_sequence_IMPL, ttcp_sequence_IR);
       return IT_p ? IT_p->_duplicate () : NULL; }



ttcp_sequence* ttcp_sequence:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


ttcp_sequence* ttcp_sequence:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
ttcp_sequence* ttcp_sequence::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_env) {
       ttcp_sequence* IT_p = (ttcp_sequence*)CORBA::Object::_castDown (IT_obj, ttcp_sequence_IR, IT_env);
        return IT_p ? IT_p->_duplicate(IT_env) : NULL;
   }

void* ttcp_sequenceProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new ttcp_sequence(IT_OR);}

void* ttcp_sequenceProxyFactoryClass::New2 () {
        return  new ttcp_sequence();}

void* ttcp_sequenceProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,ttcp_sequence_IR))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((ttcp_sequence*)IT_p),IT_s))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* ttcp_sequenceProxyFactoryClass::pptr (void *IT_p) {
       return ((ttcp_sequence*)IT_p)->_pptr ();}

void ttcp_sequenceProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, ttcp_sequence_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

long ttcp_sequence:: send(const ttcp_sequence::my_sequence& ttcp_seq, CORBA::Environment &IT_env) {

    if (IT_env || m_isNull) return 0;
    CORBA::Request IT_r (this, "send",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {
        ttcp_seq.encodeOp (IT_r);
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        long IT_result;
        IT_r >> IT_result;
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    return 0;
}

void ttcp_sequence:: start_timer(CORBA::Environment &IT_env) {

    if (IT_env || m_isNull) return ;
    CORBA::Request IT_r (this, "start_timer",IT_env,1,1);

    IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
}

void ttcp_sequence:: stop_timer(CORBA::Environment &IT_env) {

    if (IT_env || m_isNull) return ;
    CORBA::Request IT_r (this, "stop_timer",IT_env,1,1);

    IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
}


ttcp_sequenceProxyFactoryClass ttcp_sequenceProxyFactory(1);


#ifndef ttcp_sequence_dispatch_impl

unsigned char ttcp_sequence_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("ttcp_sequence");
      return 0;
}
 
#endif

ttcp_string::ttcp_string (char *IT_OR) {
      m_pptr = new ttcp_string_dispatch (IT_OR, this,(CORBA::Object*)this);
} 

#ifndef ttcp_stringForwC
#define ttcp_stringForwC
CORBA::ObjectRef ttcp_string_getBase(void *IT_p){
    return (ttcp_string*)IT_p;}

void ttcp_string_release (void *IT_p, CORBA::Environment &IT_env) {
    ((ttcp_string*)IT_p)->_release(IT_env);}

ttcp_string* ttcp_string_duplicate (void *IT_p, CORBA::Environment &IT_env) {
    return ((ttcp_string*)IT_p)->_duplicate(IT_env); }
#endif



ttcp_string* ttcp_string:: _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env) {
       ttcp_string*IT_p =
 (ttcp_string*)CORBA::Factory.New (IT_markerServer, IT_env, IT_c, host, 
		ttcp_string_IMPL, ttcp_string_IR);
       return IT_p ? IT_p->_duplicate () : NULL; }



ttcp_string* ttcp_string:: _bind (CORBA::Environment &IT_env) {
       return _bind (NULL,NULL,CORBA::Context(), IT_env); }


ttcp_string* ttcp_string:: _bind (const char* IT_markerServer, const char* host,
                CORBA::Environment &IT_env) {
       return _bind (IT_markerServer, host, CORBA::Context (), IT_env); }
ttcp_string* ttcp_string::_narrow (CORBA::Object* IT_obj, CORBA::Environment &IT_env) {
       ttcp_string* IT_p = (ttcp_string*)CORBA::Object::_castDown (IT_obj, ttcp_string_IR, IT_env);
        return IT_p ? IT_p->_duplicate(IT_env) : NULL;
   }

void* ttcp_stringProxyFactoryClass::New (char *IT_OR, CORBA::Environment&) {
        return  new ttcp_string(IT_OR);}

void* ttcp_stringProxyFactoryClass::New2 () {
        return  new ttcp_string();}

void* ttcp_stringProxyFactoryClass::IT_castUp (void *IT_p, char* IT_s) {
      void *IT_l;
      if (!CORBA::_interfaceCmp (IT_s,ttcp_string_IR))
         return IT_p;
      else if (IT_l=CORBA::ObjectFactoryClass::IT_castUp((CORBA::Object*)((ttcp_string*)IT_p),IT_s))
         return IT_l;
      else return NULL;
    }


CORBA::PPTR* ttcp_stringProxyFactoryClass::pptr (void *IT_p) {
       return ((ttcp_string*)IT_p)->_pptr ();}

void ttcp_stringProxyFactoryClass::baseInterfaces (_IDL_SEQUENCE_string& seq) {
      add (seq, ttcp_string_IR);
      CORBA::ObjectFactoryClass::baseInterfaces (seq);
}

long ttcp_string:: send(const char * ttcp_str, CORBA::Environment &IT_env) {

    if (IT_env || m_isNull) return 0;
    CORBA::Request IT_r (this, "send",IT_env,1,0);
    if (!IT_r.isException (IT_env)) {
        IT_r.encodeStringOp (ttcp_str);
    }

    IT_r.invoke (CORBA::Flags(0),IT_env);
    if (!IT_r.isException (IT_env)) {
        long IT_result;
        IT_r >> IT_result;
        IT_r.checkEnv (IT_env);
        return IT_result;
    }
    return 0;
}

void ttcp_string:: start_timer(CORBA::Environment &IT_env) {

    if (IT_env || m_isNull) return ;
    CORBA::Request IT_r (this, "start_timer",IT_env,1,1);

    IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
}

void ttcp_string:: stop_timer(CORBA::Environment &IT_env) {

    if (IT_env || m_isNull) return ;
    CORBA::Request IT_r (this, "stop_timer",IT_env,1,1);

    IT_r.invoke (CORBA::Flags(CORBA::INV_NO_RESPONSE), IT_env);
}


ttcp_stringProxyFactoryClass ttcp_stringProxyFactory(1);


#ifndef ttcp_string_dispatch_impl

unsigned char ttcp_string_dispatch::dispatch (CORBA::Request &IT_r, 
    unsigned char, void *) {
      IT_r.makeRuntimeException1 ("ttcp_string");
      return 0;
}
 
#endif



#ifndef ttcp_hh
#define ttcp_hh

#include <CORBA.h>

#include <string.h>


#ifndef _IDL_SEQUENCE_char_defined
#define _IDL_SEQUENCE_char_defined

struct IONANC__IDL_SEQUENCE_char;
struct _IDL_SEQUENCE_char {
    unsigned long _maximum;
    unsigned long _length;
    char *_buffer;

    operator IONANC__IDL_SEQUENCE_char();
    operator const IONANC__IDL_SEQUENCE_char() const;
    _IDL_SEQUENCE_char& operator= (const IONANC__IDL_SEQUENCE_char&);

    _IDL_SEQUENCE_char& operator= (const _IDL_SEQUENCE_char&);
    _IDL_SEQUENCE_char (const _IDL_SEQUENCE_char&);

    _IDL_SEQUENCE_char (unsigned long IT_size = 0);

    ~_IDL_SEQUENCE_char () { if (_buffer) delete [] _buffer; }

    char& operator [] (unsigned long IT_i) const {return  _buffer[IT_i]; }

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
};

struct IONANC__IDL_SEQUENCE_char {
    unsigned long _maximum;
    unsigned long _length;
    char *_buffer;

    char& operator [] (unsigned long IT_i) const;

    operator _IDL_SEQUENCE_char ();

    operator const _IDL_SEQUENCE_char () const;

};



#endif


#ifndef _ttcp_sequence_defined
#define _ttcp_sequence_defined
class ttcp_sequence_dispatch : public virtual CORBA::PPTR {
public:

   ttcp_sequence_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   ttcp_sequence_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   ttcp_sequence_dispatch () {}

   ttcp_sequence_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class ttcp_sequence;


#ifndef ttcp_sequenceForwH
#define ttcp_sequenceForwH
CORBA::ObjectRef ttcp_sequence_getBase (void *);
void ttcp_sequence_release (void *, CORBA::Environment &IT_env=CORBA::default_environment);
ttcp_sequence* ttcp_sequence_duplicate (void *, CORBA::Environment &IT_env=CORBA::default_environment);
#endif
#define ttcp_sequence_IMPL "ttcp_sequence"


class ttcp_sequence;
#define ttcp_sequence_IR "ttcp_sequence"
#define ttcp_sequence_IMPL "ttcp_sequence"

typedef ttcp_sequence* ttcp_sequenceRef;
typedef ttcp_sequence* ttcp_sequence_ptr;
class ttcp_sequence: public virtual CORBA::Object {
public:
    ttcp_sequence (char *IT_OR);
    ttcp_sequence () : CORBA::Object (1) {}
    ttcp_sequence* _duplicate(
            CORBA::Environment &IT_env=CORBA::default_environment) {
       CORBA::Object::_duplicate (IT_env); return this; }
   static ttcp_sequence* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::default_environment);
   static ttcp_sequence* _bind (CORBA::Environment &IT_env);
   static ttcp_sequence* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::default_environment);
    static ttcp_sequence* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::default_environment);
typedef _IDL_SEQUENCE_char my_sequence;
    	virtual long send (const ttcp_sequence::my_sequence& ttcp_seq, CORBA::Environment &IT_env=CORBA::default_environment);
    	virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment);
    	virtual void stop_timer (CORBA::Environment &IT_env=CORBA::default_environment);
};


#define TIE_ttcp_sequence(X) ttcp_sequence##X

#define DEF_TIE_ttcp_sequence(X) \
	class ttcp_sequence##X : public virtual ttcp_sequence {		\
	  X* m_obj;						\
	public:							\
								\
	   ttcp_sequence##X  (X *objp, const char* m="", CORBA::LoaderClass *l=nil)\
		: ttcp_sequence(), CORBA::Object (), m_obj(objp) {	\
		m_pptr = new ttcp_sequence_dispatch		\
			(( ttcp_sequence*)this,(CORBA::Object*)this,m,l,ttcp_sequence_IR,m_obj);	\
	   }								\
	   ttcp_sequence##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=nil)\
		: ttcp_sequence(), CORBA::Object () {	\
		m_pptr = new ttcp_sequence_dispatch		\
			(( ttcp_sequence*)this,(CORBA::Object*)this,IT_m,ttcp_sequence_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~ttcp_sequence##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
								\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
	virtual long send (const ttcp_sequence::my_sequence& ttcp_seq, CORBA::Environment &IT_env) {\
return m_obj->send ( ttcp_seq,IT_env);\
}\
	\
	virtual void start_timer (CORBA::Environment &IT_env) {\
m_obj->start_timer (IT_env);\
}\
	\
	virtual void stop_timer (CORBA::Environment &IT_env) {\
m_obj->stop_timer (IT_env);\
}\
								\
	};


#define QUALS_ttcp_sequence	\
	virtual long send (const ttcp_sequence::my_sequence& ttcp_seq, CORBA::Environment &IT_env) {\
return m_obj->send ( ttcp_seq,IT_env);\
}\
	\
	virtual void start_timer (CORBA::Environment &IT_env) {\
m_obj->start_timer (IT_env);\
}\
	\
	virtual void stop_timer (CORBA::Environment &IT_env) {\
m_obj->stop_timer (IT_env);\
}\




class ttcp_sequenceProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   ttcp_sequenceProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (ttcp_sequence_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New2 (); 

    virtual void* IT_castUp (void *IT_p, char* IT_s);

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

extern ttcp_sequenceProxyFactoryClass ttcp_sequenceProxyFactory;



class ttcp_sequenceBOAImpl : public virtual ttcp_sequence {
public:
    ttcp_sequenceBOAImpl (const char *m="", CORBA::LoaderClass *l=NULL) {
  if (CORBA::PPTR::isOK (m_pptr, ttcp_sequence_IR))
    m_pptr = new ttcp_sequence_dispatch ( (ttcp_sequence*)this,
   	 (CORBA::Object*)this, m, l, ttcp_sequence_IR, this);
}

    	virtual long send (const ttcp_sequence::my_sequence& ttcp_seq, CORBA::Environment &IT_env=CORBA::default_environment) =0;
    	virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment) =0;
    	virtual void stop_timer (CORBA::Environment &IT_env=CORBA::default_environment) =0;
};


#endif


#ifndef _ttcp_string_defined
#define _ttcp_string_defined
class ttcp_string_dispatch : public virtual CORBA::PPTR {
public:

   ttcp_string_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   ttcp_string_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   ttcp_string_dispatch () {}

   ttcp_string_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class ttcp_string;


#ifndef ttcp_stringForwH
#define ttcp_stringForwH
CORBA::ObjectRef ttcp_string_getBase (void *);
void ttcp_string_release (void *, CORBA::Environment &IT_env=CORBA::default_environment);
ttcp_string* ttcp_string_duplicate (void *, CORBA::Environment &IT_env=CORBA::default_environment);
#endif
#define ttcp_string_IMPL "ttcp_string"


class ttcp_string;
#define ttcp_string_IR "ttcp_string"
#define ttcp_string_IMPL "ttcp_string"

typedef ttcp_string* ttcp_stringRef;
typedef ttcp_string* ttcp_string_ptr;
class ttcp_string: public virtual CORBA::Object {
public:
    ttcp_string (char *IT_OR);
    ttcp_string () : CORBA::Object (1) {}
    ttcp_string* _duplicate(
            CORBA::Environment &IT_env=CORBA::default_environment) {
       CORBA::Object::_duplicate (IT_env); return this; }
   static ttcp_string* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::default_environment);
   static ttcp_string* _bind (CORBA::Environment &IT_env);
   static ttcp_string* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::default_environment);
    static ttcp_string* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::default_environment);
    	virtual long send (const char * ttcp_str, CORBA::Environment &IT_env=CORBA::default_environment);
    	virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment);
    	virtual void stop_timer (CORBA::Environment &IT_env=CORBA::default_environment);
};


#define TIE_ttcp_string(X) ttcp_string##X

#define DEF_TIE_ttcp_string(X) \
	class ttcp_string##X : public virtual ttcp_string {		\
	  X* m_obj;						\
	public:							\
								\
	   ttcp_string##X  (X *objp, const char* m="", CORBA::LoaderClass *l=nil)\
		: ttcp_string(), CORBA::Object (), m_obj(objp) {	\
		m_pptr = new ttcp_string_dispatch		\
			(( ttcp_string*)this,(CORBA::Object*)this,m,l,ttcp_string_IR,m_obj);	\
	   }								\
	   ttcp_string##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=nil)\
		: ttcp_string(), CORBA::Object () {	\
		m_pptr = new ttcp_string_dispatch		\
			(( ttcp_string*)this,(CORBA::Object*)this,IT_m,ttcp_string_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~ttcp_string##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
								\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
	virtual long send (const char * ttcp_str, CORBA::Environment &IT_env) {\
return m_obj->send ( ttcp_str,IT_env);\
}\
	\
	virtual void start_timer (CORBA::Environment &IT_env) {\
m_obj->start_timer (IT_env);\
}\
	\
	virtual void stop_timer (CORBA::Environment &IT_env) {\
m_obj->stop_timer (IT_env);\
}\
								\
	};


#define QUALS_ttcp_string	\
	virtual long send (const char * ttcp_str, CORBA::Environment &IT_env) {\
return m_obj->send ( ttcp_str,IT_env);\
}\
	\
	virtual void start_timer (CORBA::Environment &IT_env) {\
m_obj->start_timer (IT_env);\
}\
	\
	virtual void stop_timer (CORBA::Environment &IT_env) {\
m_obj->stop_timer (IT_env);\
}\




class ttcp_stringProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   ttcp_stringProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (ttcp_string_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New2 (); 

    virtual void* IT_castUp (void *IT_p, char* IT_s);

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

extern ttcp_stringProxyFactoryClass ttcp_stringProxyFactory;



class ttcp_stringBOAImpl : public virtual ttcp_string {
public:
    ttcp_stringBOAImpl (const char *m="", CORBA::LoaderClass *l=NULL) {
  if (CORBA::PPTR::isOK (m_pptr, ttcp_string_IR))
    m_pptr = new ttcp_string_dispatch ( (ttcp_string*)this,
   	 (CORBA::Object*)this, m, l, ttcp_string_IR, this);
}

    	virtual long send (const char * ttcp_str, CORBA::Environment &IT_env=CORBA::default_environment) =0;
    	virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment) =0;
    	virtual void stop_timer (CORBA::Environment &IT_env=CORBA::default_environment) =0;
};


#endif


#endif

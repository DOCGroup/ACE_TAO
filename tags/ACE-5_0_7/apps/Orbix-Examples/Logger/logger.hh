
#ifndef logger_hh
#define logger_hh

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


#ifndef _logger_defined
#define _logger_defined
class logger_dispatch : public virtual CORBA::PPTR {
public:

   logger_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   logger_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   logger_dispatch () {}

   logger_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class logger;


#ifndef loggerForwH
#define loggerForwH
CORBA::ObjectRef logger_getBase (void *);
void logger_release (void *, CORBA::Environment &IT_env=CORBA::default_environment);
logger* logger_duplicate (void *, CORBA::Environment &IT_env=CORBA::default_environment);
#endif
#define logger_IMPL "logger"


class logger;
#define logger_IR "logger"
#define logger_IMPL "logger"

typedef logger* loggerRef;
typedef logger* logger_ptr;
class logger: public virtual CORBA::Object {
public:
    logger (char *IT_OR);
    logger () : CORBA::Object (1) {}
    logger* _duplicate(
            CORBA::Environment &IT_env=CORBA::default_environment) {
       CORBA::Object::_duplicate (IT_env); return this; }
   static logger* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::default_environment);
   static logger* _bind (CORBA::Environment &IT_env);
   static logger* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::default_environment);
    static logger* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::default_environment);
enum Log_Priority {LM_MESSAGE,LM_DEBUG,LM_WARNING,LM_ERROR,LM_EMERG};

#ifndef logger_Log_Record_defined
#define logger_Log_Record_defined

struct IONANC_Log_Record;
struct Log_Record {
    logger::Log_Priority type;
    long time;
    long app_id;
    long host_addr;
    _IDL_SEQUENCE_char msg_data;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
    Log_Record(const Log_Record &);
    Log_Record();
    operator logger::IONANC_Log_Record();
    operator const logger::IONANC_Log_Record() const;
    Log_Record& operator= (const IONANC_Log_Record&);
    ~Log_Record();
    Log_Record& operator= (const Log_Record&);
};

struct IONANC_Log_Record {
    logger::Log_Priority type;
    long time;
    long app_id;
    long host_addr;
    IONANC__IDL_SEQUENCE_char msg_data;
    operator logger::Log_Record ();
    operator const logger::Log_Record () const;
    };


#endif

    	virtual void log (const logger::Log_Record& log_rec, CORBA::Environment &IT_env=CORBA::default_environment);
    virtual void verbose (char verbose, CORBA::Environment &IT_env=CORBA::default_environment);
    virtual char verbose (CORBA::Environment &IT_env=CORBA::default_environment);
};


#define TIE_logger(X) logger##X

#define DEF_TIE_logger(X) \
	class logger##X : public virtual logger {		\
	  X* m_obj;						\
	public:							\
								\
	   logger##X  (X *objp, const char* m="", CORBA::LoaderClass *l=nil)\
		: logger(), CORBA::Object (), m_obj(objp) {	\
		m_pptr = new logger_dispatch		\
			(( logger*)this,(CORBA::Object*)this,m,l,logger_IR,m_obj);	\
	   }								\
	   logger##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=nil)\
		: logger(), CORBA::Object () {	\
		m_pptr = new logger_dispatch		\
			(( logger*)this,(CORBA::Object*)this,IT_m,logger_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~logger##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
								\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
	virtual void log (const logger::Log_Record& log_rec, CORBA::Environment &IT_env) {\
m_obj->log ( log_rec,IT_env);\
}\
	\
virtual void verbose (char verbose, CORBA::Environment &IT_env) {\
    m_obj->verbose(verbose,IT_env); }\
	\
virtual char verbose (CORBA::Environment &IT_env) {\
return m_obj->verbose(IT_env); }\
								\
	};


#define QUALS_logger	\
	virtual void log (const logger::Log_Record& log_rec, CORBA::Environment &IT_env) {\
m_obj->log ( log_rec,IT_env);\
}\
	\
virtual void verbose (char verbose, CORBA::Environment &IT_env) {\
    m_obj->verbose(verbose,IT_env); }\
	\
virtual char verbose (CORBA::Environment &IT_env) {\
return m_obj->verbose(IT_env); }\




class loggerProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   loggerProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (logger_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New2 (); 

    virtual void* IT_castUp (void *IT_p, char* IT_s);

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

extern loggerProxyFactoryClass loggerProxyFactory;



class loggerBOAImpl : public virtual logger {
public:
    loggerBOAImpl (const char *m="", CORBA::LoaderClass *l=NULL) {
  if (CORBA::PPTR::isOK (m_pptr, logger_IR))
    m_pptr = new logger_dispatch ( (logger*)this,
   	 (CORBA::Object*)this, m, l, logger_IR, this);
}

    	virtual void log (const logger::Log_Record& log_rec, CORBA::Environment &IT_env=CORBA::default_environment) =0;
    virtual void verbose (char verbose, CORBA::Environment &IT_env=CORBA::default_environment)=0;
    virtual char verbose (CORBA::Environment &IT_env=CORBA::default_environment)=0;
};


#endif


#ifndef _profile_logger_defined
#define _profile_logger_defined
class profile_logger_dispatch : public virtual logger_dispatch {
public:

   profile_logger_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   profile_logger_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   profile_logger_dispatch () {}

   profile_logger_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class profile_logger;


#ifndef profile_loggerForwH
#define profile_loggerForwH
CORBA::ObjectRef profile_logger_getBase (void *);
void profile_logger_release (void *, CORBA::Environment &IT_env=CORBA::default_environment);
profile_logger* profile_logger_duplicate (void *, CORBA::Environment &IT_env=CORBA::default_environment);
#endif
#define profile_logger_IMPL "profile_logger"


class profile_logger;
#define profile_logger_IR "profile_logger"
#define profile_logger_IMPL "profile_logger"

typedef profile_logger* profile_loggerRef;
typedef profile_logger* profile_logger_ptr;
class profile_logger: public virtual logger {
public:
    profile_logger (char *IT_OR);
    profile_logger () : CORBA::Object (1) {}
    profile_logger* _duplicate(
            CORBA::Environment &IT_env=CORBA::default_environment) {
       CORBA::Object::_duplicate (IT_env); return this; }
   static profile_logger* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::default_environment);
   static profile_logger* _bind (CORBA::Environment &IT_env);
   static profile_logger* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::default_environment);
    static profile_logger* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::default_environment);

#ifndef profile_logger_Elapsed_Time_defined
#define profile_logger_Elapsed_Time_defined

struct Elapsed_Time {
    double real_time;
    double user_time;
    double system_time;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
};


#endif

    	virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment);
    	virtual void stop_timer (profile_logger::Elapsed_Time& et, CORBA::Environment &IT_env=CORBA::default_environment);
};


#define TIE_profile_logger(X) profile_logger##X

#define DEF_TIE_profile_logger(X) \
	class profile_logger##X : public virtual profile_logger {		\
	  X* m_obj;						\
	public:							\
								\
	   profile_logger##X  (X *objp, const char* m="", CORBA::LoaderClass *l=nil)\
		: profile_logger(), CORBA::Object (), m_obj(objp) {	\
		m_pptr = new profile_logger_dispatch		\
			(( profile_logger*)this,(CORBA::Object*)this,m,l,profile_logger_IR,m_obj);	\
	   }								\
	   profile_logger##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=nil)\
		: profile_logger(), CORBA::Object () {	\
		m_pptr = new profile_logger_dispatch		\
			(( profile_logger*)this,(CORBA::Object*)this,IT_m,profile_logger_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~profile_logger##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
								\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
	virtual void log (const logger::Log_Record& log_rec, CORBA::Environment &IT_env) {\
m_obj->log ( log_rec,IT_env);\
}\
	\
virtual void verbose (char verbose, CORBA::Environment &IT_env) {\
    m_obj->verbose(verbose,IT_env); }\
	\
virtual char verbose (CORBA::Environment &IT_env) {\
return m_obj->verbose(IT_env); }\
	virtual void start_timer (CORBA::Environment &IT_env) {\
m_obj->start_timer (IT_env);\
}\
	\
	virtual void stop_timer (profile_logger::Elapsed_Time& et, CORBA::Environment &IT_env) {\
m_obj->stop_timer ( et,IT_env);\
}\
								\
	};


#define QUALS_profile_logger	\
	virtual void log (const logger::Log_Record& log_rec, CORBA::Environment &IT_env) {\
m_obj->log ( log_rec,IT_env);\
}\
	\
virtual void verbose (char verbose, CORBA::Environment &IT_env) {\
    m_obj->verbose(verbose,IT_env); }\
	\
virtual char verbose (CORBA::Environment &IT_env) {\
return m_obj->verbose(IT_env); }\
	virtual void start_timer (CORBA::Environment &IT_env) {\
m_obj->start_timer (IT_env);\
}\
	\
	virtual void stop_timer (profile_logger::Elapsed_Time& et, CORBA::Environment &IT_env) {\
m_obj->stop_timer ( et,IT_env);\
}\




class profile_loggerProxyFactoryClass : public virtual loggerProxyFactoryClass {
public:
   profile_loggerProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (profile_logger_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New2 (); 

    virtual void* IT_castUp (void *IT_p, char* IT_s);

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

extern profile_loggerProxyFactoryClass profile_loggerProxyFactory;



class profile_loggerBOAImpl : public virtual profile_logger {
public:
    profile_loggerBOAImpl (const char *m="", CORBA::LoaderClass *l=NULL) {
  if (CORBA::PPTR::isOK (m_pptr, profile_logger_IR))
    m_pptr = new profile_logger_dispatch ( (profile_logger*)this,
   	 (CORBA::Object*)this, m, l, profile_logger_IR, this);
}

    	virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment) =0;
    	virtual void stop_timer (profile_logger::Elapsed_Time& et, CORBA::Environment &IT_env=CORBA::default_environment) =0;
};


#endif


#endif

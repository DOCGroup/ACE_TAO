
#ifndef Event_Comm_hh
#define Event_Comm_hh

#include <CORBA.h>

#include <string.h>

class Event_Comm {
public: 

#ifndef Event_Comm_Notification_defined
#define Event_Comm_Notification_defined

    struct Notification {
        CORBA::String_mgr tag_;

        void encodeOp (CORBA::Request &IT_r) const;
        void decodeOp (CORBA::Request &IT_r);
        void decodeInOutOp (CORBA::Request &IT_r);
        static void* IT_anySupport (CORBA::Request &IT_r,
        			void *&, void*, const CORBA::Flags&);
        static const void *IT_fn;
    };

    static const CORBA::TypeCode_ptr _tc_Notification;

#ifndef Event_Comm_NotificationVarH
#define Event_Comm_NotificationVarH

#ifndef Event_Comm_NotificationvPtr
#define Event_Comm_NotificationvPtr
typedef Notification* Notification_vPtr;
#endif

class Notification_var : public CORBA::_var
{
    public:

    Notification_var () {
        _ptr = NULL;
    }

    Notification_var (Notification *IT_p) {
        _ptr = IT_p;
    }

    Notification_var (const Notification_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
            return;
        }
        _ptr = new Notification (*(IT_s._ptr));
    }

    Notification_var &operator= (Notification *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    Notification_var &operator= (const Notification_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new Notification (*(IT_s._ptr));
        return (*this);
    }

    ~Notification_var () {
        delete _ptr;
    }

    Notification* operator-> () {
        return _ptr;
    }

    operator const Notification_vPtr () const { return _ptr;}
    operator Notification_vPtr& () { return _ptr;}
    operator Notification& () const { return * _ptr;}

    protected:
    Notification *_ptr;
    private:
    Notification_var &operator= (const CORBA::_var &IT_s);
    Notification_var (const CORBA::_var &IT_s);
};

#endif


#endif


#ifndef _Event_Comm_Notification_Receiver_defined
#define _Event_Comm_Notification_Receiver_defined
class Notification_Receiver_dispatch : public virtual CORBA::PPTR {
public:

   Notification_Receiver_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   Notification_Receiver_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   Notification_Receiver_dispatch () {}

   Notification_Receiver_dispatch (ObjectReference *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   Notification_Receiver_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class Notification_Receiver;

#ifndef Event_Comm_Notification_ReceiverPtr
#define Event_Comm_Notification_ReceiverPtr

    typedef Notification_Receiver* Notification_Receiver_ptr;

    typedef Notification_Receiver* Notification_ReceiverRef;

#endif


#ifndef Event_Comm_Notification_ReceiverForwH
#define Event_Comm_Notification_ReceiverForwH
static CORBA::ObjectRef Notification_Receiver_getBase (void *);
static void Notification_Receiver_release (Notification_Receiver *, CORBA::Environment &IT_env);
static void Notification_Receiver_release (Notification_Receiver_ptr);
static Notification_Receiver* Notification_Receiver_duplicate (Notification_Receiver_ptr, CORBA::Environment &IT_env);
static Notification_Receiver* Notification_Receiver_duplicate (Notification_Receiver_ptr );
static Notification_Receiver_ptr Notification_Receiver_nil (CORBA::Environment &IT_env);
static Notification_Receiver_ptr Notification_Receiver_nil ();
#endif
#define Event_Comm_Notification_Receiver_IMPL "Event_Comm_Notification_Receiver"


class Notification_Receiver;

    typedef Notification_Receiver Notification_ReceiverProxy;
#define Event_Comm_Notification_Receiver_IR "Event_Comm_Notification_Receiver"
#define Event_Comm_Notification_Receiver_IMPL "Event_Comm_Notification_Receiver"

#ifndef Event_Comm_Notification_ReceiverPtr
#define Event_Comm_Notification_ReceiverPtr

    typedef Notification_Receiver* Notification_Receiver_ptr;

    typedef Notification_Receiver* Notification_ReceiverRef;

#endif

class Notification_Receiver: public virtual CORBA::Object {
public:
    Notification_Receiver (char *IT_OR);
    Notification_Receiver (ObjectReference *IT_OR);
    Notification_Receiver () : CORBA::Object (1) {}
protected:
    Notification_Receiver_ptr __duplicate(
            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
       CORBA::Object::__duplicate (IT_env);
       return this;
    }
public:
    static Notification_Receiver_ptr _duplicate(
            Notification_Receiver_ptr obj,
            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
       CORBA::EnvExcRaiser IT_raise (&IT_env);
       if (CORBA::is_nil(obj, IT_env)) {
          IT_raise.maybeRaise ();
          return (obj);
       }
       Notification_Receiver_ptr IT_obj = obj->__duplicate (IT_env);
       IT_raise.maybeRaise();
       return IT_obj;
    }
public:
   static Notification_Receiver* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
   static Notification_Receiver* _bind (CORBA::Environment &IT_env);
   static Notification_Receiver* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Notification_Receiver* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Notification_Receiver_ptr _nil (CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
        CORBA::EnvExcRaiser IT_raise (&IT_env);
        IT_raise.maybeRaise();
        return (Notification_Receiver_ptr) CORBA::OBJECT_NIL;}
                virtual void receive_notification (const Event_Comm::Notification& notification, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
                virtual void disconnect (const char * reason, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
};

    static const CORBA::TypeCode_ptr _tc_Notification_Receiver;

    static const CORBA::TypeCode_ptr _tc_Notification_ReceiverRef;

#ifndef Event_Comm_Notification_ReceiverVarH
#define Event_Comm_Notification_ReceiverVarH

#ifndef Event_Comm_Notification_ReceivervPtr
#define Event_Comm_Notification_ReceivervPtr
typedef Notification_Receiver* Notification_Receiver_vPtr;
#endif

class Notification_Receiver_var : public CORBA::_var
{
    public:

    Notification_Receiver_var () {
        _ptr = Notification_Receiver_nil ();
    }

    Notification_Receiver_var (Notification_Receiver *IT_p) {
        _ptr = IT_p;
    }

    Notification_Receiver_var (const Notification_Receiver_var &IT_s) {
        _ptr = Notification_Receiver_duplicate (IT_s._ptr);
    }

    Notification_Receiver_var &operator= (Notification_Receiver *IT_p) {
        Notification_Receiver_release (_ptr);
        _ptr = IT_p;
        return (*this);
    }

    Notification_Receiver_var &operator= (const Notification_Receiver_var &IT_s) {
        Notification_Receiver_release (_ptr);
        _ptr = Notification_Receiver_duplicate (IT_s._ptr);
        return (*this);
    }

    ~Notification_Receiver_var () {
        Notification_Receiver_release (_ptr);
    }

    Notification_Receiver* operator-> () {
        return _ptr;
    }

    operator const Notification_Receiver_vPtr () const { return _ptr;}
    operator Notification_Receiver_vPtr& () { return _ptr;}

    protected:
    Notification_Receiver *_ptr;
    private:
    Notification_Receiver_var &operator= (const CORBA::_var &IT_s);
    Notification_Receiver_var (const CORBA::_var &IT_s);
    Notification_Receiver_var &operator= (const CORBA::_mgr &IT_s);
    Notification_Receiver_var &operator= (const CORBA::_SeqElem &IT_s);
    Notification_Receiver_var (const CORBA::_mgr &IT_s);
    Notification_Receiver_var (const CORBA::_SeqElem &IT_s);
};

#endif


#ifndef Event_Comm_Notification_ReceiverMgrH
#define Event_Comm_Notification_ReceiverMgrH

class Notification_Receiver_mgr : public CORBA::_mgr
{
    public:

    Notification_Receiver_mgr () {
        _ptr = Notification_Receiver_nil ();
        _release = 1;
    }

    Notification_Receiver_mgr (const Notification_Receiver_mgr &IT_s) {
        _ptr = Notification_Receiver_duplicate (IT_s._ptr);
        _release = 1;
    }

    Notification_Receiver_mgr &operator= (Notification_Receiver *IT_p) {
        if (_ptr && _release)
            Notification_Receiver_release (_ptr);
        _ptr = IT_p;
        _release = 1;
        return (*this);
    }

    Notification_Receiver_mgr &operator= (const Notification_Receiver_mgr &IT_s) {
        if (_ptr && _release)
            Notification_Receiver_release (_ptr);
        _ptr = Notification_Receiver_duplicate(IT_s._ptr);
        _release = 1;
        return (*this);
    }

    Notification_Receiver_mgr &operator= (const Notification_Receiver_var &IT_s) {
        if (_ptr && _release)
            Notification_Receiver_release (_ptr);
        _ptr = Notification_Receiver_duplicate(IT_s);
        _release = 1;
        return (*this);
    }

    ~Notification_Receiver_mgr () {
        if (_release)
            Notification_Receiver_release (_ptr);
    }

    unsigned char release () {
        return _release;
    }

    void release (unsigned char rel) {
        _release = rel;
    }

    operator int () const {
        CORBA::Environment env;
        CORBA::EnvExcRaiser IT_raise (&env);
        return (!(CORBA::is_nil((CORBA::Object*) _ptr, env)));
    }

    operator void* () const {
        return _ptr;
    }

    operator CORBA::Object * () const {
        return (CORBA::Object *) _ptr;
    }

    operator Notification_Receiver* () const {
        return (Notification_Receiver*) _ptr;
    }

    Notification_Receiver *_ptr;

    protected:

    unsigned char _release;
};

#endif

#ifndef Event_Comm_Notification_ReceiverSeqElemH
#define Event_Comm_Notification_ReceiverSeqElemH

class Notification_Receiver_SeqElem : public CORBA::_SeqElem
{
    public:

    Notification_Receiver_SeqElem (Event_Comm::Notification_Receiver_ptr* IT_p, unsigned char rel) {
        _ptr = IT_p;
        _release = rel;
    }

    Notification_Receiver_SeqElem &operator= (Event_Comm::Notification_Receiver_ptr IT_p) {
        if (!_ptr)
            return (*this);
        if (*(_ptr) && _release)
            Notification_Receiver_release (*(_ptr));
        *(_ptr) = IT_p;
        return (*this);
    }

    Notification_Receiver_SeqElem &operator= (const Notification_Receiver_SeqElem &IT_s) {
        if (!_ptr|| !IT_s._ptr)
            return (*this);
        if (*(_ptr) && _release)
            Notification_Receiver_release (*(_ptr));
        *(_ptr) = Notification_Receiver_duplicate(*(IT_s._ptr));
        return (*this);
    }

    operator Event_Comm::Notification_Receiver_ptr () const
{
        if (!_ptr)
            return (Notification_Receiver_nil());
        return (Event_Comm::Notification_Receiver_ptr) (*_ptr);
    }

    Notification_Receiver_ptr operator->() const { return *_ptr;}

    protected:
    Event_Comm::Notification_Receiver_ptr *_ptr;
    unsigned char _release;
};

#endif


#define TIE_Event_Comm_Notification_Receiver(X) Event_Comm_Notification_Receiver##X

#define DEF_TIE_Event_Comm_Notification_Receiver(X) \
	class Event_Comm_Notification_Receiver##X : public virtual Event_Comm::Notification_Receiver {		\
	  X* m_obj;						\
	public:							\
								\
	   Event_Comm_Notification_Receiver##X  (X *objp, const char* m="", CORBA::LoaderClass *l=0)\
		: Event_Comm::Notification_Receiver(), CORBA::Object (), m_obj(objp) {	\
		m_pptr = new Event_Comm::Notification_Receiver_dispatch		\
			(( Event_Comm::Notification_Receiver*)this,(CORBA::Object*)this,m,l,Event_Comm_Notification_Receiver_IR,m_obj);	\
	   }								\
	   Event_Comm_Notification_Receiver##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=0)\
		: Event_Comm::Notification_Receiver(), CORBA::Object () {	\
		m_pptr = new Event_Comm::Notification_Receiver_dispatch		\
			(( Event_Comm::Notification_Receiver*)this,(CORBA::Object*)this,IT_m,Event_Comm_Notification_Receiver_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~Event_Comm_Notification_Receiver##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
								\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
        virtual void receive_notification (const Event_Comm::Notification& notification, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->receive_notification ( notification,IT_env);\
}\
	\
        virtual void disconnect (const char * reason, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->disconnect ( reason,IT_env);\
}\
								\
	};


#define QUALS_Event_Comm_Notification_Receiver	\
        virtual void receive_notification (const Event_Comm::Notification& notification, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->receive_notification ( notification,IT_env);\
}\
	\
        virtual void disconnect (const char * reason, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->disconnect ( reason,IT_env);\
}\




class Notification_ReceiverProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   Notification_ReceiverProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (Event_Comm_Notification_Receiver_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New (ObjectReference *IT_OR, CORBA::Environment&);

    virtual void* New2 (); 

    virtual void* IT_castUp (void *IT_p, char* IT_s);

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

static Notification_ReceiverProxyFactoryClass Notification_ReceiverProxyFactory;



#endif


#ifndef _Event_Comm_Notifier_defined
#define _Event_Comm_Notifier_defined
class Notifier_dispatch : public virtual CORBA::PPTR {
public:

   Notifier_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   Notifier_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   Notifier_dispatch () {}

   Notifier_dispatch (ObjectReference *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : CORBA::PPTR (IT_OR,IT_p,IT_o) {}


   Notifier_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : CORBA::PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class Notifier;

#ifndef Event_Comm_NotifierPtr
#define Event_Comm_NotifierPtr

    typedef Notifier* Notifier_ptr;

    typedef Notifier* NotifierRef;

#endif


#ifndef Event_Comm_NotifierForwH
#define Event_Comm_NotifierForwH
static CORBA::ObjectRef Notifier_getBase (void *);
static void Notifier_release (Notifier *, CORBA::Environment &IT_env);
static void Notifier_release (Notifier_ptr);
static Notifier* Notifier_duplicate (Notifier_ptr, CORBA::Environment &IT_env);
static Notifier* Notifier_duplicate (Notifier_ptr );
static Notifier_ptr Notifier_nil (CORBA::Environment &IT_env);
static Notifier_ptr Notifier_nil ();
#endif
#define Event_Comm_Notifier_IMPL "Event_Comm_Notifier"


class Notifier;

    typedef Notifier NotifierProxy;
#define Event_Comm_Notifier_IR "Event_Comm_Notifier"
#define Event_Comm_Notifier_IMPL "Event_Comm_Notifier"

#ifndef Event_Comm_NotifierPtr
#define Event_Comm_NotifierPtr

    typedef Notifier* Notifier_ptr;

    typedef Notifier* NotifierRef;

#endif

class Notifier: public virtual CORBA::Object {
public:
    Notifier (char *IT_OR);
    Notifier (ObjectReference *IT_OR);
    Notifier () : CORBA::Object (1) {}
protected:
    Notifier_ptr __duplicate(
            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
       CORBA::Object::__duplicate (IT_env);
       return this;
    }
public:
    static Notifier_ptr _duplicate(
            Notifier_ptr obj,
            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
       CORBA::EnvExcRaiser IT_raise (&IT_env);
       if (CORBA::is_nil(obj, IT_env)) {
          IT_raise.maybeRaise ();
          return (obj);
       }
       Notifier_ptr IT_obj = obj->__duplicate (IT_env);
       IT_raise.maybeRaise();
       return IT_obj;
    }
public:
   static Notifier* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
   static Notifier* _bind (CORBA::Environment &IT_env);
   static Notifier* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Notifier* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Notifier_ptr _nil (CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
        CORBA::EnvExcRaiser IT_raise (&IT_env);
        IT_raise.maybeRaise();
        return (Notifier_ptr) CORBA::OBJECT_NIL;}
                virtual void send_disconnect (const char * reason, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
                virtual void send_notification (const Event_Comm::Notification& notification, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
                virtual void subscribe (Event_Comm::Notification_Receiver_ptr notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
                virtual void unsubscribe (Event_Comm::Notification_Receiver_ptr notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
};

    static const CORBA::TypeCode_ptr _tc_Notifier;

    static const CORBA::TypeCode_ptr _tc_NotifierRef;

#ifndef Event_Comm_NotifierVarH
#define Event_Comm_NotifierVarH

#ifndef Event_Comm_NotifiervPtr
#define Event_Comm_NotifiervPtr
typedef Notifier* Notifier_vPtr;
#endif

class Notifier_var : public CORBA::_var
{
    public:

    Notifier_var () {
        _ptr = Notifier_nil ();
    }

    Notifier_var (Notifier *IT_p) {
        _ptr = IT_p;
    }

    Notifier_var (const Notifier_var &IT_s) {
        _ptr = Notifier_duplicate (IT_s._ptr);
    }

    Notifier_var &operator= (Notifier *IT_p) {
        Notifier_release (_ptr);
        _ptr = IT_p;
        return (*this);
    }

    Notifier_var &operator= (const Notifier_var &IT_s) {
        Notifier_release (_ptr);
        _ptr = Notifier_duplicate (IT_s._ptr);
        return (*this);
    }

    ~Notifier_var () {
        Notifier_release (_ptr);
    }

    Notifier* operator-> () {
        return _ptr;
    }

    operator const Notifier_vPtr () const { return _ptr;}
    operator Notifier_vPtr& () { return _ptr;}

    protected:
    Notifier *_ptr;
    private:
    Notifier_var &operator= (const CORBA::_var &IT_s);
    Notifier_var (const CORBA::_var &IT_s);
    Notifier_var &operator= (const CORBA::_mgr &IT_s);
    Notifier_var &operator= (const CORBA::_SeqElem &IT_s);
    Notifier_var (const CORBA::_mgr &IT_s);
    Notifier_var (const CORBA::_SeqElem &IT_s);
};

#endif


#ifndef Event_Comm_NotifierMgrH
#define Event_Comm_NotifierMgrH

class Notifier_mgr : public CORBA::_mgr
{
    public:

    Notifier_mgr () {
        _ptr = Notifier_nil ();
        _release = 1;
    }

    Notifier_mgr (const Notifier_mgr &IT_s) {
        _ptr = Notifier_duplicate (IT_s._ptr);
        _release = 1;
    }

    Notifier_mgr &operator= (Notifier *IT_p) {
        if (_ptr && _release)
            Notifier_release (_ptr);
        _ptr = IT_p;
        _release = 1;
        return (*this);
    }

    Notifier_mgr &operator= (const Notifier_mgr &IT_s) {
        if (_ptr && _release)
            Notifier_release (_ptr);
        _ptr = Notifier_duplicate(IT_s._ptr);
        _release = 1;
        return (*this);
    }

    Notifier_mgr &operator= (const Notifier_var &IT_s) {
        if (_ptr && _release)
            Notifier_release (_ptr);
        _ptr = Notifier_duplicate(IT_s);
        _release = 1;
        return (*this);
    }

    ~Notifier_mgr () {
        if (_release)
            Notifier_release (_ptr);
    }

    unsigned char release () {
        return _release;
    }

    void release (unsigned char rel) {
        _release = rel;
    }

    operator int () const {
        CORBA::Environment env;
        CORBA::EnvExcRaiser IT_raise (&env);
        return (!(CORBA::is_nil((CORBA::Object*) _ptr, env)));
    }

    operator void* () const {
        return _ptr;
    }

    operator CORBA::Object * () const {
        return (CORBA::Object *) _ptr;
    }

    operator Notifier* () const {
        return (Notifier*) _ptr;
    }

    Notifier *_ptr;

    protected:

    unsigned char _release;
};

#endif

#ifndef Event_Comm_NotifierSeqElemH
#define Event_Comm_NotifierSeqElemH

class Notifier_SeqElem : public CORBA::_SeqElem
{
    public:

    Notifier_SeqElem (Event_Comm::Notifier_ptr* IT_p, unsigned char rel) {
        _ptr = IT_p;
        _release = rel;
    }

    Notifier_SeqElem &operator= (Event_Comm::Notifier_ptr IT_p) {
        if (!_ptr)
            return (*this);
        if (*(_ptr) && _release)
            Notifier_release (*(_ptr));
        *(_ptr) = IT_p;
        return (*this);
    }

    Notifier_SeqElem &operator= (const Notifier_SeqElem &IT_s) {
        if (!_ptr|| !IT_s._ptr)
            return (*this);
        if (*(_ptr) && _release)
            Notifier_release (*(_ptr));
        *(_ptr) = Notifier_duplicate(*(IT_s._ptr));
        return (*this);
    }

    operator Event_Comm::Notifier_ptr () const
{
        if (!_ptr)
            return (Notifier_nil());
        return (Event_Comm::Notifier_ptr) (*_ptr);
    }

    Notifier_ptr operator->() const { return *_ptr;}

    protected:
    Event_Comm::Notifier_ptr *_ptr;
    unsigned char _release;
};

#endif


#define TIE_Event_Comm_Notifier(X) Event_Comm_Notifier##X

#define DEF_TIE_Event_Comm_Notifier(X) \
	class Event_Comm_Notifier##X : public virtual Event_Comm::Notifier {		\
	  X* m_obj;						\
	public:							\
								\
	   Event_Comm_Notifier##X  (X *objp, const char* m="", CORBA::LoaderClass *l=0)\
		: Event_Comm::Notifier(), CORBA::Object (), m_obj(objp) {	\
		m_pptr = new Event_Comm::Notifier_dispatch		\
			(( Event_Comm::Notifier*)this,(CORBA::Object*)this,m,l,Event_Comm_Notifier_IR,m_obj);	\
	   }								\
	   Event_Comm_Notifier##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=0)\
		: Event_Comm::Notifier(), CORBA::Object () {	\
		m_pptr = new Event_Comm::Notifier_dispatch		\
			(( Event_Comm::Notifier*)this,(CORBA::Object*)this,IT_m,Event_Comm_Notifier_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~Event_Comm_Notifier##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
								\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
        virtual void send_disconnect (const char * reason, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->send_disconnect ( reason,IT_env);\
}\
	\
        virtual void send_notification (const Event_Comm::Notification& notification, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->send_notification ( notification,IT_env);\
}\
	\
        virtual void subscribe (Event_Comm::Notification_Receiver_ptr notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->subscribe ( notification_receiver, filtering_criteria,IT_env);\
}\
	\
        virtual void unsubscribe (Event_Comm::Notification_Receiver_ptr notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->unsubscribe ( notification_receiver, filtering_criteria,IT_env);\
}\
								\
	};


#define QUALS_Event_Comm_Notifier	\
        virtual void send_disconnect (const char * reason, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->send_disconnect ( reason,IT_env);\
}\
	\
        virtual void send_notification (const Event_Comm::Notification& notification, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->send_notification ( notification,IT_env);\
}\
	\
        virtual void subscribe (Event_Comm::Notification_Receiver_ptr notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->subscribe ( notification_receiver, filtering_criteria,IT_env);\
}\
	\
        virtual void unsubscribe (Event_Comm::Notification_Receiver_ptr notification_receiver, const char * filtering_criteria, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
    m_obj->unsubscribe ( notification_receiver, filtering_criteria,IT_env);\
}\




class NotifierProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   NotifierProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (Event_Comm_Notifier_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New (ObjectReference *IT_OR, CORBA::Environment&);

    virtual void* New2 (); 

    virtual void* IT_castUp (void *IT_p, char* IT_s);

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

static NotifierProxyFactoryClass NotifierProxyFactory;



#endif

};


void operator<<= (CORBA::any &IT_a, Event_Comm::Notification_Receiver_ptr IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Event_Comm::Notification_Receiver_ptr& IT_t);


void operator<<= (CORBA::any &IT_a, Event_Comm::Notifier_ptr IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Event_Comm::Notifier_ptr& IT_t);


void operator<<= (CORBA::any &IT_a, const Event_Comm::Notification& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Event_Comm::Notification*& IT_t);


#endif

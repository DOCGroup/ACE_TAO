// $Id$


#ifndef cubit_hh
#define cubit_hh

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <CORBA.h>


#ifndef _Cubit_defined
#define _Cubit_defined
class Cubit_dispatch : public virtual CORBA::PPTR {
public:

      Cubit_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m,
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   Cubit_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : PPTR (IT_OR,IT_p,IT_o) {}


   Cubit_dispatch () {}

   Cubit_dispatch (ObjectReference *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : PPTR (IT_OR,IT_p,IT_o) {}


   Cubit_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m,
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r,
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class Cubit;

#ifndef CubitPtr
#define CubitPtr

typedef Cubit* Cubit_ptr;

typedef Cubit* CubitRef;

#endif


#ifndef CubitForwH
#define CubitForwH
CORBA::ObjectRef Cubit_getBase (void *);
void Cubit_release (Cubit *, CORBA::Environment &IT_env);
void Cubit_release (Cubit_ptr);
Cubit* Cubit_duplicate (Cubit_ptr, CORBA::Environment &IT_env);
Cubit* Cubit_duplicate (Cubit_ptr );
Cubit_ptr Cubit_nil (CORBA::Environment &IT_env);
Cubit_ptr Cubit_nil ();
#endif
#define Cubit_IMPL "Cubit"


class Cubit;

typedef Cubit CubitProxy;
#define Cubit_IR "Cubit$v1.1"
#define Cubit_IMPL "Cubit"

#ifndef CubitPtr
#define CubitPtr

typedef Cubit* Cubit_ptr;

typedef Cubit* CubitRef;

#endif

class Cubit: public virtual CORBA::Object  {
public:
        Cubit (char *IT_OR);
    Cubit (ObjectReference *IT_OR);
    Cubit () : CORBA::Object  (1) {}
protected:
    Cubit_ptr __duplicate(
            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
       CORBA::Object::__duplicate (IT_env);
       return this;
    }
public:
    static Cubit_ptr _duplicate(
            Cubit_ptr,
            CORBA::Environment &IT_pEnv=CORBA::IT_chooseDefaultEnv ());
public:
   static Cubit* _bind (const char* IT_markerServer, const char* host,
                const CORBA::Context &IT_c,
                CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
   static Cubit* _bind (CORBA::Environment &IT_env);
   static Cubit* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Cubit* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Cubit_ptr _nil (CORBA::Environment &IT_pEnv=CORBA::IT_chooseDefaultEnv ()) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        return (Cubit_ptr) CORBA::OBJECT_NIL;}

#ifndef Cubit_Many_defined
#define Cubit_Many_defined

struct Many {
    CORBA::Octet o;
    CORBA::Long l;
    CORBA::Short s;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
    Many(const Many &);
    Many();
    ~Many();
    Many& operator= (const Many&);
};

#ifndef Cubit_ManyVarH
#define Cubit_ManyVarH

#ifndef Cubit_ManyvPtr
#define Cubit_ManyvPtr
typedef Many* Many_vPtr;
typedef const Many* Many_cvPtr;
#endif

class Many_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const Many_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new Many;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    Many_var (const Many_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    Many_var () {
        _ptr =  new Many;
;
    }

    Many_var (Many *IT_p) {
        _ptr = IT_p;
    }

    Many_var &operator= (Many *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    Many_var &operator= (const Many_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new Many;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~Many_var () {
        delete _ptr;
    }

    Many* operator-> () {
        return _ptr;
    }

    operator Many_cvPtr () const { return _ptr;}
    operator Many_vPtr& () { return _ptr;}
    operator Many& () const { return * _ptr;}

    protected:
    Many *_ptr;
    private:
    Many_var &operator= (const CORBA::_var &IT_s);
    Many_var (const CORBA::_var &IT_s);
};

#endif


#endif

enum discrim {e_0th,e_1st,e_2nd,e_3rd,e_4th,e_5th, IT__ENUM_Cubit_discrim=CORBA_ULONG_MAX};

#ifndef Cubit_oneof_defined
#define Cubit_oneof_defined

struct oneof {
    private:

    discrim __d;
    union {
        CORBA::Octet _o_;
        CORBA::Short _s_;
        CORBA::Long _l_;
        Many * _cm_;
    };

    public:

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);

    private:

    unsigned char isSet;

    public:

    discrim _d () const { return __d; }

    CORBA::Octet o () const {
      return _o_;
    }

    void o (CORBA::Octet IT_member) {
        if (isSet && (__d != Cubit::e_0th))
        {
            this-> oneof::~oneof();
            memset(this, 0, sizeof(*this));
        }

        __d = Cubit::e_0th;
        isSet = 1;
        _o_ = IT_member;
    }

    CORBA::Short s () const {
        return _s_;
    }

    void s (CORBA::Short IT_member) {
        if (isSet && (__d != Cubit::e_1st))
        {
            this-> oneof::~oneof();
            memset(this, 0, sizeof(*this));
        }

        __d = Cubit::e_1st;
        isSet = 1;
        _s_ = IT_member;
    }

    CORBA::Long l () const {
        return _l_;
    }

    void l (CORBA::Long IT_member) {
        if (isSet && (__d != Cubit::e_2nd))
        {
            this-> oneof::~oneof();
            memset(this, 0, sizeof(*this));
        }

        __d = Cubit::e_2nd;
        isSet = 1;
        _l_ = IT_member;
    }

    Many& cm () {
        return (*_cm_);
    }

    const Many& cm () const {
        return (*_cm_);
    }

    void cm (const Many& IT_member) {
        if (isSet && (__d != Cubit::e_3rd))
        {
            this-> oneof::~oneof();
            memset(this, 0, sizeof(*this));
        }

        __d = Cubit::e_3rd;

        if (!isSet) {
            _cm_ = new Many;
            isSet = 1;
        }
        *(_cm_) = IT_member;
    }


    oneof();
    oneof(const oneof &);
    ~oneof();
    oneof& operator= (const oneof&);
};

#ifndef Cubit_oneofVarH
#define Cubit_oneofVarH

#ifndef Cubit_oneofvPtr
#define Cubit_oneofvPtr
typedef oneof* oneof_vPtr;
typedef const oneof* oneof_cvPtr;
#endif

class oneof_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const oneof_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new oneof;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    oneof_var (const oneof_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    oneof_var () {
        _ptr = NULL;
    }

    oneof_var (oneof *IT_p) {
        _ptr = IT_p;
    }

    oneof_var &operator= (oneof *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    oneof_var &operator= (const oneof_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new oneof;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~oneof_var () {
        delete _ptr;
    }

    oneof* operator-> () {
        return _ptr;
    }

    operator oneof_cvPtr () const { return _ptr;}
    operator oneof_vPtr& () { return _ptr;}
    operator oneof& () const { return * _ptr;}

    protected:
    oneof *_ptr;
    private:
    oneof_var &operator= (const CORBA::_var &IT_s);
    oneof_var (const CORBA::_var &IT_s);
};

#endif


#endif
        virtual CORBA::Octet cube_octet (CORBA::Octet o, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual CORBA::Short cube_short (CORBA::Short s, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual CORBA::Long cube_long (CORBA::Long l, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual Many cube_struct (const Many& values, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual oneof cube_union (const oneof& values, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void please_exit (CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
};

#ifndef CubitVarH
#define CubitVarH

#ifndef CubitvPtr
#define CubitvPtr
typedef Cubit* Cubit_vPtr;
typedef const Cubit* Cubit_cvPtr;
#endif

class Cubit_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const Cubit_var &IT_s) {
        {
            _ptr = Cubit_duplicate (IT_s._ptr);
        }
        return 1;
    }

    public:

    Cubit_var (const Cubit_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    Cubit_var () {
        _ptr = Cubit_nil ();
    }

    Cubit_var (Cubit *IT_p) {
        _ptr = IT_p;
    }

    Cubit_var &operator= (Cubit *IT_p) {
        Cubit_release (_ptr);
        _ptr = IT_p;
        return (*this);
    }

    Cubit_var &operator= (const Cubit_var &IT_s) {
        Cubit_release (_ptr);
        _ptr = Cubit_duplicate (IT_s._ptr);
        return (*this);
    }

    ~Cubit_var () {
        Cubit_release (_ptr);
    }

    Cubit* operator-> () {
        return _ptr;
    }

    operator Cubit_cvPtr () const { return _ptr;}
    operator Cubit_vPtr& () { return _ptr;}

    protected:
    Cubit *_ptr;
    private:
    Cubit_var &operator= (const CORBA::_var &IT_s);
    Cubit_var (const CORBA::_var &IT_s);
    Cubit_var &operator= (const CORBA::_mgr &IT_s);
    Cubit_var &operator= (const CORBA::_SeqElem &IT_s);
    Cubit_var (const CORBA::_mgr &IT_s);
    Cubit_var (const CORBA::_SeqElem &IT_s);
};

#endif


#ifndef CubitMgrH
#define CubitMgrH

class Cubit_mgr : public CORBA::_mgr
{
    public:

    Cubit_mgr () {
        _ptr = Cubit_nil ();
        _release = 1;
    }

    Cubit_mgr (const Cubit_mgr &IT_s) {
        _ptr = Cubit_duplicate (IT_s._ptr);
        _release = 1;
    }

    Cubit_mgr &operator= (Cubit *IT_p) {
        if (_ptr && _release)
            Cubit_release (_ptr);
        _ptr = IT_p;
        _release = 1;
        return (*this);
    }

    Cubit_mgr &operator= (const Cubit_mgr &IT_s) {
        if (_ptr && _release)
            Cubit_release (_ptr);
        _ptr = Cubit_duplicate(IT_s._ptr);
        _release = 1;
        return (*this);
    }

    Cubit_mgr &operator= (Cubit_var &IT_s) {
        if (_ptr && _release)
            Cubit_release (_ptr);
        _ptr = Cubit_duplicate(IT_s);
        _release = 1;
        return (*this);
    }

    ~Cubit_mgr () {
        if (_release)
            Cubit_release (_ptr);
    }

    unsigned char release () {
        return _release;
    }

    void release (unsigned char rel) {
        _release = rel;
    }

    operator int () const {
        return (((CORBA::Object_ptr) _ptr) ? 1 : 0);
    }

    operator void* () const {
        return _ptr;
    }

    operator CORBA::Object * () const {
        return (CORBA::Object *) _ptr;
    }

    operator Cubit* () const {
        return (Cubit*) _ptr;
    }

    Cubit_ptr operator-> () const {
        return _ptr;
    }

    Cubit *_ptr;

    protected:

    unsigned char _release;
};

typedef Cubit_mgr Cubit_mgr_IT_const;
#endif

#ifndef CubitSeqElemH
#define CubitSeqElemH

class Cubit_SeqElem : public CORBA::_SeqElem
{
    public:

    Cubit_SeqElem (Cubit_ptr* IT_p, unsigned char rel) {
        _ptr = IT_p;
        _release = rel;
    }

    Cubit_SeqElem &operator= (Cubit_ptr IT_p) {
        if (_ptr) {
            if (*(_ptr) && _release)
                Cubit_release (*(_ptr));
            *(_ptr) = IT_p;
        }
        return (*this);
    }

    Cubit_SeqElem &operator= (const Cubit_SeqElem &IT_s) {
        if (_ptr && IT_s._ptr) {
            if (*(_ptr) && _release)
                Cubit_release (*(_ptr));
            *(_ptr) = Cubit_duplicate(*(IT_s._ptr));
        }
        return (*this);
    }

    operator Cubit_ptr () const {
        return _ptr ? (Cubit_ptr) (*_ptr) : Cubit_nil();
    }

    Cubit_ptr operator->() const { return *_ptr;}

    protected:
    Cubit_ptr *_ptr;
    unsigned char _release;
};

#endif


#define TIE_Cubit(X) Cubit##X

#define DEF_TIE_Cubit(X) \
        class Cubit##X : public virtual Cubit {         \
          X* m_obj;                                             \
        public:                                                 \
                                                                \
                                                                \
           Cubit##X  (X *objp, const char* m="", CORBA::LoaderClass *l=0)\
                : Cubit(), m_obj(objp) {        \
                m_pptr = new Cubit_dispatch             \
                        (( Cubit*)this,(CORBA::Object*)this,m,l,Cubit_IR,m_obj);        \
           }                                                            \
           Cubit##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=0)\
                : Cubit() {     \
                m_pptr = new Cubit_dispatch             \
                        (( Cubit*)this,(CORBA::Object*)this,IT_m,Cubit_IR,IT_p,IT_q);   \
                m_obj = (X*)(m_pptr->getImplObj ());                    \
           }                                                            \
                                                                \
           virtual ~Cubit##X  () {                              \
                if (_okToDeleteImpl ()) delete m_obj; }                                 \
           virtual void* _deref () {                                    \
                return m_obj; }                                 \
                                                                \
virtual CORBA::Octet cube_octet (CORBA::Octet o, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_octet (  o,IT_env);\
}\
        \
virtual CORBA::Short cube_short (CORBA::Short s, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_short (  s,IT_env);\
}\
        \
virtual CORBA::Long cube_long (CORBA::Long l, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_long (  l,IT_env);\
}\
        \
virtual Cubit::Many cube_struct (const Cubit::Many& values, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_struct (  values,IT_env);\
}\
        \
virtual Cubit::oneof cube_union (const Cubit::oneof& values, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_union (  values,IT_env);\
}\
        \
virtual void please_exit (CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->please_exit (IT_env);\
}\
                                                                \
        };                                              \


#define QUALS_Cubit     \
virtual CORBA::Octet cube_octet (CORBA::Octet o, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_octet (  o,IT_env);\
}\
        \
virtual CORBA::Short cube_short (CORBA::Short s, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_short (  s,IT_env);\
}\
        \
virtual CORBA::Long cube_long (CORBA::Long l, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_long (  l,IT_env);\
}\
        \
virtual Many cube_struct (const Many& values, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_struct (  values,IT_env);\
}\
        \
virtual oneof cube_union (const oneof& values, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
return m_obj->cube_union (  values,IT_env);\
}\
        \
virtual void please_exit (CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->please_exit (IT_env);\
}\




class CubitProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   CubitProxyFactoryClass (unsigned char IT_p=0)
                : CORBA::ProxyFactory (Cubit_IR, IT_p) {}

    virtual void* New (char *IT_OR, CORBA::Environment&);

    virtual void* New (ObjectReference *IT_OR, CORBA::Environment&);

    virtual void* New2 ();

    virtual CORBA::Object* NewObject (char *IT_OR, CORBA::Environment&);

    virtual CORBA::Object* NewObject (ObjectReference *IT_OR, CORBA::Environment&);

    virtual CORBA::Object* New2Object ();

    virtual void* IT_castUp (void *IT_p, char* IT_s, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());

    virtual CORBA::PPTR* pptr (void *IT_p);

    virtual void baseInterfaces (_IDL_SEQUENCE_string&);


};

extern CubitProxyFactoryClass CubitProxyFactory;

class CubitBOAImpl : public virtual Cubit  {
public:
    CubitBOAImpl (const char *m="", CORBA::LoaderClass *l=NULL) {
  if (CORBA::PPTR::isOK (m_pptr, Cubit_IR))
    m_pptr = new Cubit_dispatch ( (Cubit*)this,
         (CORBA::Object*)this, m, l, Cubit_IR, this);
}

        virtual CORBA::Octet cube_octet (CORBA::Octet o, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual CORBA::Short cube_short (CORBA::Short s, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual CORBA::Long cube_long (CORBA::Long l, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual Many cube_struct (const Many& values, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual oneof cube_union (const oneof& values, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void please_exit (CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
};


#endif


#endif


#ifndef marshal_h
#define marshal_h

#include <CORBA.h>

#include <string.h>


#ifndef _IDL_SEQUENCE_any_defined
#define _IDL_SEQUENCE_any_defined

class _IDL_SEQUENCE_any {
    CORBA::ULong _maximum;
    CORBA::ULong _length;
    CORBA::any* _buffer;
    unsigned char _release;

    public:
    _IDL_SEQUENCE_any& operator= (const _IDL_SEQUENCE_any&);
    _IDL_SEQUENCE_any (const _IDL_SEQUENCE_any&);

    _IDL_SEQUENCE_any (CORBA::ULong max);
    _IDL_SEQUENCE_any (CORBA::ULong max, CORBA::ULong length, CORBA::any* data, CORBA::Boolean release = 0);
    _IDL_SEQUENCE_any ();

    ~_IDL_SEQUENCE_any ();

    static CORBA::any* allocbuf(CORBA::ULong nelems);
    static void freebuf(CORBA::any* data);

    CORBA::ULong maximum() const;
    CORBA::ULong length() const;
    void length (CORBA::ULong len);

    CORBA::any& operator [] (CORBA::ULong IT_i);

    const CORBA::any& operator [] (CORBA::ULong IT_i) const;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
};

extern const CORBA::TypeCode_ptr _tc__IDL_SEQUENCE_any;

#ifndef _IDL_SEQUENCE_anyVarH
#define _IDL_SEQUENCE_anyVarH

#ifndef _IDL_SEQUENCE_anyvPtr
#define _IDL_SEQUENCE_anyvPtr
typedef _IDL_SEQUENCE_any* _IDL_SEQUENCE_any_vPtr;
typedef const _IDL_SEQUENCE_any* _IDL_SEQUENCE_any_cvPtr;
#endif

class _IDL_SEQUENCE_any_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const _IDL_SEQUENCE_any_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new _IDL_SEQUENCE_any;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    _IDL_SEQUENCE_any_var (const _IDL_SEQUENCE_any_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    _IDL_SEQUENCE_any_var () {
        _ptr = NULL;
    }

    _IDL_SEQUENCE_any_var (_IDL_SEQUENCE_any *IT_p) {
        _ptr = IT_p;
    }

    _IDL_SEQUENCE_any_var &operator= (_IDL_SEQUENCE_any *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    _IDL_SEQUENCE_any_var &operator= (const _IDL_SEQUENCE_any_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new _IDL_SEQUENCE_any;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~_IDL_SEQUENCE_any_var () {
        delete _ptr;
    }

    _IDL_SEQUENCE_any* operator-> () {
        return _ptr;
    }

    operator _IDL_SEQUENCE_any_cvPtr () const { return _ptr;}
    operator _IDL_SEQUENCE_any_vPtr& () { return _ptr;}
    operator _IDL_SEQUENCE_any& () const { return * _ptr;}

    const CORBA::any& operator[] (CORBA::ULong index) const;

    CORBA::any& operator[] (CORBA::ULong index);

    protected:
    _IDL_SEQUENCE_any *_ptr;
    private:
    _IDL_SEQUENCE_any_var &operator= (const CORBA::_var &IT_s);
    _IDL_SEQUENCE_any_var (const CORBA::_var &IT_s);
};

#endif


#endif


#ifndef _Marshal_defined
#define _Marshal_defined
class Marshal_dispatch : public virtual CORBA::PPTR {
public:

      Marshal_dispatch (void *IT_p, CORBA::Object* IT_o, const char *IT_m, 
        CORBA::LoaderClass *IT_l, char *IT_i, void* IT_im)
       : PPTR (IT_p,IT_o,IT_m,IT_l,IT_i,IT_im) {}


   Marshal_dispatch (char *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : PPTR (IT_OR,IT_p,IT_o) {}


   Marshal_dispatch () {}

   Marshal_dispatch (ObjectReference *IT_OR, void *IT_p, CORBA::Object *IT_o)
       : PPTR (IT_OR,IT_p,IT_o) {}


   Marshal_dispatch (void *IT_p, CORBA::Object *IT_o, const char *IT_m, 
        char *IT_i, CORBA::Object* IT_ob, void* IT_im)
       : PPTR (IT_p,IT_o,IT_m,IT_i,IT_ob,IT_im) {}


   virtual unsigned char dispatch (CORBA::Request &IT_r, 
        unsigned char IT_isTarget, void* IT_pp=NULL);


};

class Marshal;

#ifndef MarshalPtr
#define MarshalPtr

typedef Marshal* Marshal_ptr;

typedef Marshal* MarshalRef;

#endif


#ifndef MarshalForwH
#define MarshalForwH
CORBA::ObjectRef Marshal_getBase (void *);
void Marshal_release (Marshal *, CORBA::Environment &IT_env);
void Marshal_release (Marshal_ptr);
Marshal* Marshal_duplicate (Marshal_ptr, CORBA::Environment &IT_env);
Marshal* Marshal_duplicate (Marshal_ptr );
Marshal_ptr Marshal_nil (CORBA::Environment &IT_env);
Marshal_ptr Marshal_nil ();
#endif
#define Marshal_IMPL "Marshal"


class Marshal;

typedef Marshal MarshalProxy;
#define Marshal_IR "Marshal"
#define Marshal_IMPL "Marshal"

#ifndef MarshalPtr
#define MarshalPtr

typedef Marshal* Marshal_ptr;

typedef Marshal* MarshalRef;

#endif

class Marshal: public virtual CORBA::Object  {
public:
        Marshal (char *IT_OR);
    Marshal (ObjectReference *IT_OR);
    Marshal () : CORBA::Object  (1) {}
protected:
    Marshal_ptr __duplicate(
            CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) {
       CORBA::Object::__duplicate (IT_env);
       return this;
    }
public:
    static Marshal_ptr _duplicate(
            Marshal_ptr, 
            CORBA::Environment &IT_pEnv=CORBA::IT_chooseDefaultEnv ());
public:
   static Marshal* _bind (const char* IT_markerServer, const char* host,
		const CORBA::Context &IT_c, 
		CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
   static Marshal* _bind (CORBA::Environment &IT_env);
   static Marshal* _bind (const char* IT_markerServer=NULL, const char* host=NULL,
                 CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
    static Marshal* _narrow (CORBA::Object* , CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ());
private:
    static const void* IT_impl;
public:
    static Marshal_ptr _nil (CORBA::Environment &IT_pEnv=CORBA::IT_chooseDefaultEnv ()) {
        CORBA::EnvExcRaiser IT_raise;
        CORBA::Environment &IT_env = IT_raise.registerEnv (&IT_pEnv);
        return (Marshal_ptr) CORBA::OBJECT_NIL;}

#ifndef Marshal_Marshal_Struct_defined
#define Marshal_Marshal_Struct_defined

struct Marshal_Struct {
    CORBA::Short s;
    CORBA::Long l;
    CORBA::Char c;
    CORBA::Octet o;
    CORBA::Double d;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
    static void* IT_anySupport (CORBA::Request &IT_r,
    			void *&, void*, const CORBA::Flags&);
    static const void *IT_fn;
    Marshal_Struct(const Marshal_Struct &);
    Marshal_Struct();
    ~Marshal_Struct();
    Marshal_Struct& operator= (const Marshal_Struct&);
};

static const CORBA::TypeCode_ptr _tc_Marshal_Struct;

#ifndef Marshal_Marshal_StructVarH
#define Marshal_Marshal_StructVarH

#ifndef Marshal_Marshal_StructvPtr
#define Marshal_Marshal_StructvPtr
typedef Marshal_Struct* Marshal_Struct_vPtr;
typedef const Marshal_Struct* Marshal_Struct_cvPtr;
#endif

class Marshal_Struct_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const Marshal_Struct_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new Marshal_Struct;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    Marshal_Struct_var (const Marshal_Struct_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    Marshal_Struct_var () {
        _ptr =  new Marshal_Struct;
;
    }

    Marshal_Struct_var (Marshal_Struct *IT_p) {
        _ptr = IT_p;
    }

    Marshal_Struct_var &operator= (Marshal_Struct *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    Marshal_Struct_var &operator= (const Marshal_Struct_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new Marshal_Struct;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~Marshal_Struct_var () {
        delete _ptr;
    }

    Marshal_Struct* operator-> () {
        return _ptr;
    }

    operator Marshal_Struct_cvPtr () const { return _ptr;}
    operator Marshal_Struct_vPtr& () { return _ptr;}
    operator Marshal_Struct& () const { return * _ptr;}

    protected:
    Marshal_Struct *_ptr;
    private:
    Marshal_Struct_var &operator= (const CORBA::_var &IT_s);
    Marshal_Struct_var (const CORBA::_var &IT_s);
};

#endif


#endif

static const CORBA::TypeCode_ptr _tc_discrim;

enum discrim {e_0th,e_1st,e_2nd,e_3rd,e_4th,e_5th,e_6th, IT__ENUM_Marshal_discrim=CORBA_ULONG_MAX};

#ifndef Marshal_Marshal_Union_defined
#define Marshal_Marshal_Union_defined

struct Marshal_Union {
    private:

    discrim __d;
    union {
        CORBA::Short _s_;
        CORBA::Long _l_;
        CORBA::Char _c_;
        CORBA::Octet _o_;
        CORBA::Double _d_;
        Marshal_Struct * _ms_;
    };

    public:

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
    static void* IT_anySupport (CORBA::Request &IT_r,
    			void *&, void*, const CORBA::Flags&);
    static const void *IT_fn;

    private:

    unsigned char isSet;

    public:

    discrim _d () const { return __d; }

    CORBA::Short s () const {
        return _s_;
    }

    void s (CORBA::Short IT_member) {
        if (isSet && (__d != Marshal::e_0th))
        {
            this-> Marshal_Union::~Marshal_Union();
            memset(this, 0, sizeof(*this));
        }

        __d = Marshal::e_0th;
        isSet = 1;
        _s_ = IT_member;
    }

    CORBA::Long l () const {
        return _l_;
    }

    void l (CORBA::Long IT_member) {
        if (isSet && (__d != Marshal::e_1st))
        {
            this-> Marshal_Union::~Marshal_Union();
            memset(this, 0, sizeof(*this));
        }

        __d = Marshal::e_1st;
        isSet = 1;
        _l_ = IT_member;
    }

    CORBA::Char c () const {
        return _c_;
    }

    void c (CORBA::Char IT_member) {
        if (isSet && (__d != Marshal::e_2nd))
        {
            this-> Marshal_Union::~Marshal_Union();
            memset(this, 0, sizeof(*this));
        }

        __d = Marshal::e_2nd;
        isSet = 1;
        _c_ = IT_member;
    }

    CORBA::Octet o () const {
        return _o_;
    }

    void o (CORBA::Octet IT_member) {
        if (isSet && (__d != Marshal::e_3rd))
        {
            this-> Marshal_Union::~Marshal_Union();
            memset(this, 0, sizeof(*this));
        }

        __d = Marshal::e_3rd;
        isSet = 1;
        _o_ = IT_member;
    }

    CORBA::Double d () const {
        return _d_;
    }

    void d (CORBA::Double IT_member) {
        if (isSet && (__d != Marshal::e_4th))
        {
            this-> Marshal_Union::~Marshal_Union();
            memset(this, 0, sizeof(*this));
        }

        __d = Marshal::e_4th;
        isSet = 1;
        _d_ = IT_member;
    }

    Marshal_Struct& ms () {
        return (*_ms_);
    }

    const Marshal_Struct& ms () const {
        return (*_ms_);
    }

    void ms (const Marshal_Struct& IT_member) {
        if (isSet && (__d != Marshal::e_6th))
        {
            this-> Marshal_Union::~Marshal_Union();
            memset(this, 0, sizeof(*this));
        }

        __d = Marshal::e_6th;

        if (!isSet) {
            _ms_ = new Marshal_Struct;
            isSet = 1;
        }
        *(_ms_) = IT_member;
    }


    Marshal_Union();
    Marshal_Union(const Marshal_Union &);
    ~Marshal_Union();
    Marshal_Union& operator= (const Marshal_Union&);
};

static const CORBA::TypeCode_ptr _tc_Marshal_Union;

#ifndef Marshal_Marshal_UnionVarH
#define Marshal_Marshal_UnionVarH

#ifndef Marshal_Marshal_UnionvPtr
#define Marshal_Marshal_UnionvPtr
typedef Marshal_Union* Marshal_Union_vPtr;
typedef const Marshal_Union* Marshal_Union_cvPtr;
#endif

class Marshal_Union_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const Marshal_Union_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new Marshal_Union;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    Marshal_Union_var (const Marshal_Union_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    Marshal_Union_var () {
        _ptr = NULL;
    }

    Marshal_Union_var (Marshal_Union *IT_p) {
        _ptr = IT_p;
    }

    Marshal_Union_var &operator= (Marshal_Union *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    Marshal_Union_var &operator= (const Marshal_Union_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new Marshal_Union;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~Marshal_Union_var () {
        delete _ptr;
    }

    Marshal_Union* operator-> () {
        return _ptr;
    }

    operator Marshal_Union_cvPtr () const { return _ptr;}
    operator Marshal_Union_vPtr& () { return _ptr;}
    operator Marshal_Union& () const { return * _ptr;}

    protected:
    Marshal_Union *_ptr;
    private:
    Marshal_Union_var &operator= (const CORBA::_var &IT_s);
    Marshal_Union_var (const CORBA::_var &IT_s);
};

#endif


#endif
static const CORBA::TypeCode_ptr _tc_AnySeq;

typedef _IDL_SEQUENCE_any AnySeq;
typedef const _IDL_SEQUENCE_any AnySeq_IT_const;

typedef _IDL_SEQUENCE_any_var AnySeq_var;


#ifndef Marshal__IDL_SEQUENCE_Marshal_Marshal_Recursive_defined
#define Marshal__IDL_SEQUENCE_Marshal_Marshal_Recursive_defined

struct Marshal_Recursive;
class _IDL_SEQUENCE_Marshal_Marshal_Recursive {
    CORBA::ULong _maximum;
    CORBA::ULong _length;
    Marshal_Recursive* _buffer;
    unsigned char _release;

    public:
    _IDL_SEQUENCE_Marshal_Marshal_Recursive& operator= (const _IDL_SEQUENCE_Marshal_Marshal_Recursive&);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive (const _IDL_SEQUENCE_Marshal_Marshal_Recursive&);

    _IDL_SEQUENCE_Marshal_Marshal_Recursive (CORBA::ULong max);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive (CORBA::ULong max, CORBA::ULong length, Marshal_Recursive* data, CORBA::Boolean release = 0);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive ();

    ~_IDL_SEQUENCE_Marshal_Marshal_Recursive ();

    static Marshal_Recursive* allocbuf(CORBA::ULong nelems);
    static void freebuf(Marshal_Recursive* data);

    CORBA::ULong maximum() const;
    CORBA::ULong length() const;
    void length (CORBA::ULong len);

    Marshal_Recursive& operator [] (CORBA::ULong IT_i);

    const Marshal_Recursive& operator [] (CORBA::ULong IT_i) const;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
};

static const CORBA::TypeCode_ptr _tc__IDL_SEQUENCE_Marshal_Marshal_Recursive;

#ifndef Marshal__IDL_SEQUENCE_Marshal_Marshal_RecursiveVarH
#define Marshal__IDL_SEQUENCE_Marshal_Marshal_RecursiveVarH

#ifndef Marshal__IDL_SEQUENCE_Marshal_Marshal_RecursivevPtr
#define Marshal__IDL_SEQUENCE_Marshal_Marshal_RecursivevPtr
typedef _IDL_SEQUENCE_Marshal_Marshal_Recursive* _IDL_SEQUENCE_Marshal_Marshal_Recursive_vPtr;
typedef const _IDL_SEQUENCE_Marshal_Marshal_Recursive* _IDL_SEQUENCE_Marshal_Marshal_Recursive_cvPtr;
#endif

class _IDL_SEQUENCE_Marshal_Marshal_Recursive_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new _IDL_SEQUENCE_Marshal_Marshal_Recursive;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var (const _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var () {
        _ptr = NULL;
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var (_IDL_SEQUENCE_Marshal_Marshal_Recursive *IT_p) {
        _ptr = IT_p;
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &operator= (_IDL_SEQUENCE_Marshal_Marshal_Recursive *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &operator= (const _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new _IDL_SEQUENCE_Marshal_Marshal_Recursive;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~_IDL_SEQUENCE_Marshal_Marshal_Recursive_var () {
        delete _ptr;
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive* operator-> () {
        return _ptr;
    }

    operator _IDL_SEQUENCE_Marshal_Marshal_Recursive_cvPtr () const { return _ptr;}
    operator _IDL_SEQUENCE_Marshal_Marshal_Recursive_vPtr& () { return _ptr;}
    operator _IDL_SEQUENCE_Marshal_Marshal_Recursive& () const { return * _ptr;}

    const Marshal_Recursive& operator[] (CORBA::ULong index) const;

    Marshal_Recursive& operator[] (CORBA::ULong index);

    protected:
    _IDL_SEQUENCE_Marshal_Marshal_Recursive *_ptr;
    private:
    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &operator= (const CORBA::_var &IT_s);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var (const CORBA::_var &IT_s);
};

#endif


#endif


#ifndef Marshal_Marshal_Recursive_defined
#define Marshal_Marshal_Recursive_defined

struct Marshal_Recursive {
    CORBA::any value;
    _IDL_SEQUENCE_Marshal_Marshal_Recursive next;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
    static void* IT_anySupport (CORBA::Request &IT_r,
    			void *&, void*, const CORBA::Flags&);
    static const void *IT_fn;
    Marshal_Recursive(const Marshal_Recursive &);
    Marshal_Recursive();
    ~Marshal_Recursive();
    Marshal_Recursive& operator= (const Marshal_Recursive&);
};

static const CORBA::TypeCode_ptr _tc_Marshal_Recursive;

#ifndef Marshal_Marshal_RecursiveVarH
#define Marshal_Marshal_RecursiveVarH

#ifndef Marshal_Marshal_RecursivevPtr
#define Marshal_Marshal_RecursivevPtr
typedef Marshal_Recursive* Marshal_Recursive_vPtr;
typedef const Marshal_Recursive* Marshal_Recursive_cvPtr;
#endif

class Marshal_Recursive_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const Marshal_Recursive_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new Marshal_Recursive;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    Marshal_Recursive_var (const Marshal_Recursive_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    Marshal_Recursive_var () {
        _ptr = NULL;
    }

    Marshal_Recursive_var (Marshal_Recursive *IT_p) {
        _ptr = IT_p;
    }

    Marshal_Recursive_var &operator= (Marshal_Recursive *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    Marshal_Recursive_var &operator= (const Marshal_Recursive_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new Marshal_Recursive;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~Marshal_Recursive_var () {
        delete _ptr;
    }

    Marshal_Recursive* operator-> () {
        return _ptr;
    }

    operator Marshal_Recursive_cvPtr () const { return _ptr;}
    operator Marshal_Recursive_vPtr& () { return _ptr;}
    operator Marshal_Recursive& () const { return * _ptr;}

    protected:
    Marshal_Recursive *_ptr;
    private:
    Marshal_Recursive_var &operator= (const CORBA::_var &IT_s);
    Marshal_Recursive_var (const CORBA::_var &IT_s);
};

#endif


#endif

        virtual void test_short (CORBA::Short s1, CORBA::Short& s2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_long (CORBA::Long l1, CORBA::Long& l2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_octet (CORBA::Octet o1, CORBA::Octet& o2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_char (CORBA::Char c1, CORBA::Char& c2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_double (CORBA::Double d1, CORBA::Double& d2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_struct (const Marshal_Struct& ms1, Marshal_Struct& ms2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_union (const Marshal_Union& u1, Marshal_Union& u2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_any (const CORBA::any& a1, CORBA::any*& a2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_sequence (const AnySeq& as1, AnySeq*& as2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
        virtual void test_recursive (const Marshal_Recursive& mr1, Marshal_Recursive*& mr2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException);
};

extern const CORBA::TypeCode_ptr _tc_MarshalRef;

#ifndef MarshalVarH
#define MarshalVarH

#ifndef MarshalvPtr
#define MarshalvPtr
typedef Marshal* Marshal_vPtr;
typedef const Marshal* Marshal_cvPtr;
#endif

class Marshal_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const Marshal_var &IT_s) {
        {
            _ptr = Marshal_duplicate (IT_s._ptr);
        }
        return 1;
    }

    public:

    Marshal_var (const Marshal_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    Marshal_var () {
        _ptr = Marshal_nil ();
    }

    Marshal_var (Marshal *IT_p) {
        _ptr = IT_p;
    }

    Marshal_var &operator= (Marshal *IT_p) {
        Marshal_release (_ptr);
        _ptr = IT_p;
        return (*this);
    }

    Marshal_var &operator= (const Marshal_var &IT_s) {
        Marshal_release (_ptr);
        _ptr = Marshal_duplicate (IT_s._ptr);
        return (*this);
    }

    ~Marshal_var () {
        Marshal_release (_ptr);
    }

    Marshal* operator-> () {
        return _ptr;
    }

    operator Marshal_cvPtr () const { return _ptr;}
    operator Marshal_vPtr& () { return _ptr;}

    protected:
    Marshal *_ptr;
    private:
    Marshal_var &operator= (const CORBA::_var &IT_s);
    Marshal_var (const CORBA::_var &IT_s);
    Marshal_var &operator= (const CORBA::_mgr &IT_s);
    Marshal_var &operator= (const CORBA::_SeqElem &IT_s);
    Marshal_var (const CORBA::_mgr &IT_s);
    Marshal_var (const CORBA::_SeqElem &IT_s);
};

#endif


#ifndef MarshalMgrH
#define MarshalMgrH

class Marshal_mgr : public CORBA::_mgr
{
    public:

    Marshal_mgr () {
        _ptr = Marshal_nil ();
        _release = 1;
    }

    Marshal_mgr (const Marshal_mgr &IT_s) {
        _ptr = Marshal_duplicate (IT_s._ptr);
        _release = 1;
    }

    Marshal_mgr &operator= (Marshal *IT_p) {
        if (_ptr && _release)
            Marshal_release (_ptr);
        _ptr = IT_p;
        _release = 1;
        return (*this);
    }

    Marshal_mgr &operator= (const Marshal_mgr &IT_s) {
        if (_ptr && _release)
            Marshal_release (_ptr);
        _ptr = Marshal_duplicate(IT_s._ptr);
        _release = 1;
        return (*this);
    }

    Marshal_mgr &operator= (Marshal_var &IT_s) {
        if (_ptr && _release)
            Marshal_release (_ptr);
        _ptr = Marshal_duplicate(IT_s);
        _release = 1;
        return (*this);
    }

    ~Marshal_mgr () {
        if (_release)
            Marshal_release (_ptr);
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

    operator Marshal* () const {
        return (Marshal*) _ptr;
    }

    Marshal_ptr operator-> () const {
        return _ptr;
    }

    Marshal *_ptr;

    protected:

    unsigned char _release;
};

typedef Marshal_mgr Marshal_mgr_IT_const;
#endif

#ifndef MarshalSeqElemH
#define MarshalSeqElemH

class Marshal_SeqElem : public CORBA::_SeqElem
{
    public:

    Marshal_SeqElem (Marshal_ptr* IT_p, unsigned char rel) {
        _ptr = IT_p;
        _release = rel;
    }

    Marshal_SeqElem &operator= (Marshal_ptr IT_p) {
        if (_ptr) {
            if (*(_ptr) && _release)
                Marshal_release (*(_ptr));
            *(_ptr) = IT_p;
        }
        return (*this);
    }

    Marshal_SeqElem &operator= (const Marshal_SeqElem &IT_s) {
        if (_ptr && IT_s._ptr) {
            if (*(_ptr) && _release)
                Marshal_release (*(_ptr));
            *(_ptr) = Marshal_duplicate(*(IT_s._ptr));
        }
        return (*this);
    }

    operator Marshal_ptr () const {
        return _ptr ? (Marshal_ptr) (*_ptr) : Marshal_nil(); 
    }

    Marshal_ptr operator->() const { return *_ptr;}

    protected:
    Marshal_ptr *_ptr;
    unsigned char _release;
};

#endif


#define TIE_Marshal(X) Marshal##X

#define DEF_TIE_Marshal(X) \
	class Marshal##X : public virtual Marshal {		\
	  X* m_obj;						\
	public:							\
								\
								\
	   Marshal##X  (X *objp, const char* m="", CORBA::LoaderClass *l=0)\
		: Marshal(), m_obj(objp) {	\
		m_pptr = new Marshal_dispatch		\
			(( Marshal*)this,(CORBA::Object*)this,m,l,Marshal_IR,m_obj);	\
	   }								\
	   Marshal##X  (CORBA::Object *IT_p, const char* IT_m="", void *IT_q=0)\
		: Marshal() {	\
		m_pptr = new Marshal_dispatch		\
			(( Marshal*)this,(CORBA::Object*)this,IT_m,Marshal_IR,IT_p,IT_q);	\
		m_obj = (X*)(m_pptr->getImplObj ());			\
	   }								\
								\
	   virtual ~Marshal##X  () {				\
		if (_okToDeleteImpl ()) delete m_obj; }					\
	   virtual void* _deref () {					\
		return m_obj; }					\
								\
virtual void test_short (CORBA::Short s1, CORBA::Short& s2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_short (  s1, s2,IT_env);\
}\
	\
virtual void test_long (CORBA::Long l1, CORBA::Long& l2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_long (  l1, l2,IT_env);\
}\
	\
virtual void test_octet (CORBA::Octet o1, CORBA::Octet& o2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_octet (  o1, o2,IT_env);\
}\
	\
virtual void test_char (CORBA::Char c1, CORBA::Char& c2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_char (  c1, c2,IT_env);\
}\
	\
virtual void test_double (CORBA::Double d1, CORBA::Double& d2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_double (  d1, d2,IT_env);\
}\
	\
virtual void test_struct (const Marshal::Marshal_Struct& ms1, Marshal::Marshal_Struct& ms2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_struct (  ms1, ms2,IT_env);\
}\
	\
virtual void test_union (const Marshal::Marshal_Union& u1, Marshal::Marshal_Union& u2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_union (  u1, u2,IT_env);\
}\
	\
virtual void test_any (const CORBA::any& a1, CORBA::any*& a2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_any (  a1, a2,IT_env);\
}\
	\
virtual void test_sequence (const Marshal::AnySeq& as1, Marshal::AnySeq*& as2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_sequence (  as1, as2,IT_env);\
}\
	\
virtual void test_recursive (const Marshal::Marshal_Recursive& mr1, Marshal::Marshal_Recursive*& mr2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_recursive (  mr1, mr2,IT_env);\
}\
								\
	};						\


#define QUALS_Marshal	\
virtual void test_short (CORBA::Short s1, CORBA::Short& s2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_short (  s1, s2,IT_env);\
}\
	\
virtual void test_long (CORBA::Long l1, CORBA::Long& l2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_long (  l1, l2,IT_env);\
}\
	\
virtual void test_octet (CORBA::Octet o1, CORBA::Octet& o2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_octet (  o1, o2,IT_env);\
}\
	\
virtual void test_char (CORBA::Char c1, CORBA::Char& c2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_char (  c1, c2,IT_env);\
}\
	\
virtual void test_double (CORBA::Double d1, CORBA::Double& d2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_double (  d1, d2,IT_env);\
}\
	\
virtual void test_struct (const Marshal_Struct& ms1, Marshal_Struct& ms2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_struct (  ms1, ms2,IT_env);\
}\
	\
virtual void test_union (const Marshal_Union& u1, Marshal_Union& u2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_union (  u1, u2,IT_env);\
}\
	\
virtual void test_any (const CORBA::any& a1, CORBA::any*& a2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_any (  a1, a2,IT_env);\
}\
	\
virtual void test_sequence (const AnySeq& as1, AnySeq*& as2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_sequence (  as1, as2,IT_env);\
}\
	\
virtual void test_recursive (const Marshal_Recursive& mr1, Marshal_Recursive*& mr2, CORBA::Environment &IT_env) throw (CORBA::SystemException){\
m_obj->test_recursive (  mr1, mr2,IT_env);\
}\




class MarshalProxyFactoryClass : public virtual CORBA::ObjectFactoryClass {
public:
   MarshalProxyFactoryClass (unsigned char IT_p=0)
		: CORBA::ProxyFactory (Marshal_IR, IT_p) {}

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

extern MarshalProxyFactoryClass MarshalProxyFactory;

class MarshalBOAImpl : public virtual Marshal  {
public:
    MarshalBOAImpl (const char *m="", CORBA::LoaderClass *l=NULL) {
  if (CORBA::PPTR::isOK (m_pptr, Marshal_IR))
    m_pptr = new Marshal_dispatch ( (Marshal*)this,
   	 (CORBA::Object*)this, m, l, Marshal_IR, this);
}

        virtual void test_short (CORBA::Short s1, CORBA::Short& s2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_long (CORBA::Long l1, CORBA::Long& l2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_octet (CORBA::Octet o1, CORBA::Octet& o2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_char (CORBA::Char c1, CORBA::Char& c2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_double (CORBA::Double d1, CORBA::Double& d2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_struct (const Marshal_Struct& ms1, Marshal_Struct& ms2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_union (const Marshal_Union& u1, Marshal_Union& u2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_any (const CORBA::any& a1, CORBA::any*& a2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_sequence (const AnySeq& as1, AnySeq*& as2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
        virtual void test_recursive (const Marshal_Recursive& mr1, Marshal_Recursive*& mr2, CORBA::Environment &IT_env=CORBA::IT_chooseDefaultEnv ()) throw (CORBA::SystemException) =0;
};


#endif


#ifndef _IDL_SEQUENCE_Marshal_Marshal_Recursive_defined
#define _IDL_SEQUENCE_Marshal_Marshal_Recursive_defined

class _IDL_SEQUENCE_Marshal_Marshal_Recursive {
    CORBA::ULong _maximum;
    CORBA::ULong _length;
    Marshal::Marshal_Recursive* _buffer;
    unsigned char _release;

    public:
    _IDL_SEQUENCE_Marshal_Marshal_Recursive& operator= (const _IDL_SEQUENCE_Marshal_Marshal_Recursive&);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive (const _IDL_SEQUENCE_Marshal_Marshal_Recursive&);

    _IDL_SEQUENCE_Marshal_Marshal_Recursive (CORBA::ULong max);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive (CORBA::ULong max, CORBA::ULong length, Marshal::Marshal_Recursive* data, CORBA::Boolean release = 0);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive ();

    ~_IDL_SEQUENCE_Marshal_Marshal_Recursive ();

    static Marshal::Marshal_Recursive* allocbuf(CORBA::ULong nelems);
    static void freebuf(Marshal::Marshal_Recursive* data);

    CORBA::ULong maximum() const;
    CORBA::ULong length() const;
    void length (CORBA::ULong len);

    Marshal::Marshal_Recursive& operator [] (CORBA::ULong IT_i);

    const Marshal::Marshal_Recursive& operator [] (CORBA::ULong IT_i) const;

    void encodeOp (CORBA::Request &IT_r) const;
    void decodeOp (CORBA::Request &IT_r);
    void decodeInOutOp (CORBA::Request &IT_r);
};

extern const CORBA::TypeCode_ptr _tc__IDL_SEQUENCE_Marshal_Marshal_Recursive;

#ifndef _IDL_SEQUENCE_Marshal_Marshal_RecursiveVarH
#define _IDL_SEQUENCE_Marshal_Marshal_RecursiveVarH

#ifndef _IDL_SEQUENCE_Marshal_Marshal_RecursivevPtr
#define _IDL_SEQUENCE_Marshal_Marshal_RecursivevPtr
typedef _IDL_SEQUENCE_Marshal_Marshal_Recursive* _IDL_SEQUENCE_Marshal_Marshal_Recursive_vPtr;
typedef const _IDL_SEQUENCE_Marshal_Marshal_Recursive* _IDL_SEQUENCE_Marshal_Marshal_Recursive_cvPtr;
#endif

class _IDL_SEQUENCE_Marshal_Marshal_Recursive_var : public CORBA::_var
{
    private:

    unsigned char copyHelper (const _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &IT_s) {
        if (!IT_s._ptr) {
            _ptr = IT_s._ptr;
        } else
        {
            _ptr = new _IDL_SEQUENCE_Marshal_Marshal_Recursive;
            *(_ptr) =  *(IT_s._ptr);
        }
        return 1;
    }

    public:

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var (const _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &IT_s) {
        (void) copyHelper (IT_s);
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var () {
        _ptr = NULL;
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var (_IDL_SEQUENCE_Marshal_Marshal_Recursive *IT_p) {
        _ptr = IT_p;
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &operator= (_IDL_SEQUENCE_Marshal_Marshal_Recursive *IT_p) {
        if (_ptr != IT_p) {
            delete _ptr;
        }
        _ptr = IT_p;
        return (*this);
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &operator= (const _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &IT_s) {
        if (_ptr != IT_s._ptr) {
            delete _ptr;
        }
        _ptr = new _IDL_SEQUENCE_Marshal_Marshal_Recursive;
        *(_ptr) =  *(IT_s._ptr);
        return (*this);
    }

    ~_IDL_SEQUENCE_Marshal_Marshal_Recursive_var () {
        delete _ptr;
    }

    _IDL_SEQUENCE_Marshal_Marshal_Recursive* operator-> () {
        return _ptr;
    }

    operator _IDL_SEQUENCE_Marshal_Marshal_Recursive_cvPtr () const { return _ptr;}
    operator _IDL_SEQUENCE_Marshal_Marshal_Recursive_vPtr& () { return _ptr;}
    operator _IDL_SEQUENCE_Marshal_Marshal_Recursive& () const { return * _ptr;}

    const Marshal::Marshal_Recursive& operator[] (CORBA::ULong index) const;

    Marshal::Marshal_Recursive& operator[] (CORBA::ULong index);

    protected:
    _IDL_SEQUENCE_Marshal_Marshal_Recursive *_ptr;
    private:
    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var &operator= (const CORBA::_var &IT_s);
    _IDL_SEQUENCE_Marshal_Marshal_Recursive_var (const CORBA::_var &IT_s);
};

#endif


#endif


void operator<<= (CORBA::any &IT_a, const Marshal::Marshal_Union& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Marshal::Marshal_Union*& IT_t);


void operator<<= (CORBA::any &IT_a, const Marshal::Marshal_Recursive& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Marshal::Marshal_Recursive*& IT_t);


void operator<<= (CORBA::any &IT_a, Marshal_ptr IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Marshal_ptr& IT_t);


void operator<<= (CORBA::any &IT_a, const Marshal::_IDL_SEQUENCE_Marshal_Marshal_Recursive& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Marshal::_IDL_SEQUENCE_Marshal_Marshal_Recursive*& IT_t);


void operator<<= (CORBA::any &IT_a, const _IDL_SEQUENCE_Marshal_Marshal_Recursive& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, _IDL_SEQUENCE_Marshal_Marshal_Recursive*& IT_t);


void operator<<= (CORBA::any &IT_a, Marshal::discrim IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Marshal::discrim& IT_t);


void operator<<= (CORBA::any &IT_a, const _IDL_SEQUENCE_any& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, _IDL_SEQUENCE_any*& IT_t);


void operator<<= (CORBA::any &IT_a, const Marshal::Marshal_Struct& IT_t);
CORBA::Boolean operator>>= (const CORBA::any &IT_a, Marshal::Marshal_Struct*& IT_t);


#endif

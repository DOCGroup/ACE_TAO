

#include "CosNaming.hh"


#define CosNaming_NamingContext_dispatch_impl

unsigned char CosNaming::NamingContext_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"bind")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~bind~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},+obj{O~},>{v},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            CORBA::Object_ptr obj;

            obj = (CORBA::ObjectRef) IT_r.decodeObjRef ("CORBA::Object");

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->bind (  n,   obj, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            CORBA::release (obj);

            if (!IT_r.isException (IT_env)) {

                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_AlreadyBound)) {
                    ((CosNaming::NamingContext::AlreadyBound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"rebind")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~rebind~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},+obj{O~},>{v},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            CORBA::Object_ptr obj;

            obj = (CORBA::ObjectRef) IT_r.decodeObjRef ("CORBA::Object");

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->rebind (  n,   obj, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            CORBA::release (obj);

            if (!IT_r.isException (IT_env)) {

                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"bind_context")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~bind_context~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},+nc{O~CosNaming::NamingContext},>{v},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            CosNaming::NamingContext_ptr nc;

            nc = (CosNaming::NamingContext*) IT_r.decodeObjRef (CosNaming_NamingContext_IR);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->bind_context (  n,   nc, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (nc) CosNaming::NamingContext_release (nc);

            if (!IT_r.isException (IT_env)) {

                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_AlreadyBound)) {
                    ((CosNaming::NamingContext::AlreadyBound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"rebind_context")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~rebind_context~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},+nc{O~CosNaming::NamingContext},>{v},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            CosNaming::NamingContext_ptr nc;

            nc = (CosNaming::NamingContext*) IT_r.decodeObjRef (CosNaming_NamingContext_IR);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->rebind_context (  n,   nc, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (nc) CosNaming::NamingContext_release (nc);

            if (!IT_r.isException (IT_env)) {

                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"resolve")) {
            CORBA::Object_ptr IT_result;
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~resolve~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},>{O~},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    	IT_result = ((CosNaming::NamingContext*)IT_pp)->resolve (  n, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
O~\
", IT_env)) return 1;
                {
                    CORBA::Object* IT_l0 = (CORBA::Object*) IT_result;
                    IT_r << IT_l0;
                }
                CORBA::release (IT_result);
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"unbind")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~unbind~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},>{v},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->unbind (  n, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"new_context")) {
            CosNaming::NamingContext_ptr IT_result;
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~new_context~>{O~CosNaming::NamingContext},N{}\
"))
                return 1;

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    	IT_result = ((CosNaming::NamingContext*)IT_pp)->new_context (IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
O~CosNaming::NamingContext\
", IT_env)) return 1;
                {
                    CORBA::Object* IT_l0 = (CORBA::Object*) IT_result;
                    IT_r << IT_l0;
                }
                if (IT_result) CosNaming::NamingContext_release (IT_result);
                }

            else IT_r.makeSystemException (IT_env);
            return 1;
    }

    if (!strcmp(IT_s,"bind_new_context")) {
            CosNaming::NamingContext_ptr IT_result;
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~bind_new_context~+n{S{R~CosNaming::NameComponent~id{0},kind{0}},0},>{O~CosNaming::NamingContext},N{}\
"))
                return 1;
            CosNaming::Name n;

            n.decodeOp (IT_r);

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    	IT_result = ((CosNaming::NamingContext*)IT_pp)->bind_new_context (  n, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
O~CosNaming::NamingContext\
", IT_env)) return 1;
                {
                    CORBA::Object* IT_l0 = (CORBA::Object*) IT_result;
                    IT_r << IT_l0;
                }
                if (IT_result) CosNaming::NamingContext_release (IT_result);
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotFound)) {
                    ((CosNaming::NamingContext::NotFound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_AlreadyBound)) {
                    ((CosNaming::NamingContext::AlreadyBound*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_CannotProceed)) {
                    ((CosNaming::NamingContext::CannotProceed*)IT_env.exception_value()) -> encodeOp (IT_r);
                }
    else
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_InvalidName)) {
                    ((CosNaming::NamingContext::InvalidName*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"destroy")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~destroy~>{v},N{}\
"))
                return 1;

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->destroy (IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;
                }
            else {
                IT_r.convertToException (IT_env);
                if (!strcmp (IT_env.exception_id(), CosNaming::NamingContext::ex_NotEmpty)) {
                    ((CosNaming::NamingContext::NotEmpty*)IT_env.exception_value()) -> encodeOp (IT_r);
                }

                else IT_r.makeSystemException (IT_env);
            }
            return 1;
    }

    if (!strcmp(IT_s,"list")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~list~+how_many{ul},-bl{S{R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}},0},-bi{O~CosNaming::BindingIterator},>{v},N{}\
"))
                return 1;
            CORBA::ULong how_many;

            IT_r >> how_many;

            CosNaming::BindingList* bl = NULL;

            CosNaming::BindingIterator_ptr bi;

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::NamingContext*)IT_pp)->list (  how_many,  bl,  bi, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }



            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
v\
", IT_env)) return 1;

                if (!bl) {
                    IT_r.makeRuntimeException6 ();
                    return 1;
                }
                else

                (*bl).encodeOp (IT_r);
                delete bl;


                {
                    CORBA::Object* IT_l0 = (CORBA::Object*) bi;
                    IT_r << IT_l0;
                }
                if (bi) CosNaming::BindingIterator_release (bi);
                }

            else IT_r.makeSystemException (IT_env);
            return 1;
    }

    if (IT_isTarget)
            IT_r.makeRuntimeException2 ();

    return 0;
}

#define CosNaming_BindingIterator_dispatch_impl

unsigned char CosNaming::BindingIterator_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"next_one")) {
            CORBA::Boolean IT_result;
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~next_one~-b{R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}},>{b},N{}\
"))
                return 1;
            CosNaming::Binding* b = NULL;

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    	IT_result = ((CosNaming::BindingIterator*)IT_pp)->next_one ( b, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }

            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
b\
", IT_env)) return 1;
                if (!b) {
                    IT_r.makeRuntimeException6 ();
                    return 1;
                }
                else

                (*b).encodeOp (IT_r);
                delete b;
                IT_r << IT_result;
                }

            else IT_r.makeSystemException (IT_env);
            return 1;
    }

    if (!strcmp(IT_s,"next_n")) {
            CORBA::Boolean IT_result;
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~next_n~+how_many{ul},-bl{S{R~CosNaming::Binding~binding_name{S{R~CosNaming::NameComponent~id{0},kind{0}},0},binding_type{E~CosNaming::BindingType~nobject,ncontext}},0},>{b},N{}\
"))
                return 1;
            CORBA::ULong how_many;

            IT_r >> how_many;

            CosNaming::BindingList* bl = NULL;

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    	IT_result = ((CosNaming::BindingIterator*)IT_pp)->next_n (  how_many,  bl, IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }


            if (!IT_r.isException (IT_env)) {
                if (!IT_r.convertToReply ("\
b\
", IT_env)) return 1;

                if (!bl) {
                    IT_r.makeRuntimeException6 ();
                    return 1;
                }
                else

                (*bl).encodeOp (IT_r);
                delete bl;
                IT_r << IT_result;
                }

            else IT_r.makeSystemException (IT_env);
            return 1;
    }

    if (!strcmp(IT_s,"destroy")) {
            CORBA::Environment IT_env (IT_r);
            CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
            if (!IT_r.tcAssert ("\
Ro~destroy~>{v},N{}\
"))
                return 1;

            if (IT_f && !IT_r.isException (IT_env))
            	IT_f->inRequestPostM (IT_r, IT_env);
            if (!IT_r.isException (IT_env)) {
                try {
                    ((CosNaming::BindingIterator*)IT_pp)->destroy (IT_env);

                } catch (CORBA::SystemException &IT_exSys) {
                IT_env.propagate (&IT_exSys);
                } catch (CORBA::UserException &IT_exUsr) {
                IT_env.propagate (&IT_exUsr);
                }
            }
            IT_r.replyNoResults (IT_env);
            return 1;
    }

    if (IT_isTarget)
            IT_r.makeRuntimeException2 ();

    return 0;
}

#include "CosNamingC.cpp"


#include "cubit.h"


#define Cubit_dispatch_impl

unsigned char Cubit_dispatch::dispatch (CORBA::Request &IT_r, 
 unsigned char IT_isTarget, void *IT_pp) {
    if (!IT_pp)
       IT_pp = m_obj;
    const char *IT_s = IT_r.getOperation ();
    if (!strcmp(IT_s,"cube_octet")) {
        CORBA::Octet IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~cube_octet~+o{o},>{o},N{}\
"))
            return 1;
        CORBA::Octet o;

        IT_r.extractOctet (o);

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env)) {
            try {
                	IT_result = ((Cubit*)IT_pp)->cube_octet (  o, IT_env);

            } catch (CORBA::SystemException &IT_exSys) {
            IT_env.propagate (&IT_exSys);
            } catch (CORBA::UserException &IT_exUsr) {
            IT_env.propagate (&IT_exUsr);
            }
        }

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
o\
", IT_env)) return 1;
            IT_r.insertOctet (IT_result);
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    if (!strcmp(IT_s,"cube_short")) {
        CORBA::Short IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~cube_short~+s{s},>{s},N{}\
"))
            return 1;
        CORBA::Short s;

        IT_r >> s;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env)) {
            try {
                	IT_result = ((Cubit*)IT_pp)->cube_short (  s, IT_env);

            } catch (CORBA::SystemException &IT_exSys) {
            IT_env.propagate (&IT_exSys);
            } catch (CORBA::UserException &IT_exUsr) {
            IT_env.propagate (&IT_exUsr);
            }
        }

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
s\
", IT_env)) return 1;
            IT_r << IT_result;
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    if (!strcmp(IT_s,"cube_long")) {
        CORBA::Long IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~cube_long~+l{l},>{l},N{}\
"))
            return 1;
        CORBA::Long l;

        IT_r >> l;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env)) {
            try {
                	IT_result = ((Cubit*)IT_pp)->cube_long (  l, IT_env);

            } catch (CORBA::SystemException &IT_exSys) {
            IT_env.propagate (&IT_exSys);
            } catch (CORBA::UserException &IT_exUsr) {
            IT_env.propagate (&IT_exUsr);
            }
        }

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
l\
", IT_env)) return 1;
            IT_r << IT_result;
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    if (!strcmp(IT_s,"cube_struct")) {
        Cubit::Many IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~cube_struct~+values{R~Cubit::Many~o{o},l{l},s{s}},>{R~Cubit::Many~o{o},l{l},s{s}},N{}\
"))
            return 1;
        Cubit::Many values;

        values.decodeOp (IT_r);

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env)) {
            try {
                	IT_result = ((Cubit*)IT_pp)->cube_struct (  values, IT_env);

            } catch (CORBA::SystemException &IT_exSys) {
            IT_env.propagate (&IT_exSys);
            } catch (CORBA::UserException &IT_exUsr) {
            IT_env.propagate (&IT_exUsr);
            }
        }

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
R~Cubit::Many~o{o},l{l},s{s}\
", IT_env)) return 1;
            IT_result.encodeOp (IT_r);
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    if (!strcmp(IT_s,"cube_union")) {
        Cubit::oneof IT_result;
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        IT_r.tcPreAssert (
"\
Ro~cube_union~+values{U~Cubit::oneof~{E~Cubit::discrim~e_0th,e_1st,e_2nd,e_3rd,e_4th,e_5th}~Cubit::e_0th o{o};Cubit::e_1st s{s};Cubit::e_2nd l{l};Cubit::e_3rd,$ cm{R~Cubit::Many~o{o},l{l},s{s}}},>{U~Cubit::oneof~{E~Cubit::discrim~e_0th,e_1st,e_2nd,e_3rd,\
");
        if (!IT_r.tcAssert ("\
e_4th,e_5th}~Cubit::e_0th o{o};Cubit::e_1st s{s};Cubit::e_2nd l{l};Cubit::e_3rd,$ cm{R~Cubit::Many~o{o},l{l},s{s}}},N{}\
"))
            return 1;
        Cubit::oneof values;

        values.decodeOp (IT_r);

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env)) {
            try {
                	IT_result = ((Cubit*)IT_pp)->cube_union (  values, IT_env);

            } catch (CORBA::SystemException &IT_exSys) {
            IT_env.propagate (&IT_exSys);
            } catch (CORBA::UserException &IT_exUsr) {
            IT_env.propagate (&IT_exUsr);
            }
        }

        if (!IT_r.isException (IT_env)) {
            if (!IT_r.convertToReply ("\
U~Cubit::oneof~{E~Cubit::discrim~e_0th,e_1st,e_2nd,e_3rd,e_4th,e_5th}~Cubit::e_0th o{o};Cubit::e_1st s{s};Cubit::e_2nd l{l};Cubit::e_3rd,$ cm{R~Cubit::Many~o{o},l{l},s{s}}\
", IT_env)) return 1;
            IT_result.encodeOp (IT_r);
            }

        else IT_r.makeSystemException (IT_env);
        return 1;
    }

    if (!strcmp(IT_s,"please_exit")) {
        CORBA::Environment IT_env (IT_r);
        CORBA::Filter* IT_f = CORBA::Orbix.getFilter ();
        if (!IT_r.tcAssert ("\
Ro~please_exit~>{v},O{}\
"))
            return 1;

        if (IT_f && !IT_r.isException (IT_env))
        	IT_f->inRequestPostM (IT_r, IT_env);
        if (!IT_r.isException (IT_env)) {
            try {
                ((Cubit*)IT_pp)->please_exit (IT_env);

            } catch (CORBA::SystemException &IT_exSys) {
            IT_env.propagate (&IT_exSys);
            } catch (CORBA::UserException &IT_exUsr) {
            IT_env.propagate (&IT_exUsr);
            }
        }
        IT_r.replyNoResults (CORBA::Flags(CORBA::INV_NO_RESPONSE),IT_env);
        return 1;
    }

    if (IT_isTarget)
        IT_r.makeRuntimeException2 ();

    return 0;
}

#include "cubitC.cpp"


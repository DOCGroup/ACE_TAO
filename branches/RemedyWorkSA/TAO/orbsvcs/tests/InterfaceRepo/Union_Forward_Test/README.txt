$Id$

This test illustrates a problem with the IFR service in that it cannot
properly handle forward declared unions.

To validate that this test case is correct as written, modify the test.idl
so that the sequence's typedef is a string rather than the forward declared
union. See that runtest.pl terminates normally.

Second, the IDL may be compiled by the TAO IDL compiler, demonstrating that
it is well-formed IDL.

Here is a stack trace relative to the IFR service in TAO 1.6.6:

#0  0x00007f68cc421808 in TAO_SequenceDef_i::element_type_i (this=0x73b680)
    at IFRService/SequenceDef_i.cpp:144
#1  0x00007f68cc4221c4 in TAO_SequenceDef_i::type_i (this=0x73b680)
    at IFRService/SequenceDef_i.cpp:75
#2  0x00007f68cc36b945 in TAO_AliasDef_i::type_i (this=0x671f10)
    at IFRService/AliasDef_i.cpp:70
#3  0x00007f68cc42a009 in TAO_UnionDef_i::members_i (this=0x719eb0)
    at IFRService/UnionDef_i.cpp:293
#4  0x00007f68cc42a70a in TAO_UnionDef_i::type_i (this=0x719eb0)
    at IFRService/UnionDef_i.cpp:104
#5  0x00007f68cc37058e in TAO_AttributeDef_i::type_i (this=0x672110)
    at IFRService/AttributeDef_i.cpp:91
#6  0x00007f68cc370803 in TAO_AttributeDef_i::type (this=0x672110)
    at IFRService/AttributeDef_i.cpp:76
#7  0x00007f68cc40af7c in POA_CORBA::AttributeDef_tie<TAO_AttributeDef_i>::type
    (this=0x672150) at ../../orbsvcs/orbsvcs/IFRService/IFR_BasicS_T.inl:5355
#8  0x00007f68cc4a4b75 in POA_CORBA::_get_type_AttributeDef::execute (
    this=0x7fffd4cdd230) at IFRService/IFR_BasicS.cpp:14059
#9  0x00007f68cb02ed1b in TAO::Upcall_Wrapper::upcall (this=0x7fffd4cdd27f,
    server_request=@0x7fffd4cdda90, args=0x7fffd4cdd270, nargs=1,
    command=@0x7fffd4cdd230, servant_upcall=0x7fffd4cdd3d0, exceptions=0x0,
    nexceptions=0) at PortableServer/Upcall_Wrapper.cpp:113
#10 0x00007f68cc47dc25 in POA_CORBA::AttributeDef::_get_type_skel (
    server_request=@0x7fffd4cdda90, servant_upcall=0x7fffd4cdd3d0,
    servant=0x672150) at IFRService/IFR_BasicS.cpp:14111


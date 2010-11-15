// $Id$

#ifndef NT_CONNECTOR_T
#define NT_CONNECTOR_T


// connector template for Typed_NonTopic_Test_1
// in this example V is fixed length type variable, so no bool FIXED

template <typename CCM_TYPE, T, bool FIXED, U, bool FIXED, V, Tseq, Useq, VSeq>
NT_Connector_T<CCM_TYPE, T, FIXED, U, FIXED, V, TSeq, YSeq, VSeq>::NT_Connector_T (void) :
{
}


// connector template for Typed_NonTopic_Test_2
template <typename CCM_TYPE, T, bool FIXED, U, bool FIXED, Tseq, Useq>
NT_Connector_T<CCM_TYPE, T, FIXED, U, FIXED, TSeq, YSeq>::NT_Connector_T (void) :
{
}

#endif

// $Id$

#include <openssl/asn1.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>


DSA *
DSAparams_dup_wrapper (DSA *dsa)
{
	return DSAparams_dup (dsa);
}

DH *
DHparams_dup_wrapper (DH *dh)
{
	return DHparams_dup (dh);
}

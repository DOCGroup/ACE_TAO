// This may look like C, but it's really -*- C++ -*-
//
//
// utility template class
//
#if !defined(TAO_SEQUENCE_H)
#  define TAO_SEQUENCE_H

template <class T>
struct CORBA_SEQUENCE {
#if	SIZEOF_LONG == 4
    unsigned long	maximum;
    unsigned long	length;
#else
    // just assume "int" is four bytes long ...
    unsigned		maximum;
    unsigned		length;
#endif
    T			*buffer;

			CORBA_SEQUENCE ()
			: maximum (0), length (0), buffer (0) { }

			// XXX destructor should free buffer, elements!!
			~CORBA_SEQUENCE () { }
};

typedef CORBA_SEQUENCE <CORBA_Octet>	CORBA_OctetSeq;

#endif

// RandomGenerator.h: interface for the CRandomGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMGENERATOR_H__AB45B41B_98D1_489A_8F0C_E0243C8F122E__INCLUDED_)
#define AFX_RANDOMGENERATOR_H__AB45B41B_98D1_489A_8F0C_E0243C8F122E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <math.h>
#include <time.h>
#include <vector>


/*! \brief Normalized and Exponential random number generator

\par Normal distribution

	- RNOR in any expression will provide a standard normal variate with mean zero, variance 1.
	- TNOR will provide the theoretical value

 	The normal distribution is characterized by a probability density function
	\f[ y=\frac{1}{\sigma \sqrt{2 \pi}}e^{-\frac{(x-\mu)^2}{2 \sigma^2}}\f]
 
\par Exponential distribtion  
	- REXP in any expression will provide an exponential variate
with density
	- TEXP will provide the theoretical value

	The exponential distribution is characterized by a probability density function 
	\f[ f(x) = \mu e^{\mu x} \f]

\par Random generator initialization

	Before using #RNOR or #REXP in your application, construct a #CRandomGenerator
object. It will seed the generator, by default (unsigned long)time( NULL ) is used but you cand
give your own choice of seed >0. (If you do not invoke CRandomGenerator(...), you will get all zeros for RNOR and REXP.)

\par Example:

\code 
// build an object somewhere, 
// In CWinApp::InitInstance for example
CRandomGenerator randn;

// anywhere, for normal distribution
CRandomGenerator::RNOR();
// and for exponential distribution
CRandomGenerator::REXP();
\endcode

\par Reference

  For details on the methods #RNOR and #REXP, see Marsaglia and Tsang, "The ziggurat method
for generating random variables", Journ. Statistical Software.

\author Jonathan de Halleux, dehalleux@auto.ucl.ac.be, 2002 (putting it together)
*/
class CRandomGenerator  
{
public:

	/*! Constructor
	The consturctor seeds the generator with jsreed.

	By default, jsreed = (unsigned long)time( NULL )
	*/
	CRandomGenerator( unsigned long jsreed = (unsigned long)time( NULL ) );
	virtual ~CRandomGenerator(){};

	/*! \brief Standard normal variate with mean zero, variance 1,

	\param fAve mean, \f$\mu\f$,
	\param fSdev standard deviation, \f$\sigma\f$

	*/
	static float RNOR( float fAve = 0, float fSdev = 1) 
	{ 
		hz=SHR3(); 
		iz=hz&127;
		float x=(abs(hz)<kn[iz])? hz*wn[iz] : nfix();
		return  x*fSdev + fAve ;
	};
	
	/*! \brief Estimates the theoretical probability density function at x

	\param x position where to estimate probability
	\param fAve mean, \f$\mu\f$,
	\param fSdev standard deviation, \f$\sigma\f$

	The normal distribution is characterized by a probability density function
	\f[ f(x)=\frac{1}{\sigma \sqrt{2 \pi}}e^{-\frac{(x-\mu)^2}{2 \sigma^2}}\f]
	*/
	static double TNOR(double x, double fAve = 0, double fSdev = 1)
	{
		return exp ( -( x - fAve )*( x - fAve )/(2*fSdev*fSdev) ) /(fSdev*sqrt(2*PGL_PI));
	}

	/*! \brief exponential variate

	The exponential distribution is characterized by a probability density function 
	\f[ f(x) = \mu e^{-\mu x} \f]
	where \f$\mu=1\f$.
	*/
	static float REXP() 
	{ 
		jz=SHR3(); 
		iz=jz&255; 
		return (jz <ke[iz])? jz*we[iz] : efix();
	};

	/*! \brief Estimates the theoretical probability density function at x

	\param x value where to estimate probability
	\param fAve average of distribution, \f$\mu\f$,

	The exponential distribution is characterized by a probability density function 
	\f[ f(x) = \mu e^{-\mu x} \f]
	*/
	static double TEXP( double x, double fAve)
	{
		return exp(-fAve*x)/fAve;
	}

private:

	//! This procedure sets the seed and creates the tables
	static void zigset(unsigned long jsrseed);
	//! generates variates from the residue when rejection in RNOR occurs.
	static float nfix(void);
	//! Generates variates from the residue when rejection in REXP occurs. 
	static float efix(void);

private:

	static unsigned long SHR3() {	jz=jsr; jsr^=(jsr<<13); jsr^=(jsr>>17); jsr^=(jsr<<5); return jz+jsr;};
	static float UNI()			{ return .5 + (signed)SHR3() * .2328306e-9;};

private:
	static unsigned long jz;
	static unsigned long jsr;
	static long hz;
	static unsigned long iz;
	static unsigned long kn[128];
	static unsigned long ke[256];
	static float wn[128];
	static float fn[128];
	static float we[256];
	static float fe[256];
};

#endif // !defined(AFX_RANDOMGENERATOR_H__AB45B41B_98D1_489A_8F0C_E0243C8F122E__INCLUDED_)

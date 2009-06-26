// Histogram.h: interface for the THistogram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTOGRAM_H__DF74A789_2878_4CB0_BAA9_125272F0E7E1__INCLUDED_)
#define AFX_HISTOGRAM_H__DF74A789_2878_4CB0_BAA9_125272F0E7E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <valarray>

/*! \brief A histogram class

  This class computes the histogram of a vector.

\par Template parameters
	
	- T type of input data (can be any: float, double, int, UINT, etc...)
	- TOut type of output data: float or double. (default is double)

\par Moments:

	The moments (average, standard deviation, skewness, etc.) are computed using 
the algorithm of the Numerical recipies (see Numerical recipies in C, Chapter 14.1, pg 613).

\par Example:

	This example shows the typical use of the class:
\code
	// a vector containing the data
	vector<float> vData;
	// Creating histogram using float data and with 101 containers,
	THistogram<float> histo(101);
	// computing the histogram
	histo.Compute(vData);
\endcode

Once this is done, you can get a vector with the histogram or the normalized histogram (such that it's area is 1):
\code
	// getting normalized histogram
	vector<float> v=histo.GetNormalizedHistogram();
\endcode

\par Reference
	
	Equally spaced acsissa function integration (used in #GetArea): Numerical Recipies in C, Chapter 4.1, pg 130.

\author Jonathan de Halleux, dehalleux@auto.ucl.ac.be, 2002
*/

template<class T, class TOut = double>
class THistogram  
{
public:
	//! \name Constructors
	//@{
	/*! Default constructor
	\param nCounters the number of histogram containers (default value is 10)
	*/
	THistogram(UINT nCounters = 10);
	virtual ~THistogram()				{ Clear();};
	//@}

	//! \name Histogram computation, update
	//@{
	/*! Computes histogram of vector v
	\param v vector to compute histogram
	\param bComputeMinMax set to true if min/max of v have to be used to get the histogram limits	
	
	This function computes the histogram of v and stores it internally.
	\sa Update, GetHistogram
	*/
	void Compute( const std::vector<T>& v, bool bComputeMinMax = true);
	//! Update histogram with the vector v
	void Update( const std::vector<T>& v);
	//! Update histogram with t
	void Update( const T& t);
	//@}

	//! \name Resetting functions
	//@{
	//! Resize the histogram. Warning this function clear the histogram.
	void Resize( UINT nCounters );
	//! Clears the histogram
	void Clear()						{ m_vCounters.clear();};
	//@}

	//! \name Setters
	//@{
	/*! This function sets the minimum of the histogram spectrum. 
	The spectrum is not recomputed, use it with care
	*/
	void SetMinSpectrum( const T& tMin )					{	m_tMin = tMin; ComputeStep();};
	/*! This function sets the minimum of the histogram spectrum. 
	The spectrum is not recomputed, use it with care
	*/
	void SetMaxSpectrum( const T& tMax )					{	m_tMax = tMax; ComputeStep();};
	//@}
	//! \name Getters
	//@{
	//! return minimum of histogram spectrum
	const T& GetMinSpectrum() const							{	return m_tMin;};
	//! return maximum of histogram spectrum
	const T& GetMaxSpectrum() const							{	return m_tMax;};
	//! return step size of histogram containers
	TOut GetStep() const									{	return m_dStep;};
	//! return number of points in histogram
	UINT GetSum() const;
	/*! \brief returns area under the histogram 

		The Simpson rule is used to integrate the histogram.
	*/
	TOut GetArea() const;

	/*! \brief Computes the moments of the histogram

	\param vData dataset
	\param fAve mean
	\f[ \bar x = \frac{1}{N} \sum_{j=1}^N x_j\f]
	\param fAdev mean absolute deviation
	\f[ Adev(X) = \frac{1}{N} \sum_{j=1}^N | x_j - \bar x |\f]
	\param fVar average deviation:
	\f[ \mbox{Var}(X) = \frac{1}{N-1} \sum_{j=1}^N (x_j - \bar x)^2\f]
	\param fSdev standard deviation:
	\f[ \sigma(X) = \sqrt{Var(\bar x) }\f]
	\param fSkew skewness
	\f[ \mbox{Skew}(X) = \frac{1}{N}\sum_{j=1}^N \left[ \frac{x_j - \bar x}{\sigma}\right]^3\f]
	\param fKurt kurtosis
	\f[ \mbox{Kurt}(X) = \left\{ \frac{1}{N}\sum_{j=1}^N \left[ \frac{x_j - \bar x}{\sigma}\right]^4 \right\} - 3\f]

	*/
	static void GetMoments(const std::vector<T>& vData, TOut& fAve, TOut& fAdev, TOut& fSdev, TOut& fVar, TOut& fSkew, TOut& fKurt);

	//! return number of containers
	UINT GetSize() const							{	return m_vCounters.size();};
	//! returns i-th counter
	UINT operator [] (UINT i) const					{	ASSERT( i < m_vCounters.size() ); return m_vCounters[i];};
	//! return the computed histogram
	const std::vector<UINT>& GetHistogram() const	{	return m_vCounters;};
	//! return the computed histogram, in TOuts
	std::vector<TOut> GetHistogramD() const;
	/*! return the normalized computed histogram 

	\return the histogram such that the area is equal to 1
	*/
	std::vector<TOut> GetNormalizedHistogram() const;
	//! returns left containers position
	std::vector<TOut> GetLeftContainers() const;
	//! returns center containers position
	std::vector<TOut> GetCenterContainers() const;
	//@}
protected:
	//! Computes the step
	void ComputeStep()								{	m_dStep = (TOut)(((TOut)(m_tMax-m_tMin)) / (m_vCounters.size()-1));};
	//! Data accumulators
	std::vector<UINT> m_vCounters;
	//! minimum of dataset
	T m_tMin;
	//! maximum of dataset
	T m_tMax;
	//! width of container
	TOut m_dStep;
};

template<class T, class TOut>
THistogram<T,TOut>::THistogram<T,TOut>(UINT nCounters)
: m_vCounters(nCounters,0), m_tMin(0), m_tMax(0), m_dStep(0)
{

}

template<class T, class TOut>
void THistogram<T,TOut>::Resize( UINT nCounters )
{
	Clear();

	m_vCounters.resize(nCounters,0);

	ComputeStep();
}

template<class T, class TOut>
void THistogram<T,TOut>::Compute( const std::vector<T>& v, bool bComputeMinMax)
{
	using namespace std;
	UINT i;
	int index;

	if (m_vCounters.empty())
		return;

	if (bComputeMinMax)
	{
		m_tMax = m_tMin = v[0];
		for (i=1;i<v.size();i++)
		{
			m_tMax = __max( m_tMax, v[i]);
			m_tMin = __min( m_tMin, v[i]);
		}
	}

	ComputeStep();

	for (i = 0;i < v.size() ; i++)
	{
		index=(int) floor( ((TOut)(v[i]-m_tMin))/m_dStep ) ;

		if (index >= m_vCounters.size() || index < 0)
			return;

		m_vCounters[index]++;
	}
}

template<class T, class TOut>
void THistogram<T,TOut>::Update( const std::vector<T>& v)
{
	if (m_vCounters.empty())
		return;

	ComputeStep();

	TOut uSize = m_vCounters.size();

	int index;
	for (UINT i = 0;i < uSize ; i++)
	{
		index = (int)floor(((TOut)(v[i]-m_tMin))/m_dStep);

		if (index >= m_vCounters.size() || index < 0)
			return;

		m_vCounters[index]++;
	}
}

template<class T, class TOut>
void THistogram<T,TOut>::Update( const T& t)
{	
	int index=(int) floor( ((TOut)(t-m_tMin))/m_dStep ) ;

	if (index >= m_vCounters.size() || index < 0)
		return;

	m_vCounters[index]++;
};

template<class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::GetHistogramD() const
{
	std::vector<TOut> v(m_vCounters.size());
	for (UINT i = 0;i<m_vCounters.size(); i++)
		v[i]=(TOut)m_vCounters[i];

	return v;
}

template <class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::GetLeftContainers() const
{
	std::vector<TOut> vX( m_vCounters.size());

	for (UINT i = 0;i<m_vCounters.size(); i++)
		vX[i]= m_tMin + i*m_dStep;

	return vX;
}

template <class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::GetCenterContainers() const
{
	std::vector<TOut> vX( m_vCounters.size());

	for (UINT i = 0;i<m_vCounters.size(); i++)
		vX[i]= m_tMin + (i+0.5)*m_dStep;

	return vX;
}

template <class T, class TOut>
UINT THistogram<T,TOut>::GetSum() const
{
	UINT uSum = 0;
	for (UINT i = 0;i<m_vCounters.size(); i++)
		uSum+=m_vCounters[i];

	return uSum;
}

template <class T, class TOut>
TOut THistogram<T,TOut>::GetArea() const
{
	const size_t n=m_vCounters.size();
	TOut area=0;

	if (n>6)
	{
		area=3.0/8.0*(m_vCounters[0]+m_vCounters[n-1])
		+7.0/6.0*(m_vCounters[1]+m_vCounters[n-2])
		+23.0/24.0*(m_vCounters[2]+m_vCounters[n-3]);
		for (UINT i=3;i<n-3;i++)
		{
			area+=m_vCounters[i];
		}
	}
	else if (n>4)
	{
		area=5.0/12.0*(m_vCounters[0]+m_vCounters[n-1])
		+13.0/12.0*(m_vCounters[1]+m_vCounters[n-2]);
		for (UINT i=2;i<n-2;i++)
		{
			area+=m_vCounters[i];
		}
	}
	else if (n>1)
	{
		area=1/2.0*(m_vCounters[0]+m_vCounters[n-1]);
		for (UINT i=1;i<n-1;i++)
		{
			area+=m_vCounters[i];
		}
	}
	else 
		area=0;

	return area*m_dStep;
}

template <class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::GetNormalizedHistogram() const
{
	std::vector<TOut> vNormCounters( m_vCounters.size());
	TOut dArea = (TOut)GetArea();

	for (UINT i = 0;i<m_vCounters.size(); i++)
	{
		vNormCounters[i]= (TOut)m_vCounters[i]/dArea;
	}

	return vNormCounters;
};

template <class T, class TOut>
void THistogram<T,TOut>::GetMoments(const std::vector<T>& vData, TOut& fAve, TOut& fAdev, TOut& fSdev, TOut& fVar, TOut& fSkew, TOut& fKurt)
{
	int j;
	double ep=0.0,s,p;
	const size_t n = vData.size();

	if (n <= 1)
		// nrerror("n must be at least 2 in moment");
		return;

	s=0.0; // First pass to get the mean.
	for (j=0;j<n;j++)
		s += vData[j];
	
	fAve=s/(n);
	fAdev=fVar=fSkew=fKurt=0.0; 
	/* Second pass to get the first (absolute), second,
	third, and fourth moments of the
	deviation from the mean. */

		for (j=0;j<n;j++) 
		{
			fAdev += fabs(s=vData[j]-(fAve));
			ep += s;
			fVar += (p=s*s);
			fSkew += (p *= s);
			fKurt += (p *= s);
		}


	fAdev /= n;
	fVar=(fVar-ep*ep/n)/(n-1); // Corrected two-pass formula.
	fSdev=sqrt(fVar); // Put the pieces together according to the conventional definitions. 
	if (fVar) 
	{
		fSkew /= (n*(fVar)*(fSdev));
		fKurt=(fKurt)/(n*(fVar)*(fVar))-3.0;
	} 
	else
		//nrerror("No skew/kurtosis when variance = 0 (in moment)");
		return;
}

#endif // !defined(AFX_HISTOGRAM_H__DF74A789_2878_4CB0_BAA9_125272F0E7E1__INCLUDED_)

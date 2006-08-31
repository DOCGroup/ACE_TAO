// MaskedBitmap.h: interface for the CMaskedBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASKEDBITMAP_H__009027CF_EB1E_4263_8F5C_189DD8054736__INCLUDED_)
#define AFX_MASKEDBITMAP_H__009027CF_EB1E_4263_8F5C_189DD8054736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaskedBitmap
{
public:
	CMaskedBitmap();
	~CMaskedBitmap();

	BOOL  IsValid()   const	{ return (m_pBits != NULL); }

	int Width() const;
	int Height() const;

	DWORD Read(CFile& file, BOOL bFromResource = FALSE );
	DWORD ReadFromResource(UINT nResID);

	void DrawTransparent (CDC* pDC, const CRect &rect, 
        COLORREF clrTransparency, bool bGray, COLORREF grayColor);
	void DrawTransparent (CDC* pDC, int x, int y, 
        COLORREF clrTransparency, bool bGray, COLORREF grayColor);
  void Draw (CDC* pDC, const CRect &rect);
	void Draw (CDC* pDC, int x, int y);

protected:
	WORD  NumColors(  BITMAPINFOHEADER& bmiHeader ) const;
	void Free();
	BOOL  CreatePalette();

protected:
	LPBYTE m_pBits;
	LPBITMAPINFO m_pBMI;
	CPalette	*m_pPalette;

};

#endif // !defined(AFX_MASKEDBITMAP_H__009027CF_EB1E_4263_8F5C_189DD8054736__INCLUDED_)

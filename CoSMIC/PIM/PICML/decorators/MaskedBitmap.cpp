// MaskedBitmap.cpp: implementation of the CMaskedBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MaskedBitmap.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// DIB support defines
#define BMIH_SIZE sizeof BITMAPINFOHEADER
#define BMIF_SIZE sizeof BITMAPFILEHEADER

/* DIB constants */
#define PALVERSION   0x300

/* Dib Header Marker - used in writing DIBs to files */
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

/////////////////////////////////////////////////////////////////////////////


static const int MAX_PORT_LENGTH 	= 3;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaskedBitmap::CMaskedBitmap() : m_pBits(0), m_pBMI(0), m_pPalette(0)
{
}

CMaskedBitmap::~CMaskedBitmap()
{
	Free();
}

void CMaskedBitmap::Free()
{
	// Make sure all member data that might have been allocated is freed.
	if(m_pBMI)
	{
		delete m_pBMI;
		m_pBMI = NULL;
	}
	
	if(m_pBits)
	{
		delete m_pBits;
		m_pBits = NULL;
	}
	if(m_pPalette)
	{
		m_pPalette->DeleteObject();
		delete m_pPalette;
		m_pPalette = NULL;
	}
}

BOOL  CMaskedBitmap::CreatePalette()
{
	if ( !IsValid() )
		return FALSE;
	
	//get the number of colors in the DIB
	WORD wNumColors = NumColors( m_pBMI->bmiHeader );
	BOOL bResult = TRUE;
	
	if (wNumColors != 0)
	{
		// allocate memory block for logical palette
		LPLOGPALETTE pLogPal = (LPLOGPALETTE) new BYTE[ sizeof(LOGPALETTE) +
			sizeof(PALETTEENTRY)*wNumColors ];
		
		// if not enough memory, clean up and return NULL
		if( pLogPal == 0 )
			return FALSE;
		
		// set version and number of palette entries
		pLogPal->palVersion = PALVERSION;
		pLogPal->palNumEntries = wNumColors;
		
		for (WORD i = 0; i < wNumColors; i++)
		{
			pLogPal->palPalEntry[i].peRed = m_pBMI->bmiColors[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = m_pBMI->bmiColors[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue = m_pBMI->bmiColors[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}
		
		// create the palette and get handle to it 
		if (m_pPalette)
		{
			m_pPalette->DeleteObject();
			delete m_pPalette;
		}
		
		m_pPalette = new CPalette;
		bResult = m_pPalette->CreatePalette( pLogPal );
		delete pLogPal;
	}
	
	return bResult;
}

int CMaskedBitmap::Width() const
{
	if (!m_pBMI)
		return 0;
	
	/* return the DIB width */
	return m_pBMI->bmiHeader.biWidth;
}

int CMaskedBitmap::Height() const
{
	if (!m_pBMI)
		return 0;
	
	/* return the DIB height */
	return m_pBMI->bmiHeader.biHeight;
}

DWORD CMaskedBitmap::Read(CFile& file, BOOL bFromResource)
{
	DWORD dwReadBytes = 0;
	DWORD dwLength = (DWORD) file.GetLength();
	
	// Ensures no memory leaks will occur
	Free();
	
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	
	if( !bFromResource )
	{
		// Go read the DIB file header and check if it's valid.
		if( (dwReadBytes = file.Read((LPSTR)&bmfHeader, BMIF_SIZE)) != BMIF_SIZE)
			return 0;
		if(bmfHeader.bfType != DIB_HEADER_MARKER)
			return 0;
	}
	
	// Read DIB header.
	if( file.Read( &bmiHeader, BMIH_SIZE ) != BMIH_SIZE )
		return 0;
	dwReadBytes += BMIH_SIZE;
	
	DWORD dwPalSize = NumColors( bmiHeader ) * sizeof RGBQUAD;
	m_pBMI = (LPBITMAPINFO) new BYTE[BMIH_SIZE + dwPalSize];
	memcpy( m_pBMI, &bmiHeader, BMIH_SIZE );
	// read palette data
	if( file.Read( m_pBMI->bmiColors, dwPalSize ) != dwPalSize )
		return 0;
	dwReadBytes += dwPalSize;
	CreatePalette();
	
	// Go read the bits.
	m_pBits = new BYTE[ dwLength - dwReadBytes ];
	if (m_pBits == 0)
		return 0;
	
	if (file.Read( m_pBits, dwLength - dwReadBytes ) != (dwLength - dwReadBytes))
	{
		delete m_pBMI;
		m_pBMI = NULL;
		
		delete m_pBits;
		m_pBits = NULL;
		
		return 0;
	}
	dwReadBytes = dwLength;
	
	return dwReadBytes;
}

DWORD CMaskedBitmap::ReadFromResource(UINT nResID)
{
	DWORD	dwResult = 0;
	HMODULE hModule = AfxFindResourceHandle(MAKEINTRESOURCE(nResID), RT_BITMAP);
	// Load	from resource
	HRSRC	hbmres = FindResource(hModule, MAKEINTRESOURCE(nResID), RT_BITMAP );
	CMemFile file; 
	HGLOBAL	hGlob;
	
	if (hbmres)
	{
		DWORD	dwResSize = SizeofResource( hModule, hbmres );
		file.Attach( (LPBYTE)LockResource( hGlob = LoadResource(hModule, hbmres) ), dwResSize );
		dwResult = Read(file, TRUE);
		file.Detach();
		DeleteObject( hGlob );
	}
	
	return dwResult;
}


void CMaskedBitmap::Draw(CDC *pDC, const CRect &rect)
{
	if ( !IsValid() )
		return;

	CPalette* pOldPal = NULL;        // Previous palette
	
	// Get the DIB's palette, then select it into DC
	if (m_pPalette != NULL)
	{
		// Select as background since we have
		// already realized in forground if needed
		pOldPal = pDC->SelectPalette( m_pPalette, TRUE);
	}
	
	/* Make sure to use the stretching mode best for color pictures */
	pDC->SetStretchBltMode( COLORONCOLOR );
	
	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	::StretchDIBits(pDC->m_hDC,						// hDC
		rect.left,					// DestX
		rect.top,					// DestY
		rect.Width(),				// nDestWidth
		rect.Height(),				// nDestHeight
		0,							// SrcX
		0,							// SrcY
		Width(),					// wSrcWidth
		Height(),					// wSrcHeight
		m_pBits,                      // lpBits
		m_pBMI,                       // lpBitsInfo
		DIB_RGB_COLORS,               // wUsage
		SRCCOPY);                     // dwROP
	
	/* Reselect old palette */
	if (pOldPal != NULL)
	{
		pDC->SelectPalette( pOldPal, TRUE);
	}
}

void CMaskedBitmap::DrawTransparent(CDC *pDC, const CRect &rect,
    COLORREF clrTransparency, bool bGray, COLORREF grayColor)
{
	if ( !IsValid() )
		return;
    //
    // Create a memory DC (dcImage) and select the bitmap into it.
    //
    CDC dcImage;
    dcImage.CreateCompatibleDC (pDC);
	CBitmap bmImage;
	bmImage.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

    CBitmap* pOldBitmapImage = dcImage.SelectObject(&bmImage);


	CPalette* pOldPal = NULL;        // Previous palette
	// Get the DIB's palette, then select it into DC
	if (m_pPalette != NULL)
	{
		// Select as background since we have
		// already realized in forground if needed
		pOldPal = dcImage.SelectPalette( m_pPalette, TRUE);
	}
	
	/* Make sure to use the stretching mode best for color pictures */
	dcImage.SetStretchBltMode( COLORONCOLOR );
	
	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	::StretchDIBits(dcImage.m_hDC,						// hDC
		0,							// DestX
		0,							// DestY
		rect.Width(),				// nDestWidth
		rect.Height(),				// nDestHeight
		0,							// SrcX
		0,							// SrcY
		Width(),					// wSrcWidth
		Height(),					// wSrcHeight
		m_pBits,                      // lpBits
		m_pBMI,                       // lpBitsInfo
		DIB_RGB_COLORS,               // wUsage
		SRCCOPY);                     // dwROP
	
	/* Reselect old palette */
		
	if (pOldPal != NULL)
	{
		dcImage.SelectPalette( pOldPal, TRUE);
	}
		
    //
    // Create a second memory DC (dcAnd) and in it create an AND mask.
    //
    CDC dcAnd;
    dcAnd.CreateCompatibleDC (pDC);

    CBitmap bitmapAnd;
    bitmapAnd.CreateBitmap (rect.Width(), rect.Height(), 1, 1, NULL);
    CBitmap* pOldBitmapAnd = dcAnd.SelectObject (&bitmapAnd);

    dcImage.SetBkColor (clrTransparency);
    dcAnd.BitBlt (0, 0, rect.Width(), rect.Height(), &dcImage, 0, 0,
        SRCCOPY);

    //
    // Create a third memory DC (dcXor) and in it create an XOR mask.
    //
    CDC dcXor;
    dcXor.CreateCompatibleDC (pDC);

    CBitmap bitmapXor;
    bitmapXor.CreateCompatibleBitmap (&dcImage, rect.Width(), rect.Height());
    CBitmap* pOldBitmapXor = dcXor.SelectObject (&bitmapXor);

    dcXor.BitBlt (0, 0, rect.Width(), rect.Height(), &dcImage, 0, 0,
        SRCCOPY);

    dcXor.BitBlt (0, 0, rect.Width(), rect.Height(), &dcAnd, 0, 0,
        0x220326);

    //
    // Copy the pixels in the destination rectangle to a temporary
    // memory DC (dcTemp).
    //
    CDC dcTemp;
    dcTemp.CreateCompatibleDC (pDC);

    CBitmap bitmapTemp;
    bitmapTemp.CreateCompatibleBitmap (&dcImage, rect.Width(), rect.Height());
    CBitmap* pOldBitmapTemp = dcTemp.SelectObject (&bitmapTemp);

    dcTemp.BitBlt (0, 0, rect.Width(), rect.Height(), pDC, rect.left, rect.top, SRCCOPY);

    //
    // Generate the final image by applying the AND and XOR masks to
    // the image in the temporary memory DC.
    //
	if (bGray) {
		dcTemp.SetTextColor(grayColor);
		dcTemp.BitBlt (0, 0, rect.Width(), rect.Height(), &dcAnd, 0, 0,
			SRCAND);	
	}
	else {
		dcTemp.BitBlt (0, 0, rect.Width(), rect.Height(), &dcAnd, 0, 0,
			SRCAND);

		dcTemp.BitBlt (0, 0, rect.Width(), rect.Height(), &dcXor, 0, 0,
			SRCINVERT);
	}

    //
    // Blit the resulting image to the screen.
    //
    pDC->BitBlt (rect.left, rect.top, rect.Width(), rect.Height(), &dcTemp, 0, 0, SRCCOPY);

    //
    // Restore the default bitmaps.
    //
    dcTemp.SelectObject (pOldBitmapTemp);
    dcXor.SelectObject (pOldBitmapXor);
    dcAnd.SelectObject (pOldBitmapAnd);
    dcImage.SelectObject (pOldBitmapImage);
}

void CMaskedBitmap::Draw (CDC* pDC, int x, int y)
{
	CRect rect(x, y, x + Width(), y + Height());
	Draw(pDC, rect);
}

void CMaskedBitmap::DrawTransparent (CDC* pDC, int x, int y, 
        COLORREF clrTransparency, bool bGray, COLORREF grayColor)
{
	CRect rect(x, y, x + Width(), y + Height());
	DrawTransparent (pDC, rect, clrTransparency, bGray, grayColor);
}


WORD  CMaskedBitmap::NumColors(  BITMAPINFOHEADER& bmiHeader ) const
{
	if ( bmiHeader.biClrUsed != 0)
		return (WORD)bmiHeader.biClrUsed;
	
	switch ( bmiHeader.biBitCount )
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
	}
}
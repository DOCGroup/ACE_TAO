// DecoratorUtil.h: interface for the CDecoratorUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECORATORUTIL_H__0CE3DB65_F3AE_42BA_98C3_1871F51764BB__INCLUDED_)
#define AFX_DECORATORUTIL_H__0CE3DB65_F3AE_42BA_98C3_1871F51764BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "DecoratorLib.h"
#include "DecoratorStd.h"

class CDecoratorUtil;

typedef CTypedPtrList<CPtrList, CFont *>								CFontList;
typedef CTypedPtrMap<CMapPtrToPtr, void *, CPen *>						CPenTable;
typedef CTypedPtrMap<CMapPtrToPtr, void *, CBrush *>					CBrushTable;


extern CDecoratorUtil	d_util;

class CDecoratorUtil  
{
public:
	CDecoratorUtil();
	virtual ~CDecoratorUtil();

public:
	CPen *GetPen(COLORREF color,bool dash = false);
	CBrush *GetBrush(COLORREF color);
	CFont *GetFont(GMEFontKind kind);
	CFont *GetFont(int kindsize, bool bold = false, bool semibold = false);
	int  GetFontSize(GMEFontKind kind);
	void DeletePens(CPenTable &penTable);
	void DeleteBrushes(CBrushTable &brushTable);
	void CreateFonts(CFont **font,int boldness);
	void Draw3DBox(CDC *pDC,CRect rect,int borderSize,COLORREF brColor,COLORREF modelColor,bool special = false);
	void Draw3DBox(CDC *pDC,CRect rect,COLORREF brColor,COLORREF color,bool special = false);
	void DrawFlatBox(CDC *pDC,CRect rect,COLORREF brColor,COLORREF color);
	COLORREF GetDarkBorderColor(COLORREF color);
	COLORREF GetLightBorderColor(COLORREF color);
	void DrawText(CDC *pDC,CString &txt,CPoint &pt,CFont *font,COLORREF color,int align);

private:
	CPenTable pens;
	CBrushTable brushes;
	CFont *normalFonts[GME_FONT_KIND_NUM];
	CFont *semiboldFonts[GME_FONT_KIND_NUM];
	CFont *boldFonts[GME_FONT_KIND_NUM];
	CFontList allFonts;				// for destruction only
};

#endif // !defined(AFX_DECORATORUTIL_H__0CE3DB65_F3AE_42BA_98C3_1871F51764BB__INCLUDED_)

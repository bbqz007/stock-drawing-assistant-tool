/**
MIT License
Copyright (c) 2015-2020 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Title: Tool for Drawing Assistant Line and Assistant Graph
//  Auth: dennis. Z.
//  Date: June, July 2012
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRAWTOOL_BASIC__H_
#define _DRAWTOOL_BASIC__H_

#include <windows.h>
#include <deque>
#include <list>
#include <utility>
#include <map>
#include <math.h>
#include "DrawToolDef.h"

class DTAssistGraphics;
struct DTAssistGraphDescription;
class DTAssistGraph;
class DTCoordinateTransform;
class DTAbsoluteCoordinate;
class DTDiagram;
class DTEquation;
struct DTIdentity;
struct DTAbsLocations;
struct DTLineProperties;
struct DTAssistGraphDescription;
struct DTAssistGraphPersistDescription;
struct DTAssistGraphPersistText;
struct DTAssistGraphDescriptionScript;

#define ISDTOK(x) (x == 0)

long DTHelperConvertTm(DWORD _Date, DWORD _Hour = 0, DWORD _Min = 0, DWORD _Sec = 0);
long DTHelperLocalTimeToGmTime(long _Localtime);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 算术结构
#pragma pack(1)
struct tagDTRange
{
	float lowBound;
	float highBound;
};

typedef POINT tagDTPoint;

inline bool operator == (const tagDTPoint& _L, const tagDTPoint& _R)
{
	return memcmp(&_L, &_R, sizeof(tagDTPoint)) == 0;
}


struct tagDTLine
{
	tagDTPoint ptBegin;
	tagDTPoint ptEnd;
};

typedef SIZE tagDTSize;
typedef RECT tagDTRect;

inline bool operator == (const tagDTRect& _L, const tagDTRect& _R)
{
	return memcmp(&_L, &_R, sizeof(tagDTRect)) == 0;
}

struct tagDTArcLine
{
	tagDTRect rectBound;
	tagDTLine arcLine;
};

inline bool operator == (const tagDTArcLine& _L, const tagDTArcLine& _R)
{
	return memcmp(&_L, &_R, sizeof(tagDTArcLine)) == 0;
}

struct tagDTAbsPoint
{
	long xDate;
	float yValue;
};

inline bool operator == (const tagDTAbsPoint& _L, const tagDTAbsPoint& _R)
{
	return memcmp(&_L, &_R, sizeof(tagDTAbsPoint)) == 0;
}

struct tagKValue
{
	int	m_date;				// 由于要用于其它周期，所以最后为32bit的time_t
	float	m_fOpen;
	float	m_fHigh;
	float	m_fLow;
	float	m_fClose;

	tagKValue()
		: m_date(0), m_fOpen(0.0), m_fHigh(0.0), m_fLow(0.0), m_fClose(0.0)
	{}
};

struct tagViewXAxisUnit
{
	long xLeft;
	long xRight;
	long xTarget;

	tagViewXAxisUnit()
		: xLeft(0), xRight(0), xTarget(0)
	{}
	tagViewXAxisUnit(long l, long t, long r)
		: xLeft(l), xRight(r), xTarget(t)
	{}
};
#pragma pack()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct DTIdentity
{
	char stkCode[8];
	DWORD nPeriod;
	DWORD nViewId;
	DWORD nType;
	long tTime;

	DTIdentity() { reset(); }
	void reset() { memset(this, 0, sizeof(DTIdentity)); }
	bool operator == (const DTIdentity& u) const { return memcmp(this, &u, sizeof(DTIdentity)) == 0; }
	bool operator != (const DTIdentity& u) const { return memcmp(this, &u, sizeof(DTIdentity)) != 0; }
	bool operator < (const DTIdentity& u) const
	{
		if(memcmp(stkCode, u.stkCode, sizeof(stkCode)) < 0) return true;
		else if(memcmp(stkCode, u.stkCode, sizeof(stkCode)) > 0) return false;
		else if(nPeriod < u.nPeriod) return true;
		else if(nPeriod > u.nPeriod) return false;
		else if(nViewId < u.nViewId) return true;
		else if(nViewId > u.nViewId) return false;
		else if(nType < u.nType) return true;
		else if(nType > u.nType) return false;
		else if(tTime < u.tTime) return true;
		else return false;
	}
	bool operator > (const DTIdentity& u) const
	{
		if(memcmp(stkCode, u.stkCode, sizeof(stkCode)) > 0) return true;
		else if(memcmp(stkCode, u.stkCode, sizeof(stkCode)) < 0) return false;
		else if(nPeriod > u.nPeriod) return true;
		else if(nPeriod < u.nPeriod) return false;
		else if(nViewId > u.nViewId) return true;
		else if(nViewId < u.nViewId) return false;
		else if(nType > u.nType) return true;
		else if(nType < u.nType) return false;
		else if(tTime > u.tTime) return true;
		else return false;
	}
	bool operator <= (const DTIdentity& u) const { return !operator > (u); }
	bool operator >= (const DTIdentity& u) const { return !operator < (u); }
	void upgrade(char (&_C)[8], DWORD _P, DWORD _V, DWORD _T)
	{
		memcpy(stkCode, _C, sizeof(stkCode));
		nPeriod = _P;
		nViewId = _V;
		nType = _T;
		tTime = ::_time32(NULL);
	}
};

struct DTAbsLocations
{
	tagDTAbsPoint ptAbsoluteP1;
	tagDTAbsPoint ptAbsoluteP2;
	tagDTAbsPoint ptAbsoluteP3;
};

struct DTLineProperties
{
	COLORREF rgbColor;
	int nWidth;
	int flagStyle;
	int nMode;
};

struct DTAssistGraphDescription
{
	bool operator < (const DTAssistGraphDescription& _R) const;
	bool operator == (const DTAssistGraphDescription& _R) const;

	DTIdentity m_Identity;
	DTAbsLocations m_ptLocations;
	DTLineProperties m_propLine;
};

struct DTAssistGraphPersistDescription
{
	DTAssistGraphDescription m_Description;
	DWORD m_CheckSum;

	DTAssistGraphPersistDescription();
	BOOL TestCheckSum();
	DWORD CheckSummat();
};

struct DTAssistGraphPersistText
{
	enum
	{
		ENUM_CHECKSUM_SIZE = sizeof(DWORD),
		ENUM_TEXT_SIZE = 200,
		ENUM_STRUCT_SIZE = 256,
		ENUM_RAW_SIZE = ENUM_STRUCT_SIZE - ENUM_CHECKSUM_SIZE
	};
	union
	{
		struct
		{
			DTIdentity m_Identity;
			char m_Text[ ENUM_TEXT_SIZE ];
		};
		char m_Raw[ ENUM_RAW_SIZE ];
	};
	DWORD m_CheckSum;

	DTAssistGraphPersistText();
	BOOL TestCheckSum();
	DWORD CheckSummat();
	void Revise();
	void CopyText(DTAssistGraphPersistText& _Other);
	void CopyText(char* _Text);
};

struct DTAssistGraphDescriptionScript
{
	DWORD m_Location1 : 1;
	DWORD m_Location2 : 1;
	DWORD m_Location3 : 1;
	DWORD m_Text : 1;
	DWORD m_Custom : 1;

	DTAssistGraphDescriptionScript();
};

#pragma pack()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DTAssistGraphDescriptionScriptMap
{
	static DTAssistGraphDescriptionScriptMap& Instance();
	DTAssistGraphDescriptionScriptMap();
	~DTAssistGraphDescriptionScriptMap();

	std::map<DWORD, char*> m_szTypeScriptMap;
	std::map<DWORD, char*> m_szViewIdScriptMap;
	std::map<DWORD, char*> m_szPeriodScriptMap;

	std::map<DWORD, DWORD> m_timePeriodScriptMap;

	void InsertDayPeriod(DWORD _Type);
	void InsertTypeName(DWORD _Type, char* _Name);
	void InsertViewName(DWORD _ViewId, char* _Name);
	void InsertPeriodName(DWORD _Period, char* _Name);

	void TypeName(DWORD _Type, char* _Name, size_t _Size);
	void ViewName(DWORD _ViewId, char* _Name, size_t _Size);
	void PeriodName(DWORD _Period, char* _Name, size_t _Size);

	BOOL IsPeriodByTheDay(DWORD _Period);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DTAbsoluteCoordinate
{
public:
	typedef std::pair<tagKValue, tagViewXAxisUnit> _Ty_Point;

	DTAbsoluteCoordinate();

	long AbsXtoViewX(long _xDate);
	long ViewXtoAbsX(long _xView);
	long ViewXtoAbsX2(long _xView);
	long IndexOfDate(long _xDate);
	long IndexOfViewX(long _xView);
	long IndexOfViewX2(long _xView);
	long DateByIndex(long _xIndex);
	long ViewXByIndex(long _xIndex);
	long AdjustViewX(long _xView);
	tagKValue KValueByIndex(long _xIndex);
	tagViewXAxisUnit ViewXUnitByIndex(long _xIndex);

	long IndexOfBound() { return m_idxBound; }
	tagKValue& KValueOfBound() { return m_Bound.first; }
	tagViewXAxisUnit& PointOfBound() { return m_Bound.second; }
	long IndexOfOrigin() { return m_idxOrigin; }
	tagKValue& KValueOfOrigin() { return m_Origin.first; }
	tagViewXAxisUnit& PointOfOrigin() { return m_Origin.second; }
	long IndexOfStkBegin() { return m_idxStkBegin; }
	tagKValue& KValueOfStkBegin() { return m_StkBegin.first; }
	tagViewXAxisUnit& PointOfStkBegin() { return m_Origin.second; }
	long IndexOfStkEnd() { return m_idxStkEnd; }
	tagKValue& KValueOfStkEnd() { return m_StkEnd.first; }
	tagViewXAxisUnit& PointOfStkEnd() { return m_Origin.second; }
	long Size() { return m_qeuPoints.size(); }

	long UnitWidth() { return m_UnitWidth; }
	long BeginDate() { return m_StkBegin.first.m_date; }
	long EndDate() { return m_StkEnd.first.m_date; }
	long OriginDate() { return m_Origin.first.m_date; }
	long BoundDate() { return m_Bound.first.m_date; }

	int Build(tagKValue* _Vals, tagViewXAxisUnit* _Units, size_t _Size, size_t _IndexForm, size_t _IndexTo);
private:
	std::deque<_Ty_Point> m_qeuPoints;
	long m_UnitWidth;			// we should not use this;
	_Ty_Point m_Origin;			// be relative to the View
	long m_idxOrigin;			// be relative to the View
	_Ty_Point m_Bound;			// be relative to the View
	long m_idxBound;			// be relative to the View
	_Ty_Point m_StkBegin;		// be relative to the View
	long m_idxStkBegin;			// be relative to the View
	_Ty_Point m_StkEnd;			// be relative to the View
	long m_idxStkEnd;			// be relative to the View
};

class DTCoordinateTransform
{
public:
	friend class DTAssistGraphisc;
	typedef DTAbsoluteCoordinate::_Ty_Point _Ty_AbsPoint;

	DTCoordinateTransform(DTAssistGraphics* _Graphics);
	DTCoordinateTransform& operator= (DTCoordinateTransform& _Other);
	void SetViewRect(const tagDTRect& _ViewRect, const tagDTRange& _ValueRange);

	long AbsYtoViewY(float _yValue);
	float ViewYtoAbsY(long _yView);

	long AbsXtoViewX(long _xDate);
	long ViewXtoAbsX(long _xView);
	long ViewXtoAbsX2(long _xView);
	long IndexOfDate(long _date);
	long IndexOfViewX(long _xView);
	long IndexOfViewX2(long _xView);
	long DateByIndex(long _xIndex);
	long ViewXByIndex(long _xIndex);
	long AdjustViewX(long _xView);
	tagKValue KValueByIndex(long _xIndex);
	tagViewXAxisUnit ViewXUnitByIndex(long _xIndex);

	//
	long IndexOfBoundOnAbsCoord();
	long IndexOfOriginOnAbsCoord();
	long IndexOfBound();
	tagKValue& KValueOfBound();
	tagViewXAxisUnit& PointOfBound();
	long IndexOfOrigin();
	tagKValue& KValueOfOrigin();
	tagViewXAxisUnit& PointOfOrigin();
	long IndexOfStkBegin();
	tagKValue& KValueOfStkBegin();
	tagViewXAxisUnit& PointOfStkBegin();
	long IndexOfStkEnd();
	tagKValue& KValueOfStkEnd();
	tagViewXAxisUnit& PointOfStkEnd();
	long SizeOfAbsPoints();


	//
	const tagDTAbsPoint& AbsOrigin() const { return m_ptViewAbsOrigin; }
	void ViewOrigin(const tagDTAbsPoint& _Point) { m_ptViewAbsOrigin = _Point; }
	const tagDTPoint& ViewOrigin() const { return m_ptViewOrigin; }
	void ViewOrigin(const tagDTPoint& _Point) { m_ptViewOrigin = _Point; }

	const tagDTRect& VirtualRect() const { return m_rcVirtualRect; }
	void VirtualRect(const tagDTRect& _Rect) { m_rcVirtualRect = _Rect; }
	const tagDTRect& ViewRect() const { return m_rcViewRect; }
	void ViewRect(const tagDTRect& _Rect) { m_rcViewRect = _Rect; }
	const tagDTRect& ValidateRect() const { return m_rcValidateRect; }
	void ValidateRect(const tagDTRect& _Rect) { m_rcValidateRect = _Rect; }

	float UnitHeight() { return m_eUnitHeight; }

	int Build(const tagDTRect& _ViewRect, float _yValTop, float _yValBottom);
	int LoadFromAssistGraphics(DWORD _ViewId);
private:
	DTAssistGraphics* m_Graphics;

	tagDTRect m_rcVirtualRect;
	tagDTRect m_rcViewRect;
	tagDTRect m_rcValidateRect;

	float m_eUnitHeight;
	long m_eUnitWidth;

	tagDTAbsPoint m_ptViewAbsOrigin;
	tagDTPoint m_ptViewOrigin;
};

class DTAssistGraphBook
{
public:
	friend DTAssistGraphics;
	typedef DTIdentity _Ty_Id;
	typedef DTAssistGraphDescription _Ty_Desc;
	typedef char* _Ty_Text;
	typedef std::map<_Ty_Id, _Ty_Desc> _Ty_Content;
	typedef std::map<_Ty_Id, _Ty_Text> _Ty_Dictionary;
	typedef CRITICAL_SECTION _Ty_Lock;
	static DTAssistGraphBook& Instance();
	DTAssistGraphBook();
	~DTAssistGraphBook();

	LRESULT InsertGraph(_Ty_Id& _Id, _Ty_Desc& _Desc, _Ty_Text _Text = NULL);
	LRESULT RemoveGraph();
	LRESULT RemoveGraph(_Ty_Id& _Id);
	LRESULT RemoveGraphByStkCode(char (&_StkCode)[8]);

protected:
	struct DTAssistGraphBookLocker
	{
		_Ty_Lock* _Lock_;
		DTAssistGraphBookLocker(_Ty_Lock* _l)
			: _Lock_(_l)
		{
			::EnterCriticalSection(_Lock_);
		}
		~DTAssistGraphBookLocker()
		{
			::LeaveCriticalSection(_Lock_);
		}
	};

	void LoadFromTest();

	_Ty_Text AllocateFixedText(_Ty_Text _Text = NULL);
	void DeallocateFixedText(_Ty_Text _Text);
	void ReleaseDictionary();
	void ReleaseDictionary_i();
	void ReleaseDictionaryItem_i(_Ty_Id& _Id);
	void ReleaseContents();
	void ReleaseContents_i();
	void ReleaseContentsItem_i(_Ty_Id& _Id);

	LRESULT ReadFromPersist_i();
	LRESULT ReadPersistDescription_i();
	LRESULT ReadPersistText_i();
	LRESULT ReadFromPersist();
	LRESULT ReadPersistDescription();
	LRESULT ReadPersistText();
	LRESULT WriteToPersist_i();
	LRESULT WritePersistDescription_i();
	LRESULT WritePersistText_i();
	LRESULT WriteToPersist();
	LRESULT WritePersistDescription();
	LRESULT WritePersistText();
private:
	const size_t m_TextFixedLength;
	const size_t m_MaxPersistLength;
	char m_Contents_FilePath[ MAX_PATH ];
	char m_Dictionary_FilePath[ MAX_PATH ];

	_Ty_Lock m_Lock;
	_Ty_Content m_Contents;
	_Ty_Dictionary m_Dictionary;
};

class DTAssistGraph
{
public:
	DTAssistGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTAssistGraph(DTAssistGraph& _Other);
	void SetViewRect(const tagDTRect& _ViewRect, const tagDTRange& _ValueRange);
	void Commit(DTAssistGraphBook& _Book);
	DTAssistGraphDescription& Description() { return m_descGraph; }
	const tagDTRect& ViewRect() { return m_coorTransform.ViewRect(); }

	virtual ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_NULL; }
	virtual int HitTest(const tagDTPoint& _Point, long _xDate = 0) = 0;
	virtual int Render(HDC _hdc) = 0;
	virtual int RenderShadow(HDC _hdc) = 0;
	virtual int RenderEditing(HDC _hdc) = 0;
	virtual int RenderAnchors(HDC _hdc) = 0;
	virtual int RenderShadowAnchors(HDC _hdc) = 0;
	virtual int RenderEditingAnchors(HDC _hdc) = 0;
	virtual int RenderSelectedAnchors(HDC _hdc) = 0;
	virtual int NextEditMode(int _CurMode) = 0;
	virtual void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	virtual void MoveP1To(const tagDTPoint& _Point);
	virtual void MoveP2To(const tagDTPoint& _Point);
	virtual void MoveP3To(const tagDTPoint& _Point);
	virtual int EndMove(long _delta_X, long _delta_Y);
	virtual int EndMove();
	virtual int BuildAccordToViewPoint(const tagDTPoint* _Point1, const tagDTPoint* _Point2 = NULL, const tagDTPoint* _Point3 = NULL);
	virtual int BuildAccordToDescription() = 0;
	virtual int Build() = 0;
	virtual int Complete() = 0;
	virtual int Clone(DTAssistGraph& _Other);
	virtual int EndClone(DTAssistGraph& _Other);			//// <!!!! NOTE !!!!> some derived class uses the pointer to the member of the base class, it will be foolish after clone. to implement this to resolve the stupid problem. copy construct also cause this chicken problem.
	virtual DTAssistGraph* CreateShadow();
	virtual bool IsAvaiable() { return m_bAvaiable; }
	virtual char* Text() { return 0; }
	virtual int CopyText(DTAssistGraphPersistText& _Text) { return 0; }
	virtual int CopyText(char* _Text) { return 0; }

	virtual DTAssistGraphDescriptionScript DescriptionScript() = 0;

	static DTAssistGraphDescriptionScript _DescriptionScriptOfSinglePoint();
	static DTAssistGraphDescriptionScript _DescriptionScriptOfSimpleLine();
	static DTAssistGraphDescriptionScript _DescriptionScriptOfParallelLine();
	static DTAssistGraphDescriptionScript _DescriptionScriptOfText();

	static int _NextEditModeOfSinglePoint(int _CurMode);
	static int _NextEditModeOfSimpleLine(int _CurMode);
	static int _NextEditModeOfParallelLine(int _CurMode);
	static int _NextEditModeOfText(int _CurMode);
protected:
	virtual ~DTAssistGraph();
	int NextEditModeOfSinglePoint(int _CurMode);
	int NextEditModeOfSimpleLine(int _CurMode);
	int NextEditModeOfParallelLine(int _CurMode);
	int HitAnchor(const tagDTPoint& _Anchor, const tagDTPoint& _Point, int _RetSucceed);
protected:
	const static long m_nAnchorThickness;
protected:
	DTAssistGraphics* m_Graphics;

	DTCoordinateTransform m_coorTransform;
	DTAssistGraphDescription m_descGraph;

	tagDTRect m_rcVirtualRect;
	tagDTRect m_rcValidateRect;
	DWORD m_nValidStatus;
	DTLineProperties m_propLine;
	bool m_bUseDefault;
	bool m_bAvaiable;

	tagDTPoint m_ptViewLocationP1;
	tagDTPoint m_ptViewLocationP2;
	tagDTPoint m_ptViewLocationP3;
};

#define COLORREF_MASK_OF_R	0xFF
#define COLORREF_MASK_OF_G	0xFF00
#define COLORREF_MASK_OF_B	0xFF0000
#define COLORREF_MASK_OF_RG	(COLORREF_MASK_OF_R | COLORREF_MASK_OF_G)
#define COLORREF_MASK_OF_RGB	(COLORREF_MASK_OF_RG | COLORREF_MASK_OF_B)

class DTAssistGraphics
{
public:
	DTAssistGraphics();
	~DTAssistGraphics();
	DTAbsoluteCoordinate& Coordinate() { return m_AbsCoord; }

	void BackGroupColor(COLORREF _color) { m_BgColor = _color; }
	COLORREF BackGroupColor() { return m_BgColor; }
	void ForeGroupColor(COLORREF _color) { m_FgColor = _color; }
	COLORREF ForeGroupColor() { return m_FgColor; }
	void OverlapColor(COLORREF _color) { m_OverlapColor = _color; }
	COLORREF OverlapColor() { return m_OverlapColor; }

	//COLORREF ShadowBackGroupColor() { return (~m_BgColor) | COLORREF_MASK_OF_RGB; }
	//COLORREF ShadowForeGroupColor() { return (m_OverlapColor & COLORREF_MASK_OF_RG) | ((m_OverlapColor & COLORREF_MASK_OF_B) ^ COLORREF_MASK_OF_B); }
	//COLORREF ShadowColorStepXor(COLORREF _color) { return _color & COLORREF_MASK_OF_B; }
	//COLORREF ShadowColorStepAnd(COLORREF _color) { return _color & COLORREF_MASK_OF_RG | COLORREF_MASK_OF_B; }

	COLORREF ShadowBackGroupColor() { return (~m_BgColor) & COLORREF_MASK_OF_RGB; }
	COLORREF ShadowForeGroupColor() { return (m_FgColor ^ m_BgColor) & COLORREF_MASK_OF_RGB; }
	COLORREF ShadowColor() { return (m_OverlapColor ^ m_FgColor ^ m_BgColor) & COLORREF_MASK_OF_RGB; }
	COLORREF ShadowForeGroupColor(COLORREF _color) { return (_color ^ m_BgColor) & COLORREF_MASK_OF_RGB; }				// for editing
	COLORREF ShadowColor(COLORREF _color) { return (m_OverlapColor ^ _color ^ m_BgColor) & COLORREF_MASK_OF_RGB; }

	DTAssistGraph* CreateAssistGraphInstance(ENUM_ASSIST_GRAPH_TYPE _type, const DTAssistGraphDescription& _descGraph);
	DTAssistGraph* CreateAssistGraphInstance(DTAssistGraph* _other);
	void ReleaseAssistGraphInstance(DTAssistGraph* _p);

	void CommitAssistGraph();
	void DiscardAssistGraph();
	void CommitAssistGraph(DTAssistGraphBook& _Book);
	void DiscardAssistGraph(DTAssistGraphBook& _Book);
	void CommitAssistGraph(DTAssistGraphBook& _Book, DTAssistGraphDescription& _Description, char* _Text = NULL);
	void DiscardAssistGraph(DTAssistGraphBook& _Book, DTAssistGraphDescription& _Description);
	BOOL DiscardActiveGraph();
	LRESULT DiscardAllGraph();

	void SetFullViewRect(const tagDTRect& _FullViewRect) { m_FullViewRect = _FullViewRect; }
	int BuildAbsoluteCoordinate(tagKValue* _Vals, tagViewXAxisUnit* _Units, size_t _Size, size_t _IndexForm, size_t _IndexTo);
	int AddViewCoordinate(DWORD _ViewId, const tagDTRect& _ViewRect, float _yValTop, float _yValBottom);
	int LoadFormAssistGraphBook(char (&stkCode)[8], DWORD nPeriod);
	int LoadFormAssistGraphBook(DTAssistGraphBook& _Book, char (&stkCode)[8], DWORD nPeriod);
	int LoadCoordinateTransform(DTCoordinateTransform& _Transform, DWORD _ViewId);
	int FlushAssistGraph();
	int ForgetFullViewBitmap();

	void ClearEditMode();
	void PushEditMode(ENUM_ASSIST_EDIT_MODE _Mode) { m_PrevMode = m_CurMode; m_CurMode = _Mode; }
	void ResetEditMode() { PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL); }
	ENUM_ASSIST_EDIT_MODE EditMode() { return m_CurMode; }
	ENUM_ASSIST_EDIT_MODE PrevEditMode() { return m_PrevMode; }
	ENUM_ASSIST_EDIT_MODE NextEditMode(ENUM_ASSIST_GRAPH_TYPE _Type, ENUM_ASSIST_EDIT_MODE _Mode);
	bool IsSpotViewCoordValidate();

	void EnterPreEditing(ENUM_ASSIST_GRAPH_TYPE _Type);
	void CancelSelected();

	void SetStkCode(DWORD _LMarketType, char* _LStkCode);

	long HitTestViewId(tagDTPoint& _Point);
	LRESULT LastHitTest() { return m_LastHitTest; }
	void SelectCursorTarget(HCURSOR _Cursor) { m_hcrsrTarget = _Cursor; }
	void SelectCursorMovement(HCURSOR _Cursor) { m_hcrsrMove = _Cursor; }
	void SelectCursorAnchors(HCURSOR _Cursor) { m_hcrsrAnchors = _Cursor; }
	void SelectCursorNormal(HCURSOR _Cursor) { m_hcrsrNormal = _Cursor; }
	void SetCursor(ENUM_ASSIST_HITTEST _HitTest);

	// Windows Procedure
	LRESULT OnSelectedPaint(HDC _hdc);
	LRESULT OnPaint(HDC _hdc, BOOL& bHandleForward);
	LRESULT OnDynamicLayerPaint(HDC _hdc, BOOL& bHandleForward);
	LRESULT OnHitTest(DWORD _Period, DWORD _ViewId, tagDTPoint& _Point, BOOL& bHandleForward);
	LRESULT OnLButtonDown(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward);
	LRESULT OnLButtonUp(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward);
	LRESULT OnLButtonDblClk(UINT nFlags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward);
	LRESULT OnMouseMove(UINT nFlags, DWORD _Period, tagDTPoint _Point, BOOL& bHandleForward);
	LRESULT OnRButtonDown(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward);
	LRESULT OnRButtonUp(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward);
	LRESULT OnSelectedEdit();
protected:
	DTAbsoluteCoordinate m_AbsCoord;
	std::map<DWORD, DTCoordinateTransform*> m_ViewCoord;

	std::list<DTAssistGraph*> m_AssistGraph;
	std::list<DTAssistGraph*> m_DiscardAssistGraph;
	DTAssistGraph* m_ActiveGraph;
	DTAssistGraph* m_ShadowGraph;
	DTAssistGraph* m_NewGraph;
	DTAssistGraph* m_CommitGraph;
	tagDTRect m_FullViewRect;
	bool m_bEditing;
	ENUM_ASSIST_EDIT_MODE m_PrevMode;
	ENUM_ASSIST_EDIT_MODE m_CurMode;

	ENUM_ASSIST_GRAPH_TYPE m_ToolSelected;

	DTIdentity m_HitTestId;
	DTIdentity m_ActiveGraphId;
	DTIdentity m_CreateGraphId;
	DTCoordinateTransform m_SpotViewCoord;
	tagDTPoint m_SpotPoint;

	tagDTPoint m_CreationP1;
	tagDTPoint m_CreationP2;
	tagDTRect m_RecoverShadowRect;

	HBITMAP m_FullViewBitmap;
	HFONT m_LabelFont;

	COLORREF m_BgColor;
	COLORREF m_FgColor;
	COLORREF m_OverlapColor;

	LRESULT m_LastHitTest;

	HCURSOR m_hcrsrTarget;
	HCURSOR m_hcrsrMove;
	HCURSOR m_hcrsrAnchors;
	HCURSOR m_hcrsrNormal;
	HCURSOR m_hcrsrCreate;

	char m_StkCode[8];
};

inline long DTCoordinateTransform::AbsXtoViewX(long _xDate)			{ return m_Graphics->Coordinate().AbsXtoViewX(_xDate); }
inline long DTCoordinateTransform::ViewXtoAbsX(long _xView)			{ return m_Graphics->Coordinate().ViewXtoAbsX(_xView); }
inline long DTCoordinateTransform::ViewXtoAbsX2(long _xView)			{ return m_Graphics->Coordinate().ViewXtoAbsX2(_xView); }
inline long DTCoordinateTransform::IndexOfDate(long _date)			{ return m_Graphics->Coordinate().IndexOfDate(_date); }
inline long DTCoordinateTransform::IndexOfViewX(long _xView)			{ return m_Graphics->Coordinate().IndexOfViewX(_xView); }
inline long DTCoordinateTransform::IndexOfViewX2(long _xView)			{ return m_Graphics->Coordinate().IndexOfViewX2(_xView); }
inline long DTCoordinateTransform::DateByIndex(long _xIndex)			{ return m_Graphics->Coordinate().DateByIndex(_xIndex); }
inline long DTCoordinateTransform::ViewXByIndex(long _xIndex)			{ return m_Graphics->Coordinate().ViewXByIndex(_xIndex); }
inline long DTCoordinateTransform::AdjustViewX(long _xView)				{ return m_Graphics->Coordinate().AdjustViewX(_xView); }
inline tagKValue DTCoordinateTransform::KValueByIndex(long _xIndex)	{ return m_Graphics->Coordinate().KValueByIndex(_xIndex); }
inline tagViewXAxisUnit DTCoordinateTransform::ViewXUnitByIndex(long _xIndex)			{ return m_Graphics->Coordinate().ViewXUnitByIndex(_xIndex); }

//
inline long DTCoordinateTransform::IndexOfBoundOnAbsCoord()			{ return m_Graphics->Coordinate().IndexOfBound(); }
inline long DTCoordinateTransform::IndexOfOriginOnAbsCoord()			{ return m_Graphics->Coordinate().IndexOfOrigin(); }
inline long DTCoordinateTransform::IndexOfBound()						{ return m_Graphics->Coordinate().IndexOfBound(); }
inline tagKValue& DTCoordinateTransform::KValueOfBound()				{ return m_Graphics->Coordinate().KValueOfBound(); }
inline tagViewXAxisUnit& DTCoordinateTransform::PointOfBound()		{ return m_Graphics->Coordinate().PointOfBound(); }
inline long DTCoordinateTransform::IndexOfOrigin()						{ return m_Graphics->Coordinate().IndexOfOrigin(); }
inline tagKValue& DTCoordinateTransform::KValueOfOrigin()				{ return m_Graphics->Coordinate().KValueOfOrigin(); }
inline tagViewXAxisUnit& DTCoordinateTransform::PointOfOrigin()		{ return m_Graphics->Coordinate().PointOfOrigin(); }
inline long DTCoordinateTransform::IndexOfStkBegin()					{ return m_Graphics->Coordinate().IndexOfStkBegin(); }
inline tagKValue& DTCoordinateTransform::KValueOfStkBegin()			{ return m_Graphics->Coordinate().KValueOfStkBegin(); }
inline tagViewXAxisUnit& DTCoordinateTransform::PointOfStkBegin()		{ return m_Graphics->Coordinate().PointOfOrigin(); }
inline long DTCoordinateTransform::IndexOfStkEnd()					{ return m_Graphics->Coordinate().IndexOfStkEnd(); }
inline tagKValue& DTCoordinateTransform::KValueOfStkEnd()				{ return m_Graphics->Coordinate().KValueOfStkEnd(); }
inline tagViewXAxisUnit& DTCoordinateTransform::PointOfStkEnd()		{ return m_Graphics->Coordinate().PointOfOrigin(); }
inline long DTCoordinateTransform::SizeOfAbsPoints() { return m_Graphics->Coordinate().Size(); }


class DTDiagram
{
public:
	virtual ~DTDiagram();
private:

};

class DTEquation
{
public:
	virtual ~DTEquation();
	virtual int HitTest(const tagDTPoint& _Point) = 0;
	virtual int MoveTo(const tagDTPoint& _Point) = 0;
	virtual int Offset(long _delta_X, long _delta_Y) = 0;
	virtual int Render(HDC _hdc, const tagDTRect& _Rect) = 0;
private:
};

class DTAbsEquation
{
public:
	virtual ~DTAbsEquation();
	virtual int HitTest(const tagDTAbsPoint& _Point) = 0;
	virtual int MoveTo(const tagDTAbsPoint& _Point) = 0;
	virtual int Render(HDC _hdc, const tagDTRect& _Rect) = 0;

	void ShowLabel() { m_bShowLabel = true; }
	void HideLabel() { m_bShowLabel = false; }
	bool IsShowLabel() { return m_bShowLabel; }

	static const long m_LabelFontSize;
	static const long m_LabelSpan;
protected:
	DTAbsEquation();
private:
	bool m_bShowLabel;
	long m_AdjOffesX;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	BEGIN	DTEquations
////	<!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTLineEquationBase : public DTEquation
{
public:
	virtual int Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTLineEquation : public DTLineEquationBase
{
public:
	int HitTest(const tagDTPoint& _Point);
	int Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine);
	int MoveTo(const tagDTPoint& _Point);
	int Offset(long _delta_X, long _delta_Y);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	int Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB);
	int Build(float _eCoefficentA, float _eCoefficentB);
	int BuildPerpendicular(const tagDTPoint& _PointA, const tagDTPoint& _PointB);
	int IntersectAt(DTLineEquation& _OtherLine, tagDTPoint& _PointAt);
	int Perpendicular();
	bool IsVertical();
private:
	float m_eCoefficentA;
	float m_eCoefficentB;
	long m_eOffsetX;
	long m_eOffsetY;
	bool m_bVertical;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTVerticalEquation : public DTLineEquationBase
{
public:
	int HitTest(const tagDTPoint& _Point);
	int Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine);
	int MoveTo(const tagDTPoint& _Point);
	int Offset(long _delta_X, long _delta_Y);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	int Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB);
private:
	long m_eOffsetX;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTHorizonEquation : public DTLineEquationBase
{
public:
	int HitTest(const tagDTPoint& _Point);
	int Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine);
	int MoveTo(const tagDTPoint& _Point);
	int Offset(long _delta_X, long _delta_Y);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	int Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB);
private:
	long m_eOffsetY;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTVerticalEquationCluster : public DTLineEquationBase
{
public:
	int HitTest(const tagDTPoint& _Point);
	int Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine);
	int MoveTo(const tagDTPoint& _Point);
	int Offset(long _delta_X, long _delta_Y);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	int Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB);
private:
	std::deque<DTVerticalEquation> m_Cluster;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTEllipseArcEquation : public DTEquation
{
public:
	int HitTest(const tagDTPoint& _Point);
	int MoveTo(const tagDTPoint& _Point);
	int Offset(long _delta_X, long _delta_Y);
	int Retrieve(const tagDTRect& _ViewRect, tagDTRect& _Rect, tagDTLine& _ValidLine);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	int Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB);
private:
	ENUM_ASSIST_DIRECT m_Direct;
	long m_eCoefficentA;
	long m_eCoefficentB;
	long m_eOffsetX;
	long m_eOffsetY;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTCircleEquation : public DTEquation
{
public:
	int HitTest(const tagDTPoint& _Point);
	int MoveTo(const tagDTPoint& _Point);
	int Offset(long _delta_X, long _delta_Y);
	int Retrieve(const tagDTRect& _ViewRect, tagDTRect& _Rect, tagDTLine& _ValidLine);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	int Build(const tagDTPoint& _PointO, const tagDTPoint& _PointA);
	int Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB, const tagDTPoint& _PointC);
private:
	long m_eCoefficentR;
	long m_eOffsetX;
	long m_eOffsetY;
	bool m_bAvaiable;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	!-->
////	END	DTEquations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	BEGIN	DTAbsEquations
////	<!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTEqualDifferenceVerticalAbsEquation : public DTAbsEquation
{
public:
	DTEqualDifferenceVerticalAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void EqualDifferenceOnXDateAxis(long _Diff) { m_EqualDifference = _Diff; }
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
private:
	DTCoordinateTransform* _Coord;			// stupid
	long m_OffsetDate;				// x-Date Axis
	long m_EqualDifference;			// Equal Difference of Index between Two Verticals
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTGannSequenceVerticalAbsEquation : public DTAbsEquation
{
public:
	enum { enum_gann_seq_max = 23 };
	DTGannSequenceVerticalAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
private:
	static long m_GannSequenceModel[enum_gann_seq_max];  // Gann Sequence
	DTCoordinateTransform* _Coord;
	long m_OffsetDate;				// x-Date Axis
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTFibonacciSequenceVerticalAbsEquation : public DTAbsEquation
{
public:
	enum { enum_index = 0, enum_value, enum_cols, enum_seq_max = 20 };
	DTFibonacciSequenceVerticalAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
private:
	static long m_SequenceModel[enum_seq_max][enum_cols];		// Fibonacci Sequence
	DTCoordinateTransform* _Coord;
	long m_OffsetDate;				// x-Date Axis
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTGoldenPriceHorizonAbsEquation : public DTAbsEquation
{
public:
	enum { enum_seq_max = 13 };
	DTGoldenPriceHorizonAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
protected:
	int RenderBase(HDC _hdc, const tagDTRect& _Rect);
private:
	static float m_SequenceModel[enum_seq_max];		// Golden Price Sequence
	static char* m_SequenceLabel[enum_seq_max];
	DTCoordinateTransform* _Coord;
	float m_OffsetValue;				// y-Value Axis
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTGoldenAimHorizonAbsEquation : public DTAbsEquation
{
public:
	enum { enum_seq_max = 5 };
	DTGoldenAimHorizonAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void DistanceOnYValAxis(float _Distance) { m_DistanceValue = ::abs(_Distance); }
	void PositiveOnYValAxis(bool _Positive) { m_bPositive = _Positive; }
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
protected:
	int RenderBase(HDC _hdc, const tagDTRect& _Rect);
private:
	static float m_SequenceModel[enum_seq_max];		// Golden Aim Sequence
	static char* m_SequenceLabel[enum_seq_max];
	DTCoordinateTransform* _Coord;
	float m_OffsetValue;				// y-Value Axis
	float m_DistanceValue;				//
	bool m_bPositive;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTGoldenSectionHorizonAbsEquation : public DTAbsEquation
{
public:
	enum { enum_seq_max = 11 };
	DTGoldenSectionHorizonAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void DistanceOnYValAxis(float _Distance) { m_DistanceValue = ::abs(_Distance); }
	void PositiveOnYValAxis(bool _Positive) { m_bPositive = _Positive; }
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
protected:
	int RenderBase(HDC _hdc, const tagDTRect& _Rect);
private:
	static float m_SequenceModel[enum_seq_max];		// Golden Aim Sequence
	static char* m_SequenceLabel[enum_seq_max];
	DTCoordinateTransform* _Coord;
	float m_OffsetValue;				// y-Value Axis
	float m_DistanceValue;				//
	bool m_bPositive;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTPercentageHorizonAbsEquation : public DTAbsEquation
{
public:
	enum { enum_seq_max = 3 };
	DTPercentageHorizonAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void DistanceOnYValAxis(float _Distance) { m_DistanceValue = ::abs(_Distance); }
	void PositiveOnYValAxis(bool _Positive) { m_bPositive = _Positive; }
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
protected:
	int RenderBase(HDC _hdc, const tagDTRect& _Rect);
private:
	static float m_SequenceModel[enum_seq_max];		// Golden Aim Sequence
	static char* m_SequenceLabel[enum_seq_max];
	DTCoordinateTransform* _Coord;
	float m_OffsetValue;				// y-Value Axis
	float m_DistanceValue;				//
	bool m_bPositive;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTOctBandHorizonAbsEquation : public DTAbsEquation
{
public:
	enum { enum_seq_max = 7 };
	DTOctBandHorizonAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void DistanceOnYValAxis(float _Distance) { m_DistanceValue = ::abs(_Distance); }
	void PositiveOnYValAxis(bool _Positive) { m_bPositive = _Positive; }
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
protected:
	int RenderBase(HDC _hdc, const tagDTRect& _Rect);
private:
	static float m_SequenceModel[enum_seq_max];		// Golden Aim Sequence
	static char* m_SequenceLabel[enum_seq_max];
	DTCoordinateTransform* _Coord;
	float m_OffsetValue;				// y-Value Axis
	float m_DistanceValue;				//
	bool m_bPositive;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DTGouldRayAbsEquation : public DTAbsEquation
{
public:
	enum { enum_seq_max = 2 };
	DTGouldRayAbsEquation(DTCoordinateTransform* Coord);
	int HitTest(const tagDTAbsPoint& _Point);
	int MoveTo(const tagDTAbsPoint& _Point);
	int Render(HDC _hdc, const tagDTRect& _Rect);
	void DistanceOnXDateAxis(long _Distance) { m_DistanceDate = ::abs(_Distance); }
	void DistanceOnYValAxis(float _Distance) { m_DistanceValue = ::abs(_Distance); }
	void PositiveOnYValAxis(bool _Positive) { m_bPositive = _Positive; }
	void CoordinateTransform(DTCoordinateTransform* Coord) { _Coord = Coord; }
private:
	static float m_SequenceModel[enum_seq_max];		// Golden Aim Sequence
	static char* m_SequenceLabel[enum_seq_max];
	DTCoordinateTransform* _Coord;
	long m_OffsetDate;					// x-Date Axis
	long m_DistanceDate;
	float m_OffsetValue;				// y-Value Axis
	float m_DistanceValue;				//
	bool m_bPositive;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////	!-->
////	END	DTAbsEquations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _DRAWTOOL_BASIC__H_

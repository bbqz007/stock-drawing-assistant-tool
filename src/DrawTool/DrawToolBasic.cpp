/**
MIT License
Copyright (c) 2015-2023 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
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

#include "stdafx.h"
#include "DrawTool.h"
#include "DrawToolBasicUtil.h"
#include <string.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
#include "resource.h"

static long retrieve_timezone()
{
	__timeb32 timeb32;
	_ftime32(&timeb32);
	return timeb32.timezone * 60;
}

long DTHelperConvertTm(DWORD _Date, DWORD _Hour, DWORD _Min, DWORD _Sec)
{
	tm m = { _Sec, _Min, _Hour, _Date % 100, ((_Date / 100) % 100) - 1, _Date / 10000 - 1900, 0, 0, 0 };
	return _mkgmtime32(&m);
}

long DTHelperLocalTimeToGmTime(long _Localtime)
{
	static long s_tzone_offset = retrieve_timezone();
	return _Localtime - s_tzone_offset;
}

const long DTAbsEquation::m_LabelFontSize = 14;
const long DTAbsEquation::m_LabelSpan = 1;

DTAbsEquation::DTAbsEquation()
{
	m_bShowLabel = true;
	m_AdjOffesX = 0;
}

DTAssistGraphDescriptionScript::DTAssistGraphDescriptionScript()
{
	m_Location1 = 0;
	m_Location2 = 0;
	m_Location3 = 0;
	m_Text = 0;
	m_Custom = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DTAssistGraphDescriptionScriptMap
DTAssistGraphDescriptionScriptMap& DTAssistGraphDescriptionScriptMap::Instance()
{
	static DTAssistGraphDescriptionScriptMap _Inst;
	return _Inst;
}

DTAssistGraphDescriptionScriptMap::DTAssistGraphDescriptionScriptMap()
{
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Segment, "线段");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Line, "直线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow, "箭头");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Line_Extension, "射线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Price_Channels, "价格通道线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines, "平行直线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Arc, "圆弧线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Circle, "圆");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Circumcircle, "外接圆");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_GPHs, "黄金价位线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_GAHs, "黄金目标线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_GSHs, "黄金分割");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_PHs, "百分比线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_BHs, "波段线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_LRA, "线形回归带");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_LREA, "延长线形回归带");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_LRL, "线形回归线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Cycle, "周期线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Fibonacci, "费波拉契线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Gann, "江恩时间序列");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Gould_Rays, "阻速线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Gann_Rays, "江恩角度线");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Rectangle, "矩形");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow, "涨标记");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow, "跌标记");
	InsertTypeName( ENUM_ASSIST_GRAPH_TYPE_Text, "文字注释");
}

DTAssistGraphDescriptionScriptMap::~DTAssistGraphDescriptionScriptMap()
{
	if(!m_szTypeScriptMap.empty())
	{
		std::map<DWORD, char*>::iterator it = m_szTypeScriptMap.begin();
		std::map<DWORD, char*>::iterator ite = m_szTypeScriptMap.end();
		for(; it != ite; ++it)
		{
			if(it->second)
			{
				delete [] it->second;
				it->second = NULL;
			}
		}
	}

	if(!m_szViewIdScriptMap.empty())
	{
		std::map<DWORD, char*>::iterator it = m_szViewIdScriptMap.begin();
		std::map<DWORD, char*>::iterator ite = m_szViewIdScriptMap.end();
		for(; it != ite; ++it)
		{
			if(it->second)
			{
				delete [] it->second;
				it->second = NULL;
			}
		}
	}

	if(!m_szPeriodScriptMap.empty())
	{
		std::map<DWORD, char*>::iterator it = m_szPeriodScriptMap.begin();
		std::map<DWORD, char*>::iterator ite = m_szPeriodScriptMap.end();
		for(; it != ite; ++it)
		{
			if(it->second)
			{
				delete [] it->second;
				it->second = NULL;
			}
		}
	}
}

void DTAssistGraphDescriptionScriptMap::InsertTypeName(DWORD _Type, char* _Name)
{
	if(_Name)
	{
		char* _TypeName = new char[strlen(_Name) + 1];
		strcpy(_TypeName, _Name);
		std::map<DWORD, char*>::iterator it = m_szTypeScriptMap.find(_Type);
		std::map<DWORD, char*>::iterator ite = m_szTypeScriptMap.end();
		if(it == ite)
		{
			m_szTypeScriptMap[_Type] = _TypeName;
		}
		else
		{
			delete [] it->second;
			it->second = _TypeName;
		}
		_TypeName = NULL;
	}
}

void DTAssistGraphDescriptionScriptMap::InsertViewName(DWORD _ViewId, char* _Name)
{
	if(_Name)
	{
		char* _ViewName = new char[strlen(_Name) + 1];
		strcpy(_ViewName, _Name);
		std::map<DWORD, char*>::iterator it = m_szViewIdScriptMap.find(_ViewId);
		std::map<DWORD, char*>::iterator ite = m_szViewIdScriptMap.end();
		if(it == ite)
		{
			m_szViewIdScriptMap[_ViewId] = _ViewName;
		}
		else
		{
			delete [] it->second;
			it->second = _ViewName;
		}
		_ViewName = NULL;
	}
}

void DTAssistGraphDescriptionScriptMap::InsertPeriodName(DWORD _Period, char* _Name)
{
	if(_Name)
	{
		char* _PeriodName = new char[strlen(_Name) + 1];
		strcpy(_PeriodName, _Name);
		std::map<DWORD, char*>::iterator it = m_szPeriodScriptMap.find(_Period);
		std::map<DWORD, char*>::iterator ite = m_szPeriodScriptMap.end();
		if(it == ite)
		{
			m_szPeriodScriptMap[_Period] = _PeriodName;
		}
		else
		{
			delete [] it->second;
			it->second = _PeriodName;
		}
		_PeriodName = NULL;
	}
}

void DTAssistGraphDescriptionScriptMap::InsertDayPeriod(DWORD _Type)
{
	m_timePeriodScriptMap[_Type] = 1;
}


void DTAssistGraphDescriptionScriptMap::TypeName(DWORD _Type, char* _Name, size_t _Size)
{
	if(!_Name)
		return ;

	std::map<DWORD, char*>::iterator it = m_szTypeScriptMap.find(_Type);
	std::map<DWORD, char*>::iterator ite = m_szTypeScriptMap.end();
	if(it != ite && it->second)
	{
		::strncpy(_Name, it->second, _Size);
	}
	else
	{
		::memset(_Name, 0, _Size);
	}
}

void DTAssistGraphDescriptionScriptMap::ViewName(DWORD _ViewId, char* _Name, size_t _Size)
{
	if(!_Name)
		return ;

	std::map<DWORD, char*>::iterator it = m_szViewIdScriptMap.find(_ViewId);
	std::map<DWORD, char*>::iterator ite = m_szViewIdScriptMap.end();
	if(it != ite && it->second)
	{
		::strncpy(_Name, it->second, _Size);
	}
	else
	{
		::memset(_Name, 0, _Size);
	}
}

void DTAssistGraphDescriptionScriptMap::PeriodName(DWORD _Period, char* _Name, size_t _Size)
{
	if(!_Name)
		return ;

	std::map<DWORD, char*>::iterator it = m_szPeriodScriptMap.find(_Period);
	std::map<DWORD, char*>::iterator ite = m_szPeriodScriptMap.end();
	if(it != ite && it->second)
	{
		::strncpy(_Name, it->second, _Size);
	}
	else
	{
		::memset(_Name, 0, _Size);
	}
}


BOOL DTAssistGraphDescriptionScriptMap::IsPeriodByTheDay(DWORD _Period)
{
	return m_timePeriodScriptMap.find(_Period) != m_timePeriodScriptMap.end();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTAssistGraphPersistDescription::DTAssistGraphPersistDescription()
: m_CheckSum(0)
{
	memset(&m_Description, 0, sizeof(m_Description));
}

BOOL DTAssistGraphPersistDescription::TestCheckSum()
{
	unsigned char* _Walk = (unsigned char*) this;
	unsigned char* _Walk_End = _Walk + sizeof(m_Description);
	DWORD _CheckSum = 0;

	for(; _Walk < _Walk_End; ++_Walk)
	{
		_CheckSum += *_Walk;
	}

	return _CheckSum == m_CheckSum;
}

DWORD DTAssistGraphPersistDescription::CheckSummat()
{
	unsigned char* _Walk = (unsigned char*) this;
	unsigned char* _Walk_End = _Walk + sizeof(m_Description);

	for(m_CheckSum = 0; _Walk < _Walk_End; ++_Walk)
	{
		m_CheckSum += *_Walk;
	}

	return m_CheckSum;
}

DTAssistGraphPersistText::DTAssistGraphPersistText()
: m_CheckSum(0)
{
	memset(&m_Raw, 0, sizeof(m_Raw));
}

BOOL DTAssistGraphPersistText::TestCheckSum()
{
	unsigned char* _Walk = (unsigned char*) this;
	unsigned char* _Walk_End = _Walk + sizeof(m_Raw);
	DWORD _CheckSum = 0;

	for(; _Walk < _Walk_End; ++_Walk)
	{
		_CheckSum += *_Walk;
	}

	return _CheckSum == m_CheckSum;
}

DWORD DTAssistGraphPersistText::CheckSummat()
{
	unsigned char* _Walk = (unsigned char*) this;
	unsigned char* _Walk_End = _Walk + sizeof(m_Raw);

	for(m_CheckSum = 0; _Walk < _Walk_End; ++_Walk)
	{
		m_CheckSum += *_Walk;
	}

	return m_CheckSum;
}

void DTAssistGraphPersistText::Revise()
{
	char* _Walk = m_Raw + sizeof(m_Identity) + sizeof(m_Text);
	char* _Walk_End = m_Raw + sizeof(m_Raw);

	memset(_Walk, 0, _Walk_End - _Walk);
}

void DTAssistGraphPersistText::CopyText(DTAssistGraphPersistText& _Other)
{
	memcpy(m_Text, _Other.m_Text, sizeof(m_Text));
	Revise();
}

void DTAssistGraphPersistText::CopyText(char* _Text)
{
	memset(m_Text, 0, sizeof(m_Text));
	memcpy(m_Text, _Text, max(0, min(sizeof(m_Text), ::strlen(_Text))));
	Revise();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// : DTAbsoluteCoordinate
struct LessForIndexOfDate : public std::less<DTAbsoluteCoordinate::_Ty_Point>
{
	bool operator()(const DTAbsoluteCoordinate::_Ty_Point& _Left, const DTAbsoluteCoordinate::_Ty_Point& _Right) const
	{
		return _Left.first.m_date < _Right.first.m_date;
	}
};

struct LessForIndexOfViewX : public std::less<DTAbsoluteCoordinate::_Ty_Point>
{
	bool operator()(const DTAbsoluteCoordinate::_Ty_Point& _Left, const DTAbsoluteCoordinate::_Ty_Point& _Right) const
	{
		return _Left.second.xRight < _Right.second.xLeft;
	}
};

DTAbsoluteCoordinate::DTAbsoluteCoordinate()
{

}

long DTAbsoluteCoordinate::IndexOfDate(long _xDate)
{
	long ret = -1;
	long idx = 0;
	_Ty_Point _Target;
	_Target.first.m_date = _xDate;

	std::deque<_Ty_Point>::iterator itb = m_qeuPoints.begin();
	std::deque<_Ty_Point>::iterator ite = m_qeuPoints.end();
	std::deque<_Ty_Point>::iterator it = std::lower_bound(itb, ite, _Target, LessForIndexOfDate());
	if(it != ite)
	{
		if(it->first.m_date == _xDate)
		{
			ret = it - m_qeuPoints.begin();
		}
		//++idx;
	}
	return ret;
}

long DTAbsoluteCoordinate::IndexOfViewX(long _xView)
{
	long ret = -1;
	if(m_qeuPoints.empty())
		return ret;
	if(_xView <= m_StkBegin.second.xRight)
		return 0;
	if(_xView >= m_StkEnd.second.xLeft)
		return m_qeuPoints.size() - 1;

	long idx = 0;
	_Ty_Point _Target;
	_Target.second.xLeft = _xView;
	_Target.second.xTarget = _xView;
	_Target.second.xRight = _xView;

	std::deque<_Ty_Point>::iterator itb = m_qeuPoints.begin();
	std::deque<_Ty_Point>::iterator ite = m_qeuPoints.end();

	std::deque<_Ty_Point>::iterator it = std::lower_bound(itb, ite, _Target, LessForIndexOfViewX());
	if(it != ite)
	{
		if(it->second.xLeft <= _xView && _xView <= it->second.xRight)
		{
			ret = it - m_qeuPoints.begin();
		}
	}
	return ret;
}

long DTAbsoluteCoordinate::IndexOfViewX2(long _xView)
{
	long ret = -1;
	if(m_qeuPoints.empty())
		return ret;
	if(_xView <= m_StkBegin.second.xRight)
		return 0;
	if(_xView >= m_StkEnd.second.xLeft)
		return m_qeuPoints.size() - 1;

	long idx = 0;
	_Ty_Point _Target;
	_Target.second.xLeft = _xView;
	_Target.second.xTarget = _xView;
	_Target.second.xRight = _xView;

	std::deque<_Ty_Point>::iterator itb = m_qeuPoints.begin();
	std::deque<_Ty_Point>::iterator ite = m_qeuPoints.end();

	std::deque<_Ty_Point>::iterator it = std::lower_bound(itb, ite, _Target, LessForIndexOfViewX());
	if(it != ite)
	{
		ret = it - m_qeuPoints.begin();
	}
	return ret;
}

long DTAbsoluteCoordinate::DateByIndex(long _xIndex)
{
	if(m_qeuPoints.empty() || _xIndex < 0 || _xIndex > m_qeuPoints.size())
		return 0;
	return m_qeuPoints[ _xIndex ].first.m_date;
}

tagKValue DTAbsoluteCoordinate::KValueByIndex(long _xIndex)
{
	tagKValue _KValue;
	if(!m_qeuPoints.empty() && _xIndex >= 0 && _xIndex < m_qeuPoints.size())
		return m_qeuPoints[ _xIndex ].first;
	return _KValue;
}

tagViewXAxisUnit DTAbsoluteCoordinate::ViewXUnitByIndex(long _xIndex)
{
	tagViewXAxisUnit _ViewXUnit(LONG_NEGATIVE_LIMIT_RESULT, LONG_NEGATIVE_LIMIT_RESULT, LONG_NEGATIVE_LIMIT_RESULT);
	if(!m_qeuPoints.empty() && _xIndex >= 0 && _xIndex < m_qeuPoints.size())
		return m_qeuPoints[ _xIndex ].second;
	return _ViewXUnit;
}

long DTAbsoluteCoordinate::ViewXByIndex(long _xIndex)
{
	if(!m_qeuPoints.empty() && _xIndex >= 0 && _xIndex < m_qeuPoints.size())
		return m_qeuPoints[ _xIndex ].second.xTarget;
	return LONG_NEGATIVE_LIMIT_RESULT;
}

long DTAbsoluteCoordinate::AbsXtoViewX(long _xDate)
{
	if(m_qeuPoints.empty())
		return LONG_NEGATIVE_LIMIT_RESULT;
	long _index_xDate = IndexOfDate(_xDate);
	if(_index_xDate < 0 || _index_xDate >= m_qeuPoints.size())
		return LONG_NEGATIVE_LIMIT_RESULT;
	return m_qeuPoints[_index_xDate].second.xTarget;
}

long DTAbsoluteCoordinate::ViewXtoAbsX(long _xView)
{
	long _index_xView = IndexOfViewX(_xView);
	return DateByIndex(_index_xView);
}

long DTAbsoluteCoordinate::ViewXtoAbsX2(long _xView)
{
	long _index_xView = IndexOfViewX2(_xView);
	return DateByIndex(_index_xView);
}

long DTAbsoluteCoordinate::AdjustViewX(long _xView)
{
	long _index_xView = IndexOfViewX2(_xView);
	return ViewXByIndex(_index_xView);
}

int DTAbsoluteCoordinate::Build(tagKValue* _Vals, tagViewXAxisUnit* _Units, size_t _Size, size_t _IndexForm, size_t _IndexTo)
{
	//::OutputDebugStringA("DTAbsoluteCoordinate::Build()");
	if(_Size == 0)
		return 0;
	if(_IndexForm > _IndexTo || _IndexForm < 0 || _IndexTo >= _Size)
		return -1;

	m_qeuPoints.clear();
	for(size_t i = 0; i < _Size; ++i)
	{
		m_qeuPoints.push_back(std::make_pair(_Vals[i], _Units[i]));
	}

	m_idxStkBegin = 0;
	m_StkBegin = m_qeuPoints[m_idxStkBegin];
	m_idxStkEnd = _Size - 1;
	m_StkEnd = m_qeuPoints[m_idxStkEnd];
	m_idxOrigin = _IndexForm;
	m_Origin = m_qeuPoints[m_idxOrigin];
	m_idxBound = _IndexTo;
	m_Bound = m_qeuPoints[m_idxBound];

	return _Size;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// : DTCoordinateTransform
DTCoordinateTransform::DTCoordinateTransform(DTAssistGraphics* _Graphics)
: m_Graphics(_Graphics)
{
	memset(&m_rcVirtualRect, 0, sizeof(m_rcVirtualRect));
	memset(&m_rcViewRect, 0, sizeof(m_rcViewRect));
	memset(&m_rcValidateRect, 0, sizeof(m_rcValidateRect));

	m_eUnitHeight = 0.0;
	m_eUnitWidth = 0;

	memset(&m_ptViewAbsOrigin, 0, sizeof(m_ptViewAbsOrigin));
	memset(&m_ptViewOrigin, 0, sizeof(m_ptViewOrigin));
}

DTCoordinateTransform& DTCoordinateTransform::operator= (DTCoordinateTransform& _Other)
{
	m_Graphics = _Other.m_Graphics;

	memcpy(&m_rcVirtualRect, &_Other.m_rcVirtualRect, sizeof(m_rcVirtualRect));
	memcpy(&m_rcViewRect, &_Other.m_rcViewRect, sizeof(m_rcViewRect));
	memcpy(&m_rcValidateRect, &_Other.m_rcValidateRect, sizeof(m_rcValidateRect));

	m_eUnitHeight = _Other.m_eUnitHeight;
	m_eUnitWidth = _Other.m_eUnitHeight;

	memcpy(&m_ptViewAbsOrigin, &_Other.m_ptViewAbsOrigin, sizeof(m_ptViewAbsOrigin));
	memcpy(&m_ptViewOrigin, &_Other.m_ptViewOrigin, sizeof(m_ptViewOrigin));
	return *this;
}

void DTCoordinateTransform::SetViewRect(const tagDTRect& _ViewRect, const tagDTRange& _ValueRange)
{
	m_rcViewRect = _ViewRect;
	m_eUnitHeight = (_ValueRange.highBound - _ValueRange.lowBound) / (m_rcViewRect.bottom - m_rcViewRect.top);
	m_eUnitWidth = m_Graphics->Coordinate().UnitWidth();
	m_ptViewAbsOrigin.xDate = m_Graphics->Coordinate().OriginDate();
	m_ptViewAbsOrigin.yValue = _ValueRange.lowBound;
	m_ptViewOrigin.x = m_rcViewRect.left;
	m_ptViewOrigin.y = m_rcViewRect.bottom;
}

long DTCoordinateTransform::AbsYtoViewY(float _yValue)
{
	// 两种坐标的y轴正方向相反
	float _delta_yValue = (_yValue - m_ptViewAbsOrigin.yValue) / m_eUnitHeight;
	if(_delta_yValue > 1e-4)
		_delta_yValue += 0.5;
	else if(_delta_yValue < -1e-4)
		_delta_yValue -= 0.5;
	return m_ptViewOrigin.y - (long) _delta_yValue;
}

float DTCoordinateTransform::ViewYtoAbsY(long _yView)
{
	long _delta_yView = _yView - m_ptViewOrigin.y;
	return m_ptViewAbsOrigin.yValue - _delta_yView * m_eUnitHeight;
}

int DTCoordinateTransform::Build(const tagDTRect& _ViewRect, float _yValTop, float _yValBottom)
{
	m_rcViewRect = _ViewRect;
	m_eUnitHeight = (_yValTop - _yValBottom) / (float)(m_rcViewRect.bottom - m_rcViewRect.top);
	m_ptViewAbsOrigin.xDate = m_Graphics->Coordinate().BeginDate();
	m_ptViewAbsOrigin.yValue = _yValBottom;
	m_ptViewOrigin.x = m_rcViewRect.left;
	m_ptViewOrigin.y = m_rcViewRect.bottom;
	return 0;
}

int DTCoordinateTransform::LoadFromAssistGraphics(DWORD _ViewId)
{
	m_Graphics->LoadCoordinateTransform(*this, _ViewId);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// : DTAssistGraph
const long DTAssistGraph::m_nAnchorThickness = 3;

DTAssistGraph::~DTAssistGraph()
{

}

DTAssistGraph::DTAssistGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: m_Graphics(_Graphics), m_coorTransform(_Graphics)
{
	memset(&m_rcVirtualRect, 0, sizeof(m_rcVirtualRect));
	memset(&m_rcValidateRect, 0, sizeof(m_rcValidateRect));
	memset(&m_propLine, 0, sizeof(m_propLine));
	m_nValidStatus = 0;

	m_bUseDefault = true;
	m_bAvaiable = false;
	m_descGraph = _descGraph;

	memset(&m_ptViewLocationP1, 0, sizeof(m_ptViewLocationP1));
	memset(&m_ptViewLocationP2, 0, sizeof(m_ptViewLocationP2));
	memset(&m_ptViewLocationP3, 0, sizeof(m_ptViewLocationP3));

	m_coorTransform.LoadFromAssistGraphics(m_descGraph.m_Identity.nViewId);
}

DTAssistGraph::DTAssistGraph(DTAssistGraph& _Other)
: m_coorTransform(m_Graphics)
{
	m_Graphics = _Other.m_Graphics;
	m_coorTransform = _Other.m_coorTransform;

	memcpy(&m_rcVirtualRect, &_Other.m_rcVirtualRect, sizeof(m_rcVirtualRect));
	memcpy(&m_rcValidateRect, &_Other.m_rcValidateRect, sizeof(m_rcValidateRect));
	memcpy(&m_propLine, &_Other.m_propLine, sizeof(m_propLine));
	m_nValidStatus = _Other.m_nValidStatus;
	m_bUseDefault = _Other.m_bUseDefault;
	m_bAvaiable = _Other.m_bAvaiable;

	m_descGraph = _Other.m_descGraph;

	memcpy(&m_ptViewLocationP1, &_Other.m_ptViewLocationP1, sizeof(m_ptViewLocationP1));
	memcpy(&m_ptViewLocationP2, &_Other.m_ptViewLocationP2, sizeof(m_ptViewLocationP2));
	memcpy(&m_ptViewLocationP3, &_Other.m_ptViewLocationP3, sizeof(m_ptViewLocationP3));
}

void DTAssistGraph::SetViewRect(const tagDTRect& _ViewRect, const tagDTRange& _ValueRange)
{
	m_coorTransform.SetViewRect(_ViewRect, _ValueRange);
}

void DTAssistGraph::Commit(DTAssistGraphBook& _Book)
{
	m_Graphics->CommitAssistGraph(_Book, m_descGraph);
}

DTAssistGraph* DTAssistGraph::CreateShadow()
{
	return m_Graphics->CreateAssistGraphInstance(this);
}

void DTAssistGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	m_ptViewLocationP3.x += _delta_X;
	m_ptViewLocationP3.y += _delta_Y;

	EndMove(_delta_X, _delta_Y);
}

void DTAssistGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;
	EndMove();
}

void DTAssistGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;
	EndMove();
}

void DTAssistGraph::MoveP3To(const tagDTPoint& _Point)
{
	m_ptViewLocationP3.x = _Point.x;
	m_ptViewLocationP3.y = _Point.y;
	EndMove();
}

int DTAssistGraph::EndMove(long _delta_X, long _delta_Y)
{
	return Build();
}

int DTAssistGraph::EndMove()
{
	return Build();
}

int DTAssistGraph::BuildAccordToViewPoint(const tagDTPoint* _Point1, const tagDTPoint* _Point2, const tagDTPoint* _Point3)
{
	m_ptViewLocationP1.x = _Point1->x;
	m_ptViewLocationP1.y = _Point1->y;
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	if(_Point2)
	{
		m_ptViewLocationP2.x = _Point2->x;
		m_ptViewLocationP2.y = _Point2->y;
		m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
		m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);
	}

	if(_Point3)
	{
		m_ptViewLocationP3.x = _Point3->x;
		m_ptViewLocationP3.y = _Point3->y;
		m_descGraph.m_ptLocations.ptAbsoluteP3.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP3.x);
		m_descGraph.m_ptLocations.ptAbsoluteP3.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP3.y);
	}

	return BuildAccordToDescription();
}

int DTAssistGraph::EndClone(DTAssistGraph& _Other)
{
	return 0;
}

int DTAssistGraph::Clone(DTAssistGraph& _Other)
{
	m_Graphics = _Other.m_Graphics;

	m_coorTransform = _Other.m_coorTransform;
	m_descGraph = _Other.m_descGraph;

	m_rcVirtualRect = _Other.m_rcVirtualRect;
	m_rcValidateRect = _Other.m_rcValidateRect;
	m_nValidStatus = _Other.m_nValidStatus;
	m_propLine = _Other.m_propLine;
	m_bUseDefault = _Other.m_bUseDefault;
	m_bAvaiable = _Other.m_bAvaiable;

	m_ptViewLocationP1 = _Other.m_ptViewLocationP1;
	m_ptViewLocationP2 = _Other.m_ptViewLocationP2;
	m_ptViewLocationP3 = _Other.m_ptViewLocationP3;
	return EndClone(_Other);
}

int DTAssistGraph::_NextEditModeOfSimpleLine(int _CurMode)
{
	int _NextMode = ENUM_ASSIST_EDIT_MODE_UNKNOWN;
	switch(_CurMode)
	{
	case ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING:		_NextMode = ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1;	break;
	case ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1:	_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P2;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET:	_NextMode = ENUM_ASSIST_EDIT_MODE_OFFSET;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P1:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P1;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P2:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P2;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P1:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P2:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_OFFSET:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_COMMIT:				_NextMode = ENUM_ASSIST_EDIT_MODE_NULL;		break;
	default:
		break;
	}
	return _NextMode;
}

int DTAssistGraph::_NextEditModeOfParallelLine(int _CurMode)
{
	int _NextMode = ENUM_ASSIST_EDIT_MODE_UNKNOWN;
	switch(_CurMode)
	{
	case ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING:		_NextMode = ENUM_ASSIST_PRE_EDIT_MODE_SET_P1;	break;
	case ENUM_ASSIST_PRE_EDIT_MODE_SET_P1:			_NextMode = ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2;	break;
	case ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2:			_NextMode = ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P2_P2;	break;
	case ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P2_P2:	_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P3;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET:	_NextMode = ENUM_ASSIST_EDIT_MODE_OFFSET;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P1:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P1;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P2:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P2;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P3:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P3;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P1:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P2:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P3:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_OFFSET:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_COMMIT:				_NextMode = ENUM_ASSIST_EDIT_MODE_NULL;		break;
	default:
		break;
	}
	return _NextMode;
}

int DTAssistGraph::_NextEditModeOfSinglePoint(int _CurMode)
{
	int _NextMode = ENUM_ASSIST_EDIT_MODE_UNKNOWN;
	switch(_CurMode)
	{
	case ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING:		_NextMode = ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1;	break;
	case ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1:	_NextMode = ENUM_ASSIST_EDIT_MODE_OFFSET;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET:	_NextMode = ENUM_ASSIST_EDIT_MODE_OFFSET;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P1:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P1;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P1:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_OFFSET:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_COMMIT:				_NextMode = ENUM_ASSIST_EDIT_MODE_NULL;		break;
	default:
		break;
	}
	return _NextMode;
}

int DTAssistGraph::_NextEditModeOfText(int _CurMode)
{
	int _NextMode = ENUM_ASSIST_EDIT_MODE_UNKNOWN;
	switch(_CurMode)
	{
	case ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING:		_NextMode = ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1;	break;
	case ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1:	_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET:	_NextMode = ENUM_ASSIST_EDIT_MODE_OFFSET;	break;
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P1:		_NextMode = ENUM_ASSIST_EDIT_MODE_EDIT_P1;	break;
	case ENUM_ASSIST_EDIT_MODE_EDIT_P1:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_OFFSET:				_NextMode = ENUM_ASSIST_EDIT_MODE_COMMIT;	break;
	case ENUM_ASSIST_EDIT_MODE_COMMIT:				_NextMode = ENUM_ASSIST_EDIT_MODE_NULL;		break;
	default:
		break;
	}
	return _NextMode;
}

DTAssistGraphDescriptionScript DTAssistGraph::_DescriptionScriptOfSinglePoint()
{
	DTAssistGraphDescriptionScript _RetScript;
	_RetScript.m_Location1 = 1;
	_RetScript.m_Location2 = 0;
	_RetScript.m_Location3 = 0;
	_RetScript.m_Text = 0;
	_RetScript.m_Custom = 0;
	return _RetScript;
}

DTAssistGraphDescriptionScript DTAssistGraph::_DescriptionScriptOfSimpleLine()
{
	DTAssistGraphDescriptionScript _RetScript;
	_RetScript.m_Location1 = 1;
	_RetScript.m_Location2 = 1;
	_RetScript.m_Location3 = 0;
	_RetScript.m_Text = 0;
	_RetScript.m_Custom = 0;
	return _RetScript;
}

DTAssistGraphDescriptionScript DTAssistGraph::_DescriptionScriptOfParallelLine()
{
	DTAssistGraphDescriptionScript _RetScript;
	_RetScript.m_Location1 = 1;
	_RetScript.m_Location2 = 1;
	_RetScript.m_Location3 = 1;
	_RetScript.m_Text = 0;
	_RetScript.m_Custom = 0;
	return _RetScript;
}

DTAssistGraphDescriptionScript DTAssistGraph::_DescriptionScriptOfText()
{
	DTAssistGraphDescriptionScript _RetScript;
	_RetScript.m_Location1 = 1;
	_RetScript.m_Location2 = 0;
	_RetScript.m_Location3 = 0;
	_RetScript.m_Text = 1;
	_RetScript.m_Custom = 1;
	return _RetScript;
}


int DTAssistGraph::NextEditModeOfSinglePoint(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSinglePoint(_CurMode);
}

int DTAssistGraph::NextEditModeOfSimpleLine(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTAssistGraph::NextEditModeOfParallelLine(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfParallelLine(_CurMode);
}

int DTAssistGraph::HitAnchor(const tagDTPoint& _Anchor, const tagDTPoint& _Point, int _RetSucceed)
{
	return ((_Anchor.x - m_nAnchorThickness) <= _Point.x && _Point.x <= (_Anchor.x + m_nAnchorThickness)
				&& (_Anchor.y - m_nAnchorThickness) <= _Point.y && _Point.y <= (_Anchor.y + m_nAnchorThickness))
				? _RetSucceed : ENUM_ASSIST_HITTEST_NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// : DTAssistGraphBook
DTAssistGraphBook& DTAssistGraphBook::Instance()
{
	static DTAssistGraphBook _Inst;
	return _Inst;
}

void DTAssistGraphBook::LoadFromTest()
{
	DTAssistGraphDescription _Descrption[4];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Segment;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20120216);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2481.79;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2246.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Line;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20020207);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2081.79;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 3046.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow;
	_Descrption[0].m_Identity.tTime = 1;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20111229);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2481.79;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2746.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow;
	_Descrption[0].m_Identity.tTime = 2;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2481.79;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20111229);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2746.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 3;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20111229);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2481.79;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20111229);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 3046.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Line_Extension;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20071207);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2246.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2246.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Arc;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20071207);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 5481.79;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2246.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Circle;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(19990527);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 1285.32;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(19990621);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 1285.32;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Circle;
	_Descrption[0].m_Identity.tTime = 1;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20120227);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2390.54;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120307);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2451.32;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Circle;
	_Descrption[0].m_Identity.tTime = 2;
	memcpy(_Descrption[0].m_Identity.stkCode, "SH000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20120529);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2360.47;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120531);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 2375.57;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Circumcircle;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ399001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20120227);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 9749.75;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120313);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 10421.52;
	_Descrption[0].m_ptLocations.ptAbsoluteP3.xDate = DTHelperConvertTm(20120328);
	_Descrption[0].m_ptLocations.ptAbsoluteP3.yValue = 9545.44;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 0;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Circumcircle;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ399001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(19990527);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 3285.32;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(19990621);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 3285.32;
	_Descrption[0].m_ptLocations.ptAbsoluteP3.xDate = DTHelperConvertTm(19990609);
	_Descrption[0].m_ptLocations.ptAbsoluteP3.yValue = 3585.32;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_GPHs;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ399001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(19990527);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 2946.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_GAHs;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ399001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20010816);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 3246.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(19990527);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 4585.32;
	_Descrption[0].m_ptLocations.ptAbsoluteP3.xDate = DTHelperConvertTm(20070409);
	_Descrption[0].m_ptLocations.ptAbsoluteP3.yValue = 10485.32;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Cycle;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000001", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20010822);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 22.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(19990527);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 15.32;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Fibonacci;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000002", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20010822);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 22.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Gann;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000007", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20010822);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 22.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_LRA;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000004", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20010822);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 22.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20060215);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 22.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_GSHs;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000004", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20060215);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 25.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20060215);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 19.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_LREA;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000005", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20010822);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 22.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20070912);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 22.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_PHs;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000005", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20070912);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 9.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20070912);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 13.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_LRL;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000006", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20070109);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 22.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20081222);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 22.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_BHs;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000006", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20081222);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 23.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20081222);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 16.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 0;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Price_Channels;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000009", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20110406);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 17.17;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20110727);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 16.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP3.xDate = DTHelperConvertTm(20101208);
	_Descrption[0].m_ptLocations.ptAbsoluteP3.yValue = 21.04;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000009", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20110727);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 17.17;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20101208);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 16.11;
	_Descrption[0].m_ptLocations.ptAbsoluteP3.xDate = DTHelperConvertTm(20100916);
	_Descrption[0].m_ptLocations.ptAbsoluteP3.yValue = 21.04;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Gould_Rays;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000010", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20090928);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 9.17;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120320);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 12.11;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Gann_Rays;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000011", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20111018);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 10.28;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120514);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 7.30;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000011", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20111018);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 10.28;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000011", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20120514);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 7.30;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];

	_Descrption[0].m_Identity.nViewId = 0;
	_Descrption[0].m_Identity.nPeriod = 5;
	_Descrption[0].m_Identity.tTime = 1;
	_Descrption[0].m_Identity.nType = ENUM_ASSIST_GRAPH_TYPE_Rectangle;
	memcpy(_Descrption[0].m_Identity.stkCode, "SZ000011", 8);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.xDate = DTHelperConvertTm(20111018);
	_Descrption[0].m_ptLocations.ptAbsoluteP1.yValue = 9.28;
	_Descrption[0].m_ptLocations.ptAbsoluteP2.xDate = DTHelperConvertTm(20120514);
	_Descrption[0].m_ptLocations.ptAbsoluteP2.yValue = 8.30;
	m_Contents[_Descrption[0].m_Identity] = _Descrption[0];
}

DTAssistGraphBook::DTAssistGraphBook()
: m_TextFixedLength(256), m_MaxPersistLength(1024)
{
	char* _Default_Contents_FilePath = "line.dat";
	char* _Default_Dictionary_FilePath = "text.dat";

	::InitializeCriticalSection(&m_Lock);
	memset(m_Contents_FilePath, 0, sizeof(m_Contents_FilePath));
	memset(m_Dictionary_FilePath, 0, sizeof(m_Dictionary_FilePath));

	memcpy(m_Contents_FilePath, _Default_Contents_FilePath, strlen(_Default_Contents_FilePath));
	memcpy(m_Dictionary_FilePath, _Default_Dictionary_FilePath, strlen(_Default_Dictionary_FilePath));

	ReadFromPersist();
	//LoadFromTest();
}

DTAssistGraphBook::~DTAssistGraphBook()
{
	WriteToPersist();

	ReleaseDictionary();
	ReleaseContents();

	{ DTAssistGraphBookLocker _Lock_(&m_Lock); }
	::DeleteCriticalSection(&m_Lock);
}

LRESULT DTAssistGraphBook::ReadPersistDescription_i()
{
	LRESULT _lRes = 0;
	FILE* _fp = ::fopen(m_Contents_FilePath, "rb");
	if(!_fp)
		return 0;

	DTAssistGraphPersistDescription _Buffer;
	size_t _BytesToRead = sizeof(_Buffer);
	while(::fread(&_Buffer, 1, _BytesToRead, _fp) == _BytesToRead && _lRes <= m_MaxPersistLength)
	{
		if(_Buffer.TestCheckSum())
		{
			InsertGraph(_Buffer.m_Description.m_Identity, _Buffer.m_Description);
			++_lRes;
		}
	}

	::fclose(_fp);
	return _lRes;
}

LRESULT DTAssistGraphBook::ReadPersistText_i()
{
	LRESULT _lRes = 0;
	FILE* _fp = ::fopen(m_Dictionary_FilePath, "rb");
	if(!_fp)
		return 0;

	DTAssistGraphPersistText _Buffer;
	size_t _BytesToRead = sizeof(_Buffer);
	while(::fread(&_Buffer, 1, _BytesToRead, _fp) == _BytesToRead && _lRes <= m_MaxPersistLength)
	{
		if(_Buffer.TestCheckSum())
		{
			_Buffer.Revise();
			m_Dictionary[ _Buffer.m_Identity ] = AllocateFixedText(_Buffer.m_Text);
			++_lRes;
		}
	}

	::fclose(_fp);
	return _lRes;
}

LRESULT DTAssistGraphBook::ReadPersistDescription()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	return ReadPersistDescription_i();
}

LRESULT DTAssistGraphBook::ReadPersistText()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	return ReadPersistText_i();
}

LRESULT DTAssistGraphBook::ReadFromPersist_i()
{
	ReadPersistDescription_i();
	ReadPersistText_i();
	return 0;
}

LRESULT DTAssistGraphBook::ReadFromPersist()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	return ReadFromPersist_i();
}

LRESULT DTAssistGraphBook::WritePersistDescription_i()
{
	LRESULT _lRes = 0;
	FILE* _fp = ::fopen(m_Contents_FilePath, "wb");
	if(!_fp)
		return 0;

	char _DualPage[ 8192 ];
	char* _Walk = _DualPage;
	char* _Walk_End = _DualPage + sizeof(_DualPage);
	DTAssistGraphPersistDescription _Buffer;

	_Ty_Content::iterator it = m_Contents.begin();
	_Ty_Content::iterator ite = m_Contents.end();

	for(; it != ite && _lRes <= m_MaxPersistLength; ++it)
	{
		_Buffer.m_Description = it->second;
		_Buffer.CheckSummat();

		if((_Walk + sizeof(_Buffer)) > _Walk_End)
		{
			::fwrite(_DualPage, 1, _Walk - _DualPage, _fp);
			_Walk = _DualPage;
		}

		memcpy(_Walk, &_Buffer, sizeof(_Buffer));
		_Walk += sizeof(_Buffer);
		++_lRes;
	}

	if(_Walk > _DualPage && _Walk < _Walk_End)
	{
		::fwrite(_DualPage, 1, _Walk - _DualPage, _fp);
		_Walk = _DualPage;
	}

	::fclose(_fp);
	return _lRes;
}

LRESULT DTAssistGraphBook::WritePersistText_i()
{
	LRESULT _lRes = 0;
	FILE* _fp = ::fopen(m_Dictionary_FilePath, "wb");
	if(!_fp)
		return 0;

	char _DualPage[ 8192 ];
	char* _Walk = _DualPage;
	char* _Walk_End = _DualPage + sizeof(_DualPage);

	_Ty_Dictionary::iterator it = m_Dictionary.begin();
	_Ty_Dictionary::iterator ite = m_Dictionary.end();

	for(; it != ite && _lRes <= m_MaxPersistLength; ++it)
	{
		if(!it->second || m_Contents.find(it->first) == m_Contents.end())
			continue;

		DTAssistGraphPersistText _Buffer;
		_Buffer.m_Identity = it->first;
		memcpy(_Buffer.m_Text, it->second, min(sizeof(_Buffer.m_Text), strlen(it->second)));
		_Buffer.Revise();
		_Buffer.CheckSummat();

		if((_Walk + sizeof(_Buffer)) > _Walk_End)
		{
			::fwrite(_DualPage, 1, _Walk - _DualPage, _fp);
			_Walk = _DualPage;
		}

		memcpy(_Walk, &_Buffer, sizeof(_Buffer));
		_Walk += sizeof(_Buffer);
		++_lRes;
	}

	if(_Walk > _DualPage && _Walk < _Walk_End)
	{
		::fwrite(_DualPage, 1, _Walk - _DualPage, _fp);
		_Walk = _DualPage;
	}

	::fclose(_fp);
	return _lRes;
}

LRESULT DTAssistGraphBook::WritePersistDescription()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	return WritePersistDescription_i();
}

LRESULT DTAssistGraphBook::WritePersistText()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	return WritePersistText_i();
}

LRESULT DTAssistGraphBook::WriteToPersist_i()
{
	WritePersistText_i();
	WritePersistDescription_i();
	return 0;
}

LRESULT DTAssistGraphBook::WriteToPersist()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	return WriteToPersist_i();
}

DTAssistGraphBook::_Ty_Text DTAssistGraphBook::AllocateFixedText(_Ty_Text _Text)
{
	_Ty_Text _Ret = new char[ m_TextFixedLength ];
	memset(_Ret, 0, m_TextFixedLength);
	if(_Text)
	{
		memcpy(_Ret, _Text, min(strlen(_Text), m_TextFixedLength));
	}
	return _Ret;
}

void DTAssistGraphBook::DeallocateFixedText(_Ty_Text _Text)
{
	delete [] _Text;
}

void DTAssistGraphBook::ReleaseDictionary_i()
{
	_Ty_Dictionary::iterator it = m_Dictionary.begin();
	_Ty_Dictionary::iterator ite = m_Dictionary.end();

	for(; it != ite; ++it)
	{
		if(it->second)
		{
			DeallocateFixedText(it->second);
			it->second = NULL;
		}
	}

	m_Dictionary.clear();
}

void DTAssistGraphBook::ReleaseDictionaryItem_i(_Ty_Id& _Id)
{
	_Ty_Dictionary::iterator it = m_Dictionary.find(_Id);
	_Ty_Dictionary::iterator ite = m_Dictionary.end();

	if(it != ite)
	{
		if(it->second)
		{
			DeallocateFixedText(it->second);
			it->second = NULL;
		}
		m_Dictionary.erase(it);
	}
}

void DTAssistGraphBook::ReleaseDictionary()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	ReleaseDictionary_i();
}

void DTAssistGraphBook::ReleaseContents_i()
{
	m_Contents.clear();
}

void DTAssistGraphBook::ReleaseContentsItem_i(_Ty_Id& _Id)
{
	m_Contents.erase(_Id);
}

void DTAssistGraphBook::ReleaseContents()
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	ReleaseContents_i();
}

LRESULT DTAssistGraphBook::InsertGraph(_Ty_Id& _Id, _Ty_Desc& _Desc, _Ty_Text _Text)
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	m_Contents[ _Id ] = _Desc;

	_Ty_Text _Insert_Text = NULL;
	if(_Text)
	{
		_Ty_Dictionary::iterator it = m_Dictionary.find(_Id);
		_Ty_Dictionary::iterator ite = m_Dictionary.end();

		if(it != ite && it->second)
		{
			_Insert_Text = it->second;
		}
		else
		{
			_Insert_Text = AllocateFixedText();
		}

		if(_Insert_Text)
		{
			memset(_Insert_Text, 0, m_TextFixedLength);
			memcpy(_Insert_Text, _Text, min(strlen(_Text), m_TextFixedLength));
			m_Dictionary[ _Id ] = _Insert_Text;
			_Insert_Text = NULL;
		}
	}
	return 1;
}

LRESULT DTAssistGraphBook::RemoveGraph()
{
	LRESULT _lRes = m_Contents.size();
	ReleaseContents();
	ReleaseDictionary();
	return _lRes;
}

LRESULT DTAssistGraphBook::RemoveGraph(_Ty_Id& _Id)
{
	DTAssistGraphBookLocker _Lock_(&m_Lock);
	ReleaseContentsItem_i(_Id);
	ReleaseDictionaryItem_i(_Id);
	return 1;
}

LRESULT DTAssistGraphBook::RemoveGraphByStkCode(char (&_StkCode)[8])
{
	LRESULT _lRes = 0;
	DTIdentity _TargetId;
	memcpy(_TargetId.stkCode, _StkCode, sizeof(_StkCode));

	DTAssistGraphBookLocker _Lock_(&m_Lock);

	_Ty_Content::iterator it = m_Contents.lower_bound(_TargetId);
	_Ty_Content::iterator ite = m_Contents.end();

	for(; it != ite; )
	{
		if(memcmp(it->first.stkCode, _StkCode, sizeof(_StkCode)) != 0)
			break;
		m_Contents.erase(it++);		// it++;
		++_lRes;
	}

	return _lRes;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// : DTAssistGraphics
DTAssistGraphics::DTAssistGraphics()
: m_SpotViewCoord(this)
{
	m_ActiveGraph = NULL;
	m_ShadowGraph = NULL;
	m_NewGraph = NULL;
	m_CommitGraph = NULL;
	m_bEditing = false;
	m_PrevMode = ENUM_ASSIST_EDIT_MODE_NULL;
	m_CurMode = ENUM_ASSIST_EDIT_MODE_NULL;

	m_ToolSelected = ENUM_ASSIST_GRAPH_TYPE_NULL;

	m_FullViewBitmap = NULL;

	m_BgColor = 0;
	m_FgColor = 0;
	m_OverlapColor = RGB(0xC0, 0xC0, 0xC0);

	m_LastHitTest = ENUM_ASSIST_HITTEST_NULL;

	m_hcrsrTarget = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
	m_hcrsrMove = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));
	m_hcrsrAnchors = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_UPARROW));
	m_hcrsrNormal = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	m_hcrsrCreate = ::LoadCursor(DTApiGetAppInstance(), MAKEINTRESOURCE(IDC_POINTER_COPY));

	m_LabelFont = ::CreateFont(DTAbsEquation::m_LabelFontSize, 5, 0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET,
										OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY,
										VARIABLE_PITCH | FF_DONTCARE, _T("Arial"));
}

DTAssistGraphics::~DTAssistGraphics()
{
	FlushAssistGraph();
	if(m_FullViewBitmap)
	{
		::DeleteObject(m_FullViewBitmap);
		m_FullViewBitmap = NULL;
	}
	if(m_LabelFont)
	{
		::DeleteObject(m_LabelFont);
		m_LabelFont = NULL;
	}
}

void DTAssistGraphics::ClearEditMode()
{
	PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL);
	if(m_ActiveGraph)
		m_DiscardAssistGraph.push_back(m_ActiveGraph);
	if(m_CommitGraph)
		m_DiscardAssistGraph.push_back(m_CommitGraph);
	m_ActiveGraph = NULL;
	m_CommitGraph = NULL;
}

void DTAssistGraphics::SetCursor(ENUM_ASSIST_HITTEST _HitTest)
{
	switch(_HitTest)
	{
	case ENUM_ASSIST_HITTEST_NULL:
		::SetCursor(m_hcrsrNormal);
		break;
	case ENUM_ASSIST_HITTEST_TARGET:
		::SetCursor(m_hcrsrTarget);
		break;
	case ENUM_ASSIST_HITTEST_LOCATION_P1:
	case ENUM_ASSIST_HITTEST_LOCATION_P2:
	case ENUM_ASSIST_HITTEST_LOCATION_P3:
		::SetCursor(m_hcrsrAnchors);
		break;
	default:
		::SetCursor(m_hcrsrNormal);
		break;
	}
}

void DTAssistGraphics::CommitAssistGraph(DTAssistGraphBook& _Book, DTAssistGraphDescription& _Description, char* _Text)
{
	_Book.InsertGraph(_Description.m_Identity, _Description, _Text);
	PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL);
}

void DTAssistGraphics::CommitAssistGraph(DTAssistGraphBook& _Book)
{
	if(!m_CommitGraph)
		return;

	if(m_CommitGraph->IsAvaiable())
	{
		CommitAssistGraph(_Book, m_CommitGraph->Description(), m_CommitGraph->Text());
	}
	m_DiscardAssistGraph.push_back(m_CommitGraph);
	PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL);
	m_CommitGraph = NULL;
}

void DTAssistGraphics::CommitAssistGraph()
{
	CommitAssistGraph(DTAssistGraphBook::Instance());
}

void DTAssistGraphics::DiscardAssistGraph(DTAssistGraphBook& _Book, DTAssistGraphDescription& _Description)
{
	_Book.RemoveGraph(_Description.m_Identity);

	std::list<DTAssistGraph*>::iterator it = m_AssistGraph.begin();
	std::list<DTAssistGraph*>::iterator ite = m_AssistGraph.end();
	for(; it != ite; ++it)
	{
		if(*it != NULL && _Description.m_Identity == (*it)->Description().m_Identity)
		{
			m_DiscardAssistGraph.push_back(*it);
			m_AssistGraph.erase(it);
			break;
		}

	}
	if(m_ActiveGraph != NULL && _Description.m_Identity == m_ActiveGraph->Description().m_Identity)
	{
		m_DiscardAssistGraph.push_back(m_ActiveGraph);
		m_ActiveGraph = NULL;
		if(m_ShadowGraph != NULL)
		{
			m_DiscardAssistGraph.push_back(m_ShadowGraph);
			m_ShadowGraph = NULL;
		}
	}

}

void DTAssistGraphics::DiscardAssistGraph(DTAssistGraphBook& _Book)
{
	if(m_CommitGraph)
	{
		DiscardAssistGraph(_Book, m_CommitGraph->Description());
	}
}

void DTAssistGraphics::DiscardAssistGraph()
{
	DiscardAssistGraph(DTAssistGraphBook::Instance());
}

BOOL DTAssistGraphics::DiscardActiveGraph()
{
	if(m_CommitGraph)
		m_DiscardAssistGraph.push_back(m_CommitGraph);
	if(m_ActiveGraph)
		m_DiscardAssistGraph.push_back(m_ActiveGraph);
    m_CommitGraph = NULL;
	if(m_ShadowGraph)
		m_CommitGraph = m_ShadowGraph;
	m_ActiveGraph = NULL;
	m_ShadowGraph = NULL;

	if(!m_CommitGraph)
		return FALSE;

	DiscardAssistGraph();
	return TRUE;
}

LRESULT DTAssistGraphics::DiscardAllGraph()
{
	return DTAssistGraphBook::Instance().RemoveGraphByStkCode(m_StkCode);
}

int DTAssistGraphics::BuildAbsoluteCoordinate(tagKValue* _Vals, tagViewXAxisUnit* _Units, size_t _Size, size_t _IndexForm, size_t _IndexTo)
{
	//::OutputDebugStringA("DTAssistGraphics::BuildAbsoluteCoordinate()");
	for(std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.begin(), ite = m_ViewCoord.end();
		it != ite;
		++it)
	{
		delete it->second;
	}
	m_ViewCoord.clear();
	return m_AbsCoord.Build(_Vals, _Units, _Size, _IndexForm, _IndexTo);
}

int DTAssistGraphics::AddViewCoordinate(DWORD _ViewId, const tagDTRect& _ViewRect, float _yValTop, float _yValBottom)
{
	//::OutputDebugStringA("DTAssistGraphics::AddViewCoordinate()");
	if(m_AbsCoord.Size() <= 0)
		return -1;
	DTCoordinateTransform* _ViewCoord = new DTCoordinateTransform(this);
	_ViewCoord->Build(_ViewRect, _yValTop, _yValBottom);
	m_ViewCoord[_ViewId] = _ViewCoord;
	return 0;
}

bool DTAssistGraphics::IsSpotViewCoordValidate()
{
	std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(m_ActiveGraphId.nViewId);
	std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
	return it != ite && it->second
				&& it->second->ViewRect() == m_SpotViewCoord.ViewRect()
				&& it->second->AbsOrigin() == m_SpotViewCoord.AbsOrigin()
				&& it->second->ViewOrigin() == m_SpotViewCoord.ViewOrigin();
}

void DTAssistGraphics::CancelSelected()
{
	if(m_ActiveGraph)
		m_DiscardAssistGraph.push_back(m_ActiveGraph);
	if(m_ShadowGraph)
		m_DiscardAssistGraph.push_back(m_ShadowGraph);
	m_ActiveGraph = NULL;
	m_ShadowGraph = NULL;
	m_LastHitTest = ENUM_ASSIST_HITTEST_NULL;
	m_HitTestId.reset();
	m_ActiveGraphId.reset();
	m_CreateGraphId.reset();
	ResetEditMode();
}

void DTAssistGraphics::SetStkCode(DWORD _LMarketType, char* _LStkCode)
{
	DTHelper::ConvertLongStkCodeToShortStkCode(m_StkCode, _LMarketType, _LStkCode);
}

void DTAssistGraphics::EnterPreEditing(ENUM_ASSIST_GRAPH_TYPE _Type)
{
	m_ToolSelected = _Type;
	if(m_ToolSelected != ENUM_ASSIST_GRAPH_TYPE_NULL)
	{
		CancelSelected();
		PushEditMode(ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING);
	}
	else if(ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING == m_CurMode)
	{
		PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL);
	}
}

long DTAssistGraphics::HitTestViewId(tagDTPoint& _Point)
{
	std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.begin();
	std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
	for(; it != ite; ++it)
	{
		if(it->second && ::PtInRect(&it->second->ViewRect(), _Point))
		{
			return it->first;
		}
	}
	return -1;
}

int DTAssistGraphics::FlushAssistGraph()
{
	std::list<DTAssistGraph*>::iterator it = m_AssistGraph.begin();
	std::list<DTAssistGraph*>::iterator ite = m_AssistGraph.end();
	for(; it != ite; ++it)
	{
		ReleaseAssistGraphInstance(*it);
	}
	m_AssistGraph.clear();

	it = m_DiscardAssistGraph.begin();
	ite = m_DiscardAssistGraph.end();
	for(; it != ite; ++it)
	{
		ReleaseAssistGraphInstance(*it);
	}
	m_DiscardAssistGraph.clear();

	if(m_ActiveGraph)
	{
		ReleaseAssistGraphInstance(m_ActiveGraph);
		m_ActiveGraph = NULL;
	}

	if(m_ShadowGraph)
	{
		ReleaseAssistGraphInstance(m_ShadowGraph);
		m_ShadowGraph = NULL;
	}

	if(m_NewGraph)
	{
		ReleaseAssistGraphInstance(m_NewGraph);
		m_NewGraph = NULL;
	}
	if(m_CommitGraph)
	{
		ReleaseAssistGraphInstance(m_CommitGraph);
		m_CommitGraph = NULL;
	}
	return 0;
}

int DTAssistGraphics::ForgetFullViewBitmap()
{
	if(m_FullViewBitmap)
	{
		::DeleteObject(m_FullViewBitmap);
		m_FullViewBitmap = NULL;
	}
	return 0;
}

int DTAssistGraphics::LoadFormAssistGraphBook(DTAssistGraphBook& _Book, char (&stkCode)[8], DWORD nPeriod)
{
	DTIdentity _Id_LowBound;
	DTIdentity _Id_HighBound;
	memset(&_Id_LowBound, 0, sizeof(_Id_LowBound));
	memset(&_Id_HighBound, -1, sizeof(_Id_HighBound));
	memcpy(_Id_LowBound.stkCode, stkCode, sizeof(stkCode));
	memcpy(_Id_HighBound.stkCode, stkCode, sizeof(stkCode));
	_Id_LowBound.nPeriod = nPeriod;
	_Id_HighBound.nPeriod = nPeriod;

	DTAssistGraph* _ActiveGraph = NULL;
	DTAssistGraph* _ShadowGraph = NULL;

	if(!IsSpotViewCoordValidate())
	{
		if(!m_ActiveGraph
			|| !(memcmp(m_ActiveGraph->Description().m_Identity.stkCode, stkCode, sizeof(stkCode)) == 0)
			|| !(m_ActiveGraph->Description().m_Identity.nPeriod == nPeriod)
			|| !(m_ActiveGraph->Complete(), m_ActiveGraph->IsAvaiable()))
		{
			PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL);
		}
	}

	/*if(m_CurMode > ENUM_ASSIST_EDIT_MODE_BEGINNING && m_CurMode < ENUM_ASSIST_EDIT_MODE_END)
	{
		if(m_ActiveGraph != NULL)
		{
			if(memcmp(m_ActiveGraphId.stkCode, stkCode, sizeof(stkCode)) == 0
				&& m_ActiveGraphId.nPeriod == nPeriod
				&& m_ActiveGraph->Description().m_Identity == m_ActiveGraphId)
			{
				std::swap(_ActiveGraph, m_ActiveGraph);
				if(m_ShadowGraph != NULL && m_ShadowGraph->Description().m_Identity == m_ActiveGraphId)
				{
					std::swap(_ShadowGraph, m_ShadowGraph);
				}
			}
			else
			{
				PushEditMode(ENUM_ASSIST_EDIT_MODE_NULL);
			}
		}
		if(m_ShadowGraph != NULL && m_ShadowGraph->Description().m_Identity == m_ActiveGraphId)
		{
			std::swap(_ShadowGraph, m_ShadowGraph);
		}
	}*/
	if(m_CurMode > ENUM_ASSIST_EDIT_MODE_BEGINNING && m_CurMode < ENUM_ASSIST_EDIT_MODE_END)
	{
		std::swap(_ActiveGraph, m_ActiveGraph);
	}
	if(m_ShadowGraph
		&& memcmp(m_ShadowGraph->Description().m_Identity.stkCode, stkCode, sizeof(stkCode)) == 0
		&& m_ShadowGraph->Description().m_Identity.nPeriod == nPeriod)
	{
		std::swap(_ShadowGraph, m_ShadowGraph);
	}

	FlushAssistGraph();

	{
		DTAssistGraphBook::DTAssistGraphBookLocker _Lock_(&_Book.m_Lock);

		DTAssistGraphBook::_Ty_Content::iterator it = _Book.m_Contents.lower_bound(_Id_LowBound);
		DTAssistGraphBook::_Ty_Content::iterator ite = _Book.m_Contents.end();
		for(; it != ite; ++it)
		{
			if(it->first > _Id_HighBound)
				break;
			if(m_ViewCoord.find(it->first.nViewId) != m_ViewCoord.end())
			{
				DTAssistGraph* _Graph = NULL;
				_Graph = CreateAssistGraphInstance((ENUM_ASSIST_GRAPH_TYPE)it->first.nType, it->second);
				if(_Graph)
				{
					if(_Graph->Type() == ENUM_ASSIST_GRAPH_TYPE_Text)
					{
						DTAssistGraphBook::_Ty_Dictionary::iterator it = _Book.m_Dictionary.find(_Graph->Description().m_Identity);
						DTAssistGraphBook::_Ty_Dictionary::iterator ite = _Book.m_Dictionary.end();
						if(it != ite)
						{
							_Graph->CopyText(it->second);
						}
					}
					_Graph->BuildAccordToDescription();

					if(it->second.m_Identity == m_ActiveGraphId)
					{
						if(_ShadowGraph)
						{
							if(!ISDTOK(_ShadowGraph->Clone(*_Graph)))
							{
								m_DiscardAssistGraph.push_back(_ShadowGraph);
								_ShadowGraph = CreateAssistGraphInstance(_Graph);
							}
							std::swap(_ShadowGraph, m_ShadowGraph);
						}
						if(m_ShadowGraph && _ActiveGraph)
						{
							if(_ActiveGraph->Description().m_Identity == m_ActiveGraphId)
							{
								std::swap(_ActiveGraph, m_ActiveGraph);
							}
						}
					}
					m_AssistGraph.push_back(_Graph);
					_Graph = NULL;
				}
			}
		}
	}

	if(_ActiveGraph)
	{
		if(_ActiveGraph->Description().m_Identity == m_CreateGraphId
			&& !m_ActiveGraph)
		{
			std::swap(_ActiveGraph, m_ActiveGraph);
		}
		else
		{
			m_DiscardAssistGraph.push_back(_ActiveGraph);
		}
	}
	if(_ShadowGraph)
	{
		if(_ShadowGraph->Description().m_Identity == m_CreateGraphId
			&& !m_ShadowGraph)
		{
			std::swap(_ShadowGraph, m_ShadowGraph);
		}
		else
		{
			m_DiscardAssistGraph.push_back(_ShadowGraph);
		}
	}
	_ActiveGraph = NULL;
	_ShadowGraph = NULL;

	return 0;
}

int DTAssistGraphics::LoadFormAssistGraphBook(char (&stkCode)[8], DWORD nPeriod)
{
	return LoadFormAssistGraphBook(DTAssistGraphBook::Instance(), stkCode, nPeriod);
}

int DTAssistGraphics::LoadCoordinateTransform(DTCoordinateTransform& _Transform, DWORD _ViewId)
{
	std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(_ViewId);
	std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
	if(it != ite)
	{
		_Transform = *(it->second);
	}
	return 0;
}

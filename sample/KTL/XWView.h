/**
MIT License
Copyright (c) 2015 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
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

// XWView.h : interface of the CXWView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlmisc.h>
#include "XW/include/CALayer.h"
#include "XW/include/CATextLayer.h"
#include "DrawTool/DrawTool.h"
#include <vector>
#include <fstream>
#include <sstream>

class CAKLineChartCursorLayer;
class CAKLineChartLayer;
class CAKLineCoordLayer;
class CAKLineLayer; /** <!-- front:CAKLineChartCursorLayer second:CAKLineCoordLayer back:CAKLineChartLayer --> */
class CAKLineInfoLabelLayer;
class CAKLineInfoLayer; /** <!-- vertiallayout:CAKLineInfoLabelLayer --> */
class CAKLineBoxLayer; /** <!-- left:CAKLineLayer right:CAKLineInfoLayer --> */

struct CRawK
{
    int _date;
    int _open;
    int _high;
    int _low;
    int _close;
    int _vol;
    int _amnt;
    int _reserved;
};

class CKAdapter
{
public:
    std::vector<CRawK> _ks;
    CKAdapter()
    {
        load();
        _zoom = .1;
    }
    void load()
    {
        std::ifstream ifs("sh000001.day", std::ios::ios_base::in| std::ios::ios_base::binary);
        if(ifs.is_open())
        {
            std::stringstream ss;
            ss << ifs.rdbuf();
            std::string str;
            str = ss.str();
            int i = 0;
            while(i + sizeof(CRawK) <= str.length())
            {
                _ks.push_back(*(CRawK*)(str.c_str()+i));
                i += sizeof(CRawK);
            }
        }
        _index = 0;
        _count = _ks.size();
        _high = 0;
        _low = 0;
        std::pair<std::pair<int, int>, bool> res = getPriceRange(0, _count);
        if (res.second)
        {
            _high = res.first.first;
            _low = res.first.second;
        }
    }
    std::pair<std::pair<int, int>, bool> getPriceRange(int index, int count = -1)
    {
        int end = _ks.size();
        if (index >= end)
        {
            return std::make_pair<std::pair<int, int>, bool>(std::make_pair<int, int>(0, 0), false);
        }
        if (count > 0)
        {
            end = min(index+count, end);
        }
        int high = _ks[index]._high;
        int low = _ks[index]._low;
        for (int i = index + 1; i < end; ++i)
        {
            high = max(_ks[i]._high, high);
            low = min(_ks[i]._low, low);
        }

        return std::make_pair<std::pair<int, int>, bool>(std::make_pair<int, int>(high, low), true);
    }
    int _index;
    int _count;
    int _high;
    int _low;
    float _zoom;
};

class CAKLineChartCursorLayerListener
{
public:
    virtual void update(CRawK& k) = 0;
};

class CAKLineChartCursorLayer : public CALayer
{
public:
    CKAdapter& _kadapter;
    int _X, _Y;
    CATextLayer* _date;
    CATextLayer* _price;
    CAKLineChartCursorLayerListener* _listener;
    ~CAKLineChartCursorLayer()
    {
        _date->release();
        _price->release();
    }
    CAKLineChartCursorLayer(CKAdapter& kadapter) : _kadapter(kadapter)
    {
        _listener = NULL;
        addSublayer(_date = NSObject::allocT<CATextLayer>());
        addSublayer(_price = NSObject::allocT<CATextLayer>());
        _date->setHidden(True);
        _price->setHidden(True);
        _date->setForegroundColor(CGColor(CGColor::Pink));
        _price->setForegroundColor(CGColor(CGColor::Pink));
        _date->set_backgroundColor(CGColor(kClearColor));
        _price->set_backgroundColor(CGColor(kClearColor));
        _date->set_borderColor(CGColor(kClearColor));
        _price->set_borderColor(CGColor(kClearColor));
        _date->setPreferBackingStore(False);
        _price->setPreferBackingStore(False);
    }
    void setListener(CAKLineChartCursorLayerListener* listener)
    {
        _listener = listener;
    }
    int getSafeIndex(int x, int y)
    {
        CGPoint pt = convertCursor(x, y);
        double xunit = _kadapter._zoom;
        int index = max(0, min(_X / xunit, _kadapter._count)) + _kadapter._index;
        index = min(index, _kadapter._ks.size());
        return index;
    }
    void setCursor(int x, int y)
    {
        CGPoint pt = convertCursor(x, y);
        _X = pt.X;
        _Y = pt.Y;
    }
    CGPoint convertCursor(int x, int y)
    {
        int htx = x;
        int hty = y;
        for(CALayer* super = this; super != nilObject; super = super->superLayer())
        {
            CGRect frame = super->frame();
            htx -= frame.X;
            hty -= frame.Y;
        }
        return CGPoint(htx, hty);
    }
    CGRect convertHostRect()
    {
        int htx = 0;
        int hty = 0;
        CGRect rc = self.frame();
        for(CALayer* super = this; super != nilObject; super = super->superLayer())
        {
            CGRect frame = super->frame();
            htx += frame.X;
            hty += frame.Y;
        }
        rc.X = htx;
        rc.Y = hty;
        return rc;
    }
    bool hitTestHost(int x, int y)
    {
        CGPoint pt = convertCursor(x, y);
        CGRect rc = self.bounds();
        return rc.Contains(pt.X, pt.Y);
    }
    float getXRatio(int x)
    {
        CGPoint pt = convertCursor(x, 0);
        CGRect rc = self.bounds();
        return pt.X / rc.Width;
    }
    void drawInContext(CGContext* ctx)
    {
        CGRect rc = self.bounds();
        if (!rc.Contains(_X, _Y))
        {
            _date->setHidden(True);
            _price->setHidden(True);
            return;
        }

        double xunit = _kadapter._zoom;
        double yunit = rc.Height / (_kadapter._high - _kadapter._low);
        int index = max(0, min(_X / xunit, _kadapter._count)) + _kadapter._index;
        if (index < _kadapter._ks.size())
        {
            _date->setHidden(False);
            _price->setHidden(False);
            CRawK& k = _kadapter._ks[index];
            NSString* fmt = NSString::stringWithCString("%d");
            std::stringstream ssdate;
            ssdate << k._date;
            _date->setString(NSString::stringWithCString(ssdate.str().c_str(), NSASCIIStringEncoding));
            std::stringstream ssprice;
            ssprice << _kadapter._high - int(_Y / yunit);
            _price->setString(NSString::stringWithCString(ssprice.str().c_str(), NSASCIIStringEncoding));
            CGRect rcdate(_X, 0., rc.Width, 20.);
            CGRect rcprice(0., _Y, rc.Width, 20.);
            if (_X < rc.Width/2)
            {
                _date->setAlignmentMode(CAAlignmentMode::kCAAlignmentLeft);
                _price->setAlignmentMode(CAAlignmentMode::kCAAlignmentRight);
            }
            else
            {
                rcdate.X = 0.; rcdate.Width = _X;
                _date->setAlignmentMode(CAAlignmentMode::kCAAlignmentRight);
                _price->setAlignmentMode(CAAlignmentMode::kCAAlignmentLeft);
            }
            if (_Y > rc.Height/2)
            {
                rcprice.Y -= rcprice.Height;
            }
            _date->setFrame(rcdate);
            _price->setFrame(rcprice);

            if (_listener)
                _listener->update(k);
        }

        ctx->setStrokeColor(CGColor(CGColor::Blue));
        CGContextBeginPath(ctx);
        CGContextMoveToPoint(ctx, rc.X, _Y);
        CGContextAddLineToPoint(ctx, rc.Width, _Y);
        CGContextStrokePath(ctx);
        CGContextMoveToPoint(ctx, _X, rc.Y);
        CGContextAddLineToPoint(ctx, _X, rc.Height);
        CGContextStrokePath(ctx);
    }
};

class CAKLineChartLayerListener
{
public:
    virtual void update() = 0;
};

class CAKLineChartLayer : public CALayer
{
public:
    CKAdapter& _kadapter;
    ::Gdiplus::Bitmap* _backingStore;
    CBitmap m_bmp;
    bool _displayNeeded;
    CAKLineChartLayerListener* _listener;
    ~CAKLineChartLayer()
    {
        if (_backingStore)
            delete _backingStore;
    }
    CAKLineChartLayer(CKAdapter& kadapter) : _kadapter(kadapter), _backingStore(NULL)
    {
        _listener = NULL;
        _displayNeeded = true;
    }
    void setDisplayNeeded()
    {
        _displayNeeded = true;
    }
    void makeBackingStore(CGContext* ctx)
    {
        using namespace ::Gdiplus;
        bool need = !_backingStore;
        CGRect rc = bounds();
#if 0
        if (_backingStore)
        {
            need =
            rc.Width > _backingStore->GetWidth()
                || rc.Height > _backingStore->GetHeight();
        }
#else
        if (m_bmp)
        {
            SIZE csz;
            m_bmp.GetSize(csz);
            need =
            rc.Width > csz.cx
                || rc.Height > csz.cy;
        }
#endif
        need |= _displayNeeded;
        if (need)
        {
#if 0
            if (_backingStore)
                delete _backingStore;

            _backingStore = new Bitmap(rc.Width, rc.Height, PixelFormat32bppRGB);
            CDCT<false> dcCraft = (HDC)((CGContextState*)ctx)->_hdcCraft;
            HBITMAP sel; _backingStore->GetHBITMAP(CGColor(CGColor::Red), &sel);
            HBITMAP old = dcCraft.SelectBitmap(sel);
#else
            if (m_bmp)
                m_bmp.DeleteObject();
            CDCT<false> dcCraft = (HDC)((CGContextState*)ctx)->_hdcCraft;
            m_bmp.CreateCompatibleBitmap((HDC)((CGContextState*)ctx)->_hdcPaint, rc.Width, rc.Height);
            HBITMAP old = dcCraft.SelectBitmap((HBITMAP)m_bmp);
#endif
            CGContext bmpctx((HDC)dcCraft, (HDC)dcCraft, NULL, bounds());
            _drawInContext(&bmpctx);

            dcCraft.SelectBitmap(old);

            if (_listener)
                _listener->update();
            _displayNeeded = false;
        }
    }
    void selectBest()
    {
        CGRect rc = self.bounds();
        int maxcount = (int)(rc.Width / _kadapter._zoom);
        if (maxcount > _kadapter._ks.size())
        {
            _kadapter._zoom = rc.Width / (float)_kadapter._ks.size();
            _kadapter._index = 0;
            _kadapter._count = _kadapter._ks.size();
        }

    }
    void selectBest(int index, float zoom, float xratio)
    {
        CGRect rc = self.bounds();
        int maxcount = (int)(rc.Width / zoom);
        if (maxcount > _kadapter._ks.size())
        {
            _kadapter._zoom = rc.Width / (float)_kadapter._ks.size();
            _kadapter._index = 0;
            _kadapter._count = _kadapter._ks.size();
            return;
        }
        if (xratio < 0.)
        {
            if (maxcount / 2 > index)
            {
                _kadapter._zoom = zoom;
                _kadapter._index = 0;
                _kadapter._count = maxcount;
                return;
            }

            if (maxcount / 2 + index >= _kadapter._ks.size())
            {
                _kadapter._zoom = zoom;
                _kadapter._index = max(0, _kadapter._ks.size() - maxcount);
                _kadapter._count = maxcount;
                return;
            }
        }
        else if (xratio <= 1.)
        {
            if (index < maxcount * xratio)
            {
                _kadapter._zoom = zoom;
                _kadapter._index = 0;
                _kadapter._count = maxcount;
                return;
            }
            else if (index + maxcount * (1 - xratio) >= _kadapter._ks.size())
            {
                _kadapter._zoom = zoom;
                _kadapter._index = max(0, _kadapter._ks.size() - maxcount);
                _kadapter._count = maxcount;
                return;
            }
            else
            {
                _kadapter._zoom = zoom;
                _kadapter._index = max(0, index - maxcount * xratio);
                _kadapter._count = maxcount;
            }
        }


        _kadapter._zoom = zoom;
        _kadapter._index = max(0, index - maxcount / 2);
        _kadapter._count = maxcount;
    }
    void drawInContext(CGContext* ctx)
    {
        CGRect rc = self.bounds();
        makeBackingStore(ctx);
#if 0
        ::Gdiplus::Graphics& g = (::Gdiplus::Graphics&)*ctx;
        g.DrawImage(_backingStore, bounds());
#else
    #if 0
        CDCT<false> dcCraft = (HDC)((CGContextState*)ctx)->_hdcCraft;
            HBITMAP old = dcCraft.SelectBitmap((HBITMAP)m_bmp);
            ::BitBlt((HDC)((CGContextState*)ctx)->_hdcMem, ::GetSystemMetrics(SM_CXDLGFRAME), ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE), rc.Width, rc.Height,
                     (HDC)dcCraft, 0, 0, SRCCOPY);

            dcCraft.SelectBitmap(old);
    #else
        ::Gdiplus::Bitmap gpbmp((HBITMAP)m_bmp, NULL);
        ::Gdiplus::Graphics& g = (::Gdiplus::Graphics&)*ctx;
        g.DrawImage(&gpbmp, 0, 0);
    #endif
#endif
    }
    void _drawInContext(CGContext* ctx)
    {
        CGRect rc = self.bounds();
        float xunit;
        double yunit;
        float ybase;
        xunit = _kadapter._zoom;
        std::pair<std::pair<int, int>, bool> rs = _kadapter.getPriceRange(_kadapter._index, _kadapter._count);
        if (rs.second == false)
            return;

        _kadapter._high = rs.first.first;
        _kadapter._low = rs.first.second;
        yunit = rc.Height / (rs.first.first - rs.first.second) ;
        ybase = rs.first.first * yunit;
        for(int i = 0; i < _kadapter._count && i + _kadapter._index < _kadapter._ks.size(); ++i)
        {
            CRawK& k = _kadapter._ks[i + _kadapter._index];
            if(k._open < k._close)
            {
                ctx->setStrokeColor(CGColor(CGColor::Red));
                ctx->setFillColor(CGColor(CGColor::Red));
            }
            else
            {
                ctx->setStrokeColor(CGColor(CGColor::Green));
                ctx->setFillColor(CGColor(CGColor::Green));
            }
            CGRect rcstk(i*xunit, ybase - max(k._close, k._open)*yunit, xunit, abs(k._open - k._close)*yunit);
            CGPoint ptstk[2] = {CGPoint(rcstk.X+xunit/2, ybase-k._high * yunit), CGPoint(rcstk.X+xunit/2, ybase-k._low * yunit)};
            //CGPoint ptstk2[2] = {CGPoint(rcstk.X-xunit/2, ybase-k._open * yunit), CGPoint(rcstk.X-xunit/2, ybase-k._close * yunit)};

            CGContextBeginPath(ctx);
            CGContextMoveToPoint(ctx, ptstk[0].X, ptstk[0].Y);
            CGContextAddLineToPoint(ctx, ptstk[1].X, ptstk[1].Y);
            CGContextStrokePath(ctx);

            CGContextAddRect(ctx, rcstk);
            CGContextFillPath(ctx);
        }
    }
};

class CAKLineCoordLayer : public CALayer
{
public:
    CKAdapter& _kadapter;
    CAKLineCoordLayer(CKAdapter& kadapter) : _kadapter(kadapter) {}
    void drawInContext(CGContext* ctx)
    {
        using namespace ::Gdiplus;
        CGRect rc = self.bounds();
        ctx->setStrokeColor(CGColor(CGColor::Pink));
        DashStyle ds = ((::Gdiplus::Pen&)*ctx).GetDashStyle();
        ((Pen&)*ctx).SetDashStyle(DashStyle::DashStyleDash);
        CGContextBeginPath(ctx);
        CGContextMoveToPoint(ctx, 0., rc.Height/2);
        CGContextAddLineToPoint(ctx, rc.Width, rc.Height/2);
        CGContextStrokePath(ctx);
        CGContextMoveToPoint(ctx, 0., rc.Height/4);
        CGContextAddLineToPoint(ctx, rc.Width, rc.Height/4);
        CGContextStrokePath(ctx);
        CGContextMoveToPoint(ctx, 0., rc.Height/4 * 3);
        CGContextAddLineToPoint(ctx, rc.Width, rc.Height/4 * 3);
        CGContextStrokePath(ctx);
        ((::Gdiplus::Pen&)*ctx).SetDashStyle(ds);
    }
};

class CAKLineLayer : public CALayer
{
public:
    CKAdapter _kadapter;
    ~CAKLineLayer()
    {
        _chart->release();
        _coord->release();
    }
    CAKLineLayer()
    {
        addSublayer(_chart = NSObject::allocT1<CAKLineChartLayer, CKAdapter&>(_kadapter));
        addSublayer(_coord = NSObject::allocT1<CAKLineCoordLayer, CKAdapter&>(_kadapter));
        addSublayer(_cursor = NSObject::allocT1<CAKLineChartCursorLayer, CKAdapter&>(_kadapter));
        set_backgroundColor(CGColor(CGColor::Black));
        set_borderColor(CGColor(CGColor::Pink));
        set_borderWidth(2.);
        set_smoothBorderWidth(4.);
        set_cornerRadius(9.);
        _coord->set_backgroundColor(CGColor(kClearColor));
        _chart->set_backgroundColor(CGColor(kClearColor));
        _cursor->set_backgroundColor(CGColor(kClearColor));
        //_cursor->setHidden(True);
    }
    virtual void layoutSublayers()
    {
        CGRect rc = self.bounds();
        rc.Inflate(-2., -2.);
        _chart->setFrame(rc);
        _coord->setFrame(rc);
        _cursor->setFrame(rc);
        _chart->selectBest();
    }
    CAKLineChartLayer* _chart;
    CAKLineCoordLayer* _coord;
    CAKLineChartCursorLayer* _cursor;
};

class CAKLineInfoLabelLayer : public CATextLayer
{
public:
    ~CAKLineInfoLabelLayer()
    {

    }
    CAKLineInfoLabelLayer()
    {
        setPreferBackingStore(false);
        set_backgroundColor(CGColor(kClearColor));
        set_borderColor(CGColor(kClearColor));
        setForegroundColor(CGColor(CGColor::Blue));
        setFont(L"Arial", 20, Gdiplus::FontStyleRegular);
    }
};
class CAKLineInfoValueLayer : public CAKLineInfoLabelLayer
{
public:
    ~CAKLineInfoValueLayer()
    {

    }
    CAKLineInfoValueLayer()
    {
        setAlignmentMode(CAAlignmentMode::kCAAlignmentRight);
        setForegroundColor(CGColor(CGColor::Pink));
        setFont(L"Arial", 16, Gdiplus::FontStyleRegular);
    }
};

class CAKLineInfoLayer : public CALayer, public CAKLineChartCursorLayerListener
{
public:
    ~CAKLineInfoLayer()
    {
        _timelabel->release();
        _codelabel->release();
        _openlabel->release();
        _closelabel->release();
        _highlabel->release();
        _lowlabel->release();
        _volumelabel->release();
        _amountlabel->release();
        _timevalue->release();
        _codevalue->release();
        _openvalue->release();
        _closevalue->release();
        _highvalue->release();
        _lowvalue->release();
        _volumevalue->release();
        _amountvalue->release();
    }
    CAKLineInfoLayer()
    {
        addSublayer(_timelabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_codelabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_openlabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_closelabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_highlabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_lowlabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_volumelabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_amountlabel = NSObject::allocT<CAKLineInfoLabelLayer>());
        addSublayer(_timevalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_codevalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_openvalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_closevalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_highvalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_lowvalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_volumevalue = NSObject::allocT<CAKLineInfoValueLayer>());
        addSublayer(_amountvalue = NSObject::allocT<CAKLineInfoValueLayer>());
        _timelabel->setString(L"time:");
        _codelabel->setString(L"code:");
        _openlabel->setString(L"open:");
        _closelabel->setString(L"close:");
        _highlabel->setString(L"high:");
        _lowlabel->setString(L"low:");
        _volumelabel->setString(L"volume:");
        _amountlabel->setString(L"amount:");
        set_backgroundColor(CGColor(CGColor::Green));
        set_borderColor(CGColor(CGColor::Pink));
        set_borderWidth(2.);
        set_smoothBorderWidth(4.);
        set_cornerRadius(9.);
    }
    void update(CRawK& k)
    {
        struct lambda
        {
            static void set(CATextLayer* layer, int value)
            {
                std::stringstream ss;
                ss << value;
                layer->setString(NSString::stringWithCString(ss.str().c_str(), NSASCIIStringEncoding));
            }
        };
        lambda::set(_timevalue, k._date);
        lambda::set(_openvalue, k._open);
        lambda::set(_closevalue, k._close);
        lambda::set(_highvalue, k._high);
        lambda::set(_lowvalue, k._low);
        lambda::set(_volumevalue, k._vol);
        lambda::set(_amountvalue, k._amnt);
    }
    void layoutSublayers()
    {
        CGRect rc = self.bounds();
        CGRect urc = rc;
        urc.Height = 32.;
        _timelabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _timevalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _codelabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _codevalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _openlabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _openvalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _closelabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _closevalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _highlabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _highvalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _lowlabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _lowvalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _volumelabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _volumevalue->setFrame(urc); urc.Offset(0.f, urc.Height);
        _amountlabel->setFrame(urc); urc.Offset(0.f, urc.Height);
        _amountvalue->setFrame(urc); urc.Offset(0.f, urc.Height);
    }
    CAKLineInfoLabelLayer* _timelabel, *_timevalue;
    CAKLineInfoLabelLayer* _codelabel, *_codevalue;
    CAKLineInfoLabelLayer* _openlabel, *_openvalue;
    CAKLineInfoLabelLayer* _closelabel, *_closevalue;
    CAKLineInfoLabelLayer* _highlabel, *_highvalue;
    CAKLineInfoLabelLayer* _lowlabel, *_lowvalue;
    CAKLineInfoLabelLayer* _volumelabel, *_volumevalue;
    CAKLineInfoLabelLayer* _amountlabel, *_amountvalue;
};

class CAKLineBoxLayer : public CALayer
{
public:
    ~CAKLineBoxLayer()
    {
        _kline->release();
        _info->release();
    }
    CAKLineBoxLayer()
    {
        addSublayer(_kline = NSObject::allocT<CAKLineLayer>());
		addSublayer(_info = NSObject::allocT<CAKLineInfoLayer>());
		_kline->_cursor->setListener(_info);
		set_backgroundColor(CGColor(kClearColor));
    }
    CAKLineLayer* _kline;
    CAKLineInfoLayer* _info;
    void layoutSublayers()
    {
        CGRect rc = self.bounds();
        CGRect rrc = rc;
        rrc.Width = 150.;
        rrc.X = max(0., rc.Width-rrc.Width);
        CGRect lrc = rc;
        lrc.Width = max(rrc.Width, rrc.X);
        _kline->setFrame(lrc);
        _info->setFrame(rrc);
        _kline->layoutSublayers();
        _info->layoutSublayers();
    }
};

class CXWView : public CWindowImpl<CXWView>,
				public CDoubleBufferImpl<CXWView>,
				public CAKLineChartLayerListener
{
    CAKLineBoxLayer* _rootLayer;
    CALayer* _klineLayer;
    CALayer* _infoLayer;
    // Assist Editing Layer
    DTAssistGraphics* m_DTAssistGraphics;
    bool m_bDisableDrawTool;
    bool m_bMouseMoving;
    bool m_bCursorSelection;
    bool m_bCrossCursor;
    bool m_bMouseOverCursorBorder;
    bool m_bHasPositionChangedSinceLastMouseButtonAction;
    tagPOINT m_ptLastMouseButtonAction;
    tagPOINT m_ptLastMouseMove;
public:
	DECLARE_WND_CLASS(NULL)

	~CXWView()
	{
		// error, may out of the main, gdiplus.dll released.
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	void layout()
	{
	    CRect rc; GetClientRect(&rc);
		CGRect cgrc(rc.left, rc.top, rc.right, rc.bottom);
		cgrc.Offset(::GetSystemMetrics(SM_CXDLGFRAME),
              ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE));
		_rootLayer->setFrame(cgrc);
		_rootLayer->layoutSublayers();
	}

#define ID_DT_FLTBAR_EDIT 1234
#define ID_DT_FLTBAR_DELETE 1235
	BEGIN_MSG_MAP(CXWView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_MOVING, OnMoving)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWinPosChanging)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWinPosChanged)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_NCACTIVATE, OnNCActivate)
		MESSAGE_HANDLER(WM_NCPAINT, OnNCPaint)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_DT_FLTBAR_DELETE, OnChainMessage)
		MESSAGE_HANDLER(WM_DT_FLTBAR_DELETE_TARGET, OnChainMessage)
		MESSAGE_HANDLER(WM_DT_FLTBAR_MASK, OnChainMessage)
		MESSAGE_HANDLER(WM_DT_FLTBAR_EDIT_TARGET, OnChainMessage)
		COMMAND_ID_HANDLER(ID_DT_FLTBAR_EDIT, OnFloatBarEdit)
		COMMAND_ID_HANDLER(ID_DT_FLTBAR_DELETE, OnFloatBarDelete)
		//CHAIN_MSG_MAP(CDoubleBufferImpl<CXWView>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	    m_DTAssistGraphics = new DTAssistGraphics;
	    m_bMouseMoving = FALSE;
        m_bCursorSelection = FALSE;
        m_bCrossCursor = FALSE;
        m_bMouseOverCursorBorder = FALSE;
        m_bDisableDrawTool = FALSE;
	    _rootLayer = NSObject::allocT<CAKLineBoxLayer>();
        _klineLayer = (CALayer*)_rootLayer->_kline->retain();
        _infoLayer = (CALayer*)_rootLayer->_info->retain();
        _rootLayer->_kline->_chart->_listener = this;

        DTApiInsertPeriodNameToScript(0, "k", TRUE);

        return 0;   // no background painting needed
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//::buildActivateAnimations();

		return 0;   // no background painting needed
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1;   // no background painting needed
	}

    LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
	    layout();
		return 1;
	}

	LRESULT OnMoving(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect rc; GetWindowRect(&rc);
		rc.right -= rc.left, rc.bottom -= rc.top, rc.top = rc.left = 0;
		return 1;   // no background painting needed
	}


	int GetKType()
	{
	    return 0;
	}

	DWORD GetMarket()
	{
	    return 'ESHS';
	}

	const char* GetStockCode()
	{
	    return "000001";
	}

	void SetupAssistGraph()
    {
        size_t nNumOfKData = 0;
        tagKValue* pKValues = NULL;
        tagViewXAxisUnit* pUnits = NULL;

        m_DTAssistGraphics->ForgetFullViewBitmap();

        CKAdapter& kadapter = _rootLayer->_kline->_kadapter;
        std::vector<CRawK>& kdata = _rootLayer->_kline->_kadapter._ks;
        nNumOfKData = max(0, kdata.size());
        if(nNumOfKData > 0)
        {
            pKValues = new tagKValue[nNumOfKData];
            pUnits = new tagViewXAxisUnit[nNumOfKData];

            for(size_t i = 0; i < nNumOfKData; ++i)
            {
                pKValues[i].m_date = kdata[i]._date;
                pKValues[i].m_fOpen = kdata[i]._open;
                pKValues[i].m_fHigh = kdata[i]._high;
                pKValues[i].m_fLow = kdata[i]._low;
                pKValues[i].m_fClose = kdata[i]._close;

                pUnits[i].xLeft = ((int)i - kadapter._index) * kadapter._zoom + .5;
                pUnits[i].xTarget = pUnits[i].xLeft + kadapter._zoom / 2 + .5;
                pUnits[i].xRight = pUnits[i].xLeft + kadapter._zoom + .5;
            }
        }

        if(m_DTAssistGraphics->BuildAbsoluteCoordinate(pKValues, pUnits, nNumOfKData, kadapter._index, kadapter._index + kadapter._count) > 0)
        {
            CGRect rchost = _rootLayer->_kline->_cursor->convertHostRect();
            RECT rc = {rchost.X + ::GetSystemMetrics(SM_CXEDGE), rchost.Y,
            rchost.X + ::GetSystemMetrics(SM_CXEDGE) + rchost.Width, rchost.Y + rchost.Height};

            double eYMin = 0.0;
            double eYMax = 0.0;

            eYMin = kadapter._low;
            eYMax = kadapter._high;
            m_DTAssistGraphics->AddViewCoordinate(0, (RECT)rc, eYMax, eYMin);
        }

        char code[8] = {'S', 'H', '0', '0', '0', '0', '0', '1'};
        m_DTAssistGraphics->LoadFormAssistGraphBook(/**m_DTAssistGraphBook, */code, GetKType());

        m_DTAssistGraphics->ForeGroupColor(0xff4422);
        m_DTAssistGraphics->ShadowColor(0x22ff44);
        m_DTAssistGraphics->BackGroupColor(0x4422ff);

        if(pKValues)
            delete [] pKValues;
        if(pUnits)
            delete [] pUnits;
        pKValues = NULL;
        pUnits = NULL;
    }

    void update()
    {
        SetupAssistGraph();
    }
/// <!-- forward message to assist graphics layer -->
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
	    CRect rc; GetWindowRect(&rc);
		rc.right -= rc.left, rc.bottom -= rc.top, rc.top = rc.left = 0;
		int fwKeys = wParam;
        int xPos = LOWORD(lParam) + ::GetSystemMetrics(SM_CXDLGFRAME) + ::GetSystemMetrics(SM_CXEDGE);
        int yPos = HIWORD(lParam) + ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE) *2;
        _rootLayer->_kline->_cursor->setCursor(xPos, yPos);

        tagPOINT point = {xPos, yPos};
        // <!-- OnPreMouseMove
        if(m_bMouseMoving && m_ptLastMouseMove == point)
        {
            return 1;
        }
        m_bMouseMoving = TRUE;
        m_ptLastMouseMove = point;
        m_bHasPositionChangedSinceLastMouseButtonAction |= m_ptLastMouseButtonAction == point;
        // -->

        if(m_DTAssistGraphics->EditMode() <= ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING && !m_bDisableDrawTool)
        {
            m_DTAssistGraphics->EnterPreEditing(DTApiRetrievePickedGraphTool());
        }

        if(m_bDisableDrawTool || m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_NULL)
        {
            BOOL bHandleForward = FALSE;
            LRESULT nHitTest = ENUM_ASSIST_HITTEST_NULL;
            LRESULT lRes = ENUM_ASSIST_EDIT_MODE_NULL;
            if(!::DTApiIsSignalMaskGraphTool())
            {
                if(!m_bDisableDrawTool)
                {
                    int nViewIdSelected = m_DTAssistGraphics->HitTestViewId(point);
                    nHitTest = m_DTAssistGraphics->OnHitTest(GetKType(), nViewIdSelected, (POINT)point, bHandleForward);
                }
                if(!m_bDisableDrawTool)
                {
                    m_DTAssistGraphics->SetCursor((ENUM_ASSIST_HITTEST) nHitTest);
                }
            }
        }
        else
        {
            OnMouseMoveEditing(0, point);
        }

        InvalidateRect(rc);
        UpdateWindow();
		return 1;   // no background painting needed
	}


	LRESULT OnMouseMoveEditing(UINT nFlags, CPoint point)
    {
        BOOL bHandleForward = FALSE;
        LRESULT lRes = ENUM_ASSIST_EDIT_MODE_NULL;

        if(!::DTApiIsSignalMaskGraphTool())
        {
            lRes = m_DTAssistGraphics->OnMouseMove(0, GetKType(), (POINT)point, bHandleForward);
        }
        return 1;
    }

	LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CRect rc; GetWindowRect(&rc);
		rc.right -= rc.left, rc.bottom -= rc.top, rc.top = rc.left = 0;
		int fwKeys = LOWORD(wParam);
		int zDelta = (short)HIWORD((int)wParam);
		CPoint pt(LOWORD(lParam), HIWORD(lParam));
		ScreenToClient(&pt);
        int xPos = pt.x + ::GetSystemMetrics(SM_CXDLGFRAME) + ::GetSystemMetrics(SM_CXEDGE);
        int yPos = pt.y + ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE) *2;
        if (!_rootLayer->_kline->_cursor->hitTestHost(xPos, yPos))
            return 1;
        //_rootLayer->_kline->_cursor->setCursor(xPos, yPos);
        float zoom = _rootLayer->_kline->_kadapter._zoom;
        if (zDelta > 0)
        {
            if (zoom < 1.)
                zoom = 1.;
            else
            {
                if (zoom < 20.)
                {
                    zoom += 2.;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            if (zoom < 1.)
            {
                if (zoom > .11)
                    zoom -= .1;
                else
                    return 1;
            }

            else
            {
                if (zoom <= 20.)
                {
                    zoom -= 2.;
                }
                else
                {
                    zoom = 20.;
                }
            }
        }
        zoom = max(zoom, .05);
        int index = _rootLayer->_kline->_cursor->getSafeIndex(xPos, yPos);
        float xratio = _rootLayer->_kline->_cursor->getXRatio(xPos);
        _rootLayer->_kline->_chart->selectBest(index, zoom, xratio);
        std::stringstream ss;
        CKAdapter& kadapter = _rootLayer->_kline->_kadapter;
        ss << "index: " << index << ", zoom: " << zoom << ", zDelta: " << zDelta << "\n"
            << kadapter._index << ", " << kadapter._count << ", " << kadapter._zoom;
        ::OutputDebugStringA(ss.str().c_str());
        _rootLayer->_kline->_chart->setDisplayNeeded();
        InvalidateRect(rc);
        UpdateWindow();
		return 1;   // no background painting needed
	}

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
	    int xPos = LOWORD(lParam) + ::GetSystemMetrics(SM_CXDLGFRAME) + ::GetSystemMetrics(SM_CXEDGE);
        int yPos = HIWORD(lParam) + ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE) *2;
        tagDTPoint pt = {xPos, yPos};
        BOOL bHasPositionChangedSinceLastMouseButtonAction = m_bHasPositionChangedSinceLastMouseButtonAction;
        m_bHasPositionChangedSinceLastMouseButtonAction = FALSE;
        m_ptLastMouseButtonAction = pt;
        m_bMouseMoving = FALSE;

        if(m_bDisableDrawTool || m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_NULL)
        {

        }
        else
        {
            bool bShiftPressed = wParam == MK_SHIFT;
            bool bCtrlPressed = wParam == MK_CONTROL;
            OnLButtonUpEditing(0, pt, bShiftPressed, bCtrlPressed);
            if(!::DTApiIsSignalMaskGraphTool())
            {
                Invalidate();
                UpdateWindow();
            }
        }
	    return 1;
	}

	void OnLButtonUpEditing(UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed)
    {
        BOOL bHandleForward = FALSE;
        LRESULT lRes = ENUM_ASSIST_EDIT_MODE_NULL;
        if(!::DTApiIsSignalMaskGraphTool())
        {
            lRes = m_DTAssistGraphics->OnLButtonUp(nFlags, GetKType(), (POINT)point, bShiftPressed, bCtrlPressed, bHandleForward);
            if(lRes == ENUM_ASSIST_EDIT_MODE_COMMIT)
            {
                m_DTAssistGraphics->CommitAssistGraph();
                m_DTAssistGraphics->SetCursor(ENUM_ASSIST_HITTEST_NULL);
                Invalidate();
                char code[8] = {'S', 'H', '0', '0', '0', '0', '0', '1'};
                m_DTAssistGraphics->LoadFormAssistGraphBook(/**m_DTAssistGraphBook, */code, GetKType());
            }
        }
    }

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
	    if (!DTApiIsShowFloatBar())
            DTApiShowFloatBar(NULL, TRUE, TRUE);
	    int xPos = LOWORD(lParam) + ::GetSystemMetrics(SM_CXDLGFRAME) + ::GetSystemMetrics(SM_CXEDGE);
        int yPos = HIWORD(lParam) + ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE) *2;
        tagDTPoint pt = {xPos, yPos};
        long nViewIdSelected = m_DTAssistGraphics->HitTestViewId(pt);
        m_DTAssistGraphics->SetStkCode('ESHS', "000001");
        if(m_DTAssistGraphics->EditMode() <= ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING && !m_bDisableDrawTool)
        {
            ENUM_ASSIST_GRAPH_TYPE nTypePicked = DTApiPickGraphTool();
            switch(nTypePicked)
            {
            case ENUM_ASSIST_GRAPH_TYPE_NULL:
                break;
            case ENUM_ASSIST_GRAPH_TYPE_LRA:
            case ENUM_ASSIST_GRAPH_TYPE_LREA:
            case ENUM_ASSIST_GRAPH_TYPE_LRL:
                if(nViewIdSelected > 0)
                {
                    return 1;
                }
            default:
                if(_rootLayer->_kline->_kadapter._zoom < 1.)
                {
                    ::MessageBox((HWND) *this, _T("当前倍数过低，鼠标滑轮放大后再画线。"), _T("zhelper"), MB_OKCANCEL | MB_ICONQUESTION | MB_TASKMODAL | MB_TOPMOST);
                    return 1;
                }
                if(nViewIdSelected < 0)
                {
                    return 1;
                }

                break;
            }
            m_DTAssistGraphics->EnterPreEditing(nTypePicked);
        }

        if(m_bDisableDrawTool || m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_NULL)
        {
            BOOL bHandleForward = FALSE;
            LRESULT nHitTest = ENUM_ASSIST_HITTEST_NULL;
            DWORD nViewIdSelected = m_DTAssistGraphics->HitTestViewId(pt);
            if(!::DTApiIsSignalMaskGraphTool())
            {
                if(!m_bDisableDrawTool)
                {
                    nHitTest = m_DTAssistGraphics->OnHitTest(GetKType(), nViewIdSelected, (POINT)pt, bHandleForward);
                }
                if(nHitTest != ENUM_ASSIST_HITTEST_NULL)
                {
                    m_DTAssistGraphics->SetCursor((ENUM_ASSIST_HITTEST) nHitTest);
                }
            }
        }
        //else

        {
            bool bShiftPressed = wParam == MK_SHIFT;
            bool bCtrlPressed = wParam == MK_CONTROL;
            OnLButtonDownEditing(0, pt, bShiftPressed, bCtrlPressed);
            if(!::DTApiIsSignalMaskGraphTool())
            {
                Invalidate();
                UpdateWindow();
            }
            return 1;
        }
        return 1;
	}

	void OnLButtonDownEditing(UINT nFlags, CPoint point, BOOL bShiftPressed, BOOL bCtrlPressed)
    {
        BOOL bHandleForward = FALSE;
        LRESULT lRes = ENUM_ASSIST_EDIT_MODE_NULL;

        if(!::DTApiIsSignalMaskGraphTool())
        {
            lRes = m_DTAssistGraphics->OnLButtonDown(nFlags, 0, point, bShiftPressed, bCtrlPressed, bHandleForward);
            if(lRes == ENUM_ASSIST_EDIT_MODE_COMMIT)
            {
                m_DTAssistGraphics->CommitAssistGraph();
                m_DTAssistGraphics->SetCursor(ENUM_ASSIST_HITTEST_NULL);
                Invalidate();
                char code[8] = {'S', 'H', '0', '0', '0', '0', '0', '1'};
                m_DTAssistGraphics->LoadFormAssistGraphBook(/**m_DTAssistGraphBook, */code, GetKType());
                return ;
            }
            else if(lRes == ENUM_ASSIST_EDIT_MODE_UNEXPECTED)
            {

            }
        }
    }

    LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
	    int xPos = LOWORD(lParam) + ::GetSystemMetrics(SM_CXDLGFRAME) + ::GetSystemMetrics(SM_CXEDGE);
        int yPos = HIWORD(lParam) + ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE) *2;
        tagDTPoint pt = {xPos, yPos};
        BOOL bHasPositionChangedSinceLastMouseButtonAction = m_bHasPositionChangedSinceLastMouseButtonAction;
        m_bHasPositionChangedSinceLastMouseButtonAction = FALSE;
        m_ptLastMouseButtonAction = pt;
        m_bMouseMoving = FALSE;

        CMenu menu;
        menu.CreatePopupMenu();

        BOOL bHandleForward = FALSE;
        LRESULT lRes = ENUM_ASSIST_EDIT_MODE_NULL;
        if(!m_bDisableDrawTool && !::DTApiIsSignalMaskGraphTool())
        {
            bool bShiftPressed = wParam == MK_SHIFT;
            bool bCtrlPressed = wParam == MK_CONTROL;
            lRes = m_DTAssistGraphics->OnRButtonUp(0, GetKType(), pt, bShiftPressed, bCtrlPressed, bHandleForward);
            if(m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_POPUPMENU)
            {
                m_DTAssistGraphics->SetCursor(ENUM_ASSIST_HITTEST_NULL);

                int	nIndexClick	= _rootLayer->_kline->_cursor->getSafeIndex(pt.x, pt.y);

                menu.AppendMenu(MF_STRING, ID_DT_FLTBAR_EDIT, _T("编辑画线"));
                menu.AppendMenu(MF_STRING, ID_DT_FLTBAR_DELETE, _T("删除画线"));
                ClientToScreen(&pt);
                menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON , pt.x, pt.y, (HWND)*this);
            }
        }
        return 1;
	}

	LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
	    int xPos = LOWORD(lParam) + ::GetSystemMetrics(SM_CXDLGFRAME) + ::GetSystemMetrics(SM_CXEDGE);
        int yPos = HIWORD(lParam) + ::GetSystemMetrics(SM_CYCAPTION) + ::GetSystemMetrics(SM_CYEDGE) *2;
        tagDTPoint pt = {xPos, yPos};
        BOOL bHasPositionChangedSinceLastMouseButtonAction = m_bHasPositionChangedSinceLastMouseButtonAction;
        m_bHasPositionChangedSinceLastMouseButtonAction = FALSE;
        m_ptLastMouseButtonAction = pt;
        m_bMouseMoving = FALSE;

        BOOL bHandleForward = FALSE;
        LRESULT lRes = ENUM_ASSIST_EDIT_MODE_NULL;

        if(!m_bDisableDrawTool && !::DTApiIsSignalMaskGraphTool())
        {
            bool bShiftPressed = wParam == MK_SHIFT;
            bool bCtrlPressed = wParam == MK_CONTROL;
            lRes = m_DTAssistGraphics->OnRButtonDown(0, GetKType(), pt, bShiftPressed, bCtrlPressed, bHandleForward);
            if(m_DTAssistGraphics->LastHitTest() != ENUM_ASSIST_GRAPH_TYPE_NULL)
            {

            }
        }
        return 1;
	}

	LRESULT OnFloatBarEdit(WORD /*id*/, WORD /*wParam*/, HWND /*lParam*/, BOOL& bHandled)
	{
	    return OnChainMessage(WM_DT_FLTBAR_EDIT_TARGET, 0, 0, bHandled);
	}

	LRESULT OnFloatBarDelete(WORD /*id*/, WORD /*wParam*/, HWND /*lParam*/, BOOL& bHandled)
	{
	    return OnChainMessage(WM_DT_FLTBAR_DELETE_TARGET, 0, 0, bHandled);
	}

	LRESULT OnChainMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        static BOOL __DoModal_MessageBox = FALSE;
        if(uMsg == WM_DT_FLTBAR_DELETE)
        {
            if(m_DTAssistGraphics->DiscardActiveGraph())
            {
                Invalidate();
                return TRUE;
            }
            else if(!__DoModal_MessageBox)
            {
                __DoModal_MessageBox = TRUE;
                if(::MessageBox((HWND) *this, _T("是否要删除当前所有画线？"), _T("zhelper"), MB_OKCANCEL | MB_ICONQUESTION | MB_TASKMODAL | MB_TOPMOST) == IDOK)
                {
                    m_DTAssistGraphics->SetStkCode('ESHS', "000001");
                    if(m_DTAssistGraphics->DiscardAllGraph() > 0)
                    {
                        Invalidate();
                    }
                }
                __DoModal_MessageBox = FALSE;
                return TRUE;
            }
        }
        else if(uMsg == WM_DT_FLTBAR_DELETE_TARGET)
        {
            if(m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_NULL
                || m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_POPUPMENU)
            {
                m_DTAssistGraphics->DiscardActiveGraph();
                Invalidate();
                return TRUE;
            }
        }
        else if(uMsg == WM_DT_FLTBAR_EDIT_TARGET)
        {
            LRESULT lRes = 0;
            lRes = m_DTAssistGraphics->OnSelectedEdit();
            if(ISDTOK(lRes) && m_DTAssistGraphics->EditMode() == ENUM_ASSIST_EDIT_MODE_COMMIT)
            {
                m_DTAssistGraphics->CommitAssistGraph();
                m_DTAssistGraphics->SetCursor(ENUM_ASSIST_HITTEST_NULL);
                Invalidate();
            }
        }
        else if(uMsg == WM_DT_FLTBAR_MASK)
        {
            Invalidate();
        }
        return FALSE;
    }
/// <!-- end forward message to assist graphics layer -->

	LRESULT OnWinPosChanging(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect rc; GetWindowRect(&rc);
		rc.right -= rc.left, rc.bottom -= rc.top, rc.top = rc.left = 0;
		return 1;   // no background painting needed
	}

	LRESULT OnWinPosChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		CRect rc; GetWindowRect(&rc);
		rc.right -= rc.left, rc.bottom -= rc.top, rc.top = rc.left = 0;
		WINDOWPOS* wps = (WINDOWPOS*)lParam;
		if(wps->flags & SWP_NOMOVE)
        {
            layout();
        }
		return 1;   // no background painting needed
	}

	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		UINT ht = DefWindowProc(WM_NCHITTEST, wParam, lParam);
		return 0;
	}

	LRESULT OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		UINT ht = DefWindowProc(uMsg, wParam, lParam);
		//_rootLayer->_kline->_cursor->setHidden(True);
		if(HTCAPTION == ht || HTREDUCE == ht || HTZOOM == ht || /*HTCLOSE == ht ||*/ HTSYSMENU == ht)
			ht = HTCAPTION;
        else if(HTCLIENT == ht)
        {
            POINT pt;
            ::GetCursorPos(&pt);
            ::ScreenToClient(m_hWnd, &pt);
            if (_rootLayer->_kline->_cursor->hitTestHost(pt.x + ::GetSystemMetrics(SM_CXDLGFRAME),
                                                         pt.y + ::GetSystemMetrics(SM_CYCAPTION)))
            {
                //_rootLayer->_kline->_cursor->setHidden(False);
            }
        }
        else if(HTCLOSE != ht)
            ht = HTNOWHERE;
		return ht;
	}

	LRESULT OnNCActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		////The DefWindowProc function draws the title bar or icon title in its active colors
		////////when the wParam parameter is TRUE and in its inactive colors when wParam is FALSE.

        /// side-effect, dialogs will not emit controls commands.
		return 1;
	}

	LRESULT OnNCPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CDC dc(::GetDCEx(m_hWnd, (HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN));
		return 0;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        // display layers
		{
            CPaintDC paintdc(m_hWnd);

            CWindowDC dc(m_hWnd);
            CRect rc; GetWindowRect(&rc);
            rc.right -= rc.left, rc.bottom -= rc.top, rc.top = rc.left = 0;
            CMemoryDC dcMem(dc.m_hDC, rc);

            CRect rcmap = rc; MapWindowPoints(0, (LPPOINT)&rcmap, sizeof(rc)/sizeof(POINT));
            CRect rc2; GetClientRect(&rc2);
            rcmap.OffsetRect(rc2.Width() - rc.Width() + 3, rc2.Height() - rc.Height() + 3);


            CDC dcscr(::GetDC(0));
            CDC dcCraft; dcCraft.CreateCompatibleDC(dcscr.m_hDC);

			CGRect cgrc(rc.left, rc.top, rc.Width(), rc.Height());
			CGContext ctx(dc, dcMem, dcCraft, cgrc);

			_rootLayer->display(&ctx);

			if(!::DTApiIsSignalMaskGraphTool())
            {
                BOOL bForward = FALSE;
                m_DTAssistGraphics->OnPaint((HDC)dcMem, bForward);
                m_DTAssistGraphics->OnDynamicLayerPaint((HDC)dcMem, bForward);
            }
		}

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		if((GetStyle() & (WS_CHILD | WS_POPUP)) == 0)
			::PostQuitMessage(1);
        if(_rootLayer)
            _rootLayer->release();
        _rootLayer = NULL;
        if(_infoLayer)
            _infoLayer->release();
        _infoLayer = NULL;
        if(_klineLayer)
            _klineLayer->release();
        _klineLayer = NULL;

        delete m_DTAssistGraphics;

		bHandled = FALSE;
		return 1;
	}

	void DoPaint(CDCHandle dcPaint)
	{
		int nContextState = dcPaint.SaveDC();



		dcPaint.RestoreDC( nContextState );
	}
};

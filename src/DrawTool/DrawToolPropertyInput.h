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

#ifndef _DRAWTOOL_PROPPERTY_INPUT__H_
#define _DRAWTOOL_PROPPERTY_INPUT__H_

#include "DrawToolTextInput.h"

class DTPropertyInputBox : public DTTextInputBox
{
public:
	DTPropertyInputBox();
	~DTPropertyInputBox();

	void SetDescriptionScript(DTAssistGraphDescriptionScript _Script);
	void SetDescription(DTAssistGraphDescription& _Description);
	void SetText(char* _Text);

	DTAssistGraphDescription& Description();

protected:
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnIgoreCheckBoxClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	BOOL m_Customer;
	DTAssistGraphDescriptionScript m_Script;
	DTAssistGraphDescription m_Description;
};

#endif // _DRAWTOOL_PROPPERTY_INPUT__H_

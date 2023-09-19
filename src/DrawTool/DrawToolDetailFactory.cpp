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
#include "DrawToolDetail.h"

DTAssistGraph* DTAssistGraphics::CreateAssistGraphInstance(ENUM_ASSIST_GRAPH_TYPE _type, const DTAssistGraphDescription& _descGraph)
{
	DTAssistGraph* _AssistGraph = NULL;
	switch(_type)
	{
	case ENUM_ASSIST_GRAPH_TYPE_Segment:
		_AssistGraph = (DTAssistGraph*) new DTLineSegmentGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Line:
		_AssistGraph = (DTAssistGraph*) new DTLineGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow:
		_AssistGraph = (DTAssistGraph*) new DTArrowSegmentGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Line_Extension:
		_AssistGraph = (DTAssistGraph*) new DTLineExtensionGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Price_Channels:
		_AssistGraph = (DTAssistGraph*) new DTPriceChannelsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines:
		_AssistGraph = (DTAssistGraph*) new DTParallelLinesGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Arc:
		_AssistGraph = (DTAssistGraph*) new DTEllipseArcGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Circle:
		_AssistGraph = (DTAssistGraph*) new DTCircleGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Circumcircle:
		_AssistGraph = (DTAssistGraph*) new DTCircumcircleGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GPHs:
		_AssistGraph = (DTAssistGraph*) new DTGoldenPriceHorizonsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GAHs:
		_AssistGraph = (DTAssistGraph*) new DTGoldenAimHorizonsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GSHs:
		_AssistGraph = (DTAssistGraph*) new DTGoldenSecionHorizonsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_PHs:
		_AssistGraph = (DTAssistGraph*) new DTPercentageHorizonsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_BHs:
		_AssistGraph = (DTAssistGraph*) new DTOctBandHorizonsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LRA:
		_AssistGraph = (DTAssistGraph*) new DTLinearRegressionAreaGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LREA:
		_AssistGraph = (DTAssistGraph*) new DTLinearRegressionExtensionAreaGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LRL:
		_AssistGraph = (DTAssistGraph*) new DTLinearRegressionLineGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Cycle:
		_AssistGraph = (DTAssistGraph*) new DTCycleVerticalsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Fibonacci:
		_AssistGraph = (DTAssistGraph*) new DTFibonacciVerticalsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gann:
		_AssistGraph = (DTAssistGraph*) new DTGannSequenceVerticalsGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gould_Rays:
		_AssistGraph = (DTAssistGraph*) new DTGouldRaysGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gann_Rays:
		_AssistGraph = (DTAssistGraph*) new DTGannRaysGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Rectangle:
		_AssistGraph = (DTAssistGraph*) new DTRectangleGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow:
		_AssistGraph = (DTAssistGraph*) new DTRedUpArrowGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow:
		_AssistGraph = (DTAssistGraph*) new DTGreenDownArrowGraph(this, _descGraph);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Text:
		_AssistGraph = (DTAssistGraph*) new DTTextGraph(this, _descGraph);
		break;
	default:
		break;
	}
	return _AssistGraph;
}

DTAssistGraph* DTAssistGraphics::CreateAssistGraphInstance(DTAssistGraph* _other)
{
	DTAssistGraph* _AssistGraph = NULL;
	switch(_other->Type())
	{
	case ENUM_ASSIST_GRAPH_TYPE_Segment:
		_AssistGraph = (DTAssistGraph*) new DTLineSegmentGraph(*(DTLineSegmentGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Line:
		_AssistGraph = (DTAssistGraph*) new DTLineGraph(*(DTLineGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow:
		_AssistGraph = (DTAssistGraph*) new DTArrowSegmentGraph(*(DTArrowSegmentGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Line_Extension:
		_AssistGraph = (DTAssistGraph*) new DTLineExtensionGraph(*(DTLineExtensionGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Price_Channels:
		_AssistGraph = (DTAssistGraph*) new DTPriceChannelsGraph(*(DTPriceChannelsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines:
		_AssistGraph = (DTAssistGraph*) new DTParallelLinesGraph(*(DTParallelLinesGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Arc:
		_AssistGraph = (DTAssistGraph*) new DTEllipseArcGraph(*(DTEllipseArcGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Circle:
		_AssistGraph = (DTAssistGraph*) new DTCircleGraph(*(DTCircleGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Circumcircle:
		_AssistGraph = (DTAssistGraph*) new DTCircumcircleGraph(*(DTCircumcircleGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GPHs:
		_AssistGraph = (DTAssistGraph*) new DTGoldenPriceHorizonsGraph(*(DTGoldenPriceHorizonsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GAHs:
		_AssistGraph = (DTAssistGraph*) new DTGoldenAimHorizonsGraph(*(DTGoldenAimHorizonsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GSHs:
		_AssistGraph = (DTAssistGraph*) new DTGoldenSecionHorizonsGraph(*(DTGoldenSecionHorizonsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_PHs:
		_AssistGraph = (DTAssistGraph*) new DTPercentageHorizonsGraph(*(DTPercentageHorizonsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_BHs:
		_AssistGraph = (DTAssistGraph*) new DTOctBandHorizonsGraph(*(DTOctBandHorizonsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LRA:
		_AssistGraph = (DTAssistGraph*) new DTLinearRegressionAreaGraph(*(DTLinearRegressionAreaGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LREA:
		_AssistGraph = (DTAssistGraph*) new DTLinearRegressionExtensionAreaGraph(*(DTLinearRegressionExtensionAreaGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LRL:
		_AssistGraph = (DTAssistGraph*) new DTLinearRegressionLineGraph(*(DTLinearRegressionLineGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Cycle:
		_AssistGraph = (DTAssistGraph*) new DTCycleVerticalsGraph(*(DTCycleVerticalsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Fibonacci:
		_AssistGraph = (DTAssistGraph*) new DTFibonacciVerticalsGraph(*(DTFibonacciVerticalsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gann:
		_AssistGraph = (DTAssistGraph*) new DTGannSequenceVerticalsGraph(*(DTGannSequenceVerticalsGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gould_Rays:
		_AssistGraph = (DTAssistGraph*) new DTGouldRaysGraph(*(DTGouldRaysGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gann_Rays:
		_AssistGraph = (DTAssistGraph*) new DTGannRaysGraph(*(DTGannRaysGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Rectangle:
		_AssistGraph = (DTAssistGraph*) new DTRectangleGraph(*(DTRectangleGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow:
		_AssistGraph = (DTAssistGraph*) new DTRedUpArrowGraph(*(DTRedUpArrowGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow:
		_AssistGraph = (DTAssistGraph*) new DTGreenDownArrowGraph(*(DTGreenDownArrowGraph*)_other);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Text:
		_AssistGraph = (DTAssistGraph*) new DTTextGraph(*(DTTextGraph*)_other);
		break;
	default:
		break;
	}
	return _AssistGraph;
}

void DTAssistGraphics::ReleaseAssistGraphInstance(DTAssistGraph* _p)
{
	delete (DTAssistGraphI*) _p;
}


ENUM_ASSIST_EDIT_MODE DTAssistGraphics::NextEditMode(ENUM_ASSIST_GRAPH_TYPE _Type, ENUM_ASSIST_EDIT_MODE _Mode)
{
	ENUM_ASSIST_EDIT_MODE _NextEditMode = ENUM_ASSIST_EDIT_MODE_NULL;
	switch(_Type)
	{
	case ENUM_ASSIST_GRAPH_TYPE_Segment:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTLineSegmentGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Line:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTLineGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTArrowSegmentGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Line_Extension:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTLineExtensionGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Price_Channels:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTPriceChannelsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTParallelLinesGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Arc:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTEllipseArcGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Circle:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTCircleGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Circumcircle:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTCircumcircleGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GPHs:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGoldenPriceHorizonsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GAHs:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGoldenAimHorizonsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_GSHs:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGoldenSecionHorizonsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_PHs:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTPercentageHorizonsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_BHs:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTOctBandHorizonsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LRA:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTLinearRegressionAreaGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LREA:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTLinearRegressionExtensionAreaGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_LRL:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTLinearRegressionLineGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Cycle:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTCycleVerticalsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Fibonacci:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTFibonacciVerticalsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gann:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGannSequenceVerticalsGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gould_Rays:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGouldRaysGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Gann_Rays:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGannRaysGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Rectangle:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTRectangleGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTRedUpArrowGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTGreenDownArrowGraph::_NextEditMode(_Mode);
		break;
	case ENUM_ASSIST_GRAPH_TYPE_Text:
		_NextEditMode = (ENUM_ASSIST_EDIT_MODE) DTTextGraph::_NextEditMode(_Mode);
		break;
	default:
		break;
	}
	return _NextEditMode;
}

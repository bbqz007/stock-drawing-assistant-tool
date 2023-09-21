![img](https://github.com/bbqz007/stock-drawing-assistant-tool/blob/master/res/demo1.gif)


* Overview
	* a Graphic Layer as portion of DrawTool, accepts editing interactions and renders charts.
	* with a floating ToolBar containing choices of editable charts.

* Purpose
	* a Graphic Layer can attach to any chart view with a coordinate about Values and Dates.
	* the attached chart view as the background layer and the hosted window.
	* a Graphic Layer manages any number of independed chart graphs as sub layers. it is a connector between the hosted window and the sub chart graphs.
	* a chart graph is abstracted as `DTAssistGraphisc` for extending.
	* a Equation, some like `x=0` is a line, `0<x<1 and y = 1` is a line segment, `x*x + y*y = r*r` is a cycle. a Equation alos respond to tranform coords. a chart graph can use Equations to render, move-to and hit-test.
	* edit, a chart graph has three anchor points for location and editting, this software code support a framework for editing a chart graph.

* Namings
	* files with prefix `DrawTool`
	* Classes with prefix `DT`
	* functions with prefix `DTApi`
	
* KeyRoles	
	* `DTAssistGraphiscs`, a layer, a connector between window and sub layers.
	* `DTAssistGraphisc`, a sub layer, act as a chart graph
	* `DTCoordinateTransform`, between screen x-y-coord and view date-value-coord.
	* `ENUM_ASSIST_EDIT_MODE`, state machine for layer edting.

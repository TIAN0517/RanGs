#pragma once

#include "UIEvent.h"

namespace GUIBase
{

// Event 처리가 가능한 클래스는 이 클래스를 구현해야 함;
class IUIEventable
{
public:
	/// Event 처리;
	virtual void OnSize( std::tr1::shared_ptr<CUIResizeEvent> resizeEvent ) { }
	virtual void OnFocus( std::tr1::shared_ptr<CUIFocusEvent> focusEvent ) { }
	virtual void OnShow( std::tr1::shared_ptr<CUIShowEvent> showEvent ) { }
	virtual void OnHide( std::tr1::shared_ptr<CUIShowEvent> hideEvent ) { }
	virtual bool OnKeyEvent( std::tr1::shared_ptr<CUIKeyEvent> keyEvent ) { return true; }
	virtual bool OnMouseEvent( std::tr1::shared_ptr<CUIMouseEvent> mouseEvent ) { return true; }
	virtual bool OnWheelEvent( std::tr1::shared_ptr<CUIWheelEvent> wheelEvent ) { return true; }
	virtual bool OnInvokeEvent( std::tr1::shared_ptr<CUIInvokeEvent> invokeEvent ) { return true; }
};

}
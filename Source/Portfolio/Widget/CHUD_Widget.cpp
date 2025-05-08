#include "../Widget/CHUD_Widget.h"

#include "CAssassinationTargetWidget.h"
#include "DefaultWidgetController.h"
#include "../Global.h"

void UCHUD_Widget::InitHudController(UDefaultWidgetController* Object)
{
	CheckNull(Object);
	
	WidgetController = Object;
	InitWidgetController();
}

void UCHUD_Widget::InitWidgetController_Implementation()
{
}
#include "CMenu.h"

bool UCMenu::Initialize()
{
	Super::Initialize();

	// WB_Menu -> How to get HostButton
	//for(Cast<UPannelWidget>(GetRootWidget())->GetAllChildren);	// 자식 1, 2, 3... 버튼타입인지 계속 캐스팅하면서 찾아야 함(뻘짓)

	return true;
}
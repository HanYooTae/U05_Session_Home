#include "CMenu.h"

bool UCMenu::Initialize()
{
	Super::Initialize();

	// WB_Menu -> How to get HostButton
	//for(Cast<UPannelWidget>(GetRootWidget())->GetAllChildren);	// �ڽ� 1, 2, 3... ��ưŸ������ ��� ĳ�����ϸ鼭 ã�ƾ� ��(����)

	return true;
}
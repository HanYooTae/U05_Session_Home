#include "CMenu.h"
#include "Components/Button.h"
#include "Global.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UCMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	CheckFalseResult(bSuccess, false);

	// WB_Menu -> How to get HostButton
	//for(Cast<UPannelWidget>(GetRootWidget())->GetAllChildren);	// �ڽ� 1, 2, 3... ��ưŸ������ ��� ĳ�����ϸ鼭 ã�ƾ� ��(����)

	//for(auto component : Cast<UPanelWidget>(GetRootWidget())->GetAllChildren())
	//{
	//	for (Cast<UPanelWidget>(component)->GetAllChildren())
	//	{
	//		for
	//			for
	//				for...
	//	}
	//}

	CheckNullResult(HostButton, false);
	HostButton->OnClicked.AddDynamic(this, &UCMenu::HostServer);

	CheckNullResult(JoinButton, false);
	JoinButton->OnClicked.AddDynamic(this, &UCMenu::OpenJoinMenu);

	CheckNullResult(CancelJoinMenuButton, false);
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UCMenu::OpenMainMenu);

	CheckNullResult(ConfirmJoinMenuButton, false);
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UCMenu::JoinServer);

	CheckNullResult(QuitButton, false);
	QuitButton->OnClicked.AddDynamic(this, &UCMenu::QuitGame);

	return true;
}

void UCMenu::HostServer()
{
	//GetOwner
	CheckNull(OwingGameInstance);
	OwingGameInstance->Host();

	CLog::Log("Host Button Pressed");
}

void UCMenu::JoinServer()
{
	CheckNull(OwingGameInstance);
	CheckNull(IPAddressField);

	const FString& address = IPAddressField->GetText().ToString();

	OwingGameInstance->Join(address);

	CLog::Log("Join Button Pressed");
}

void UCMenu::OpenJoinMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(JoinMenu);
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UCMenu::OpenMainMenu()
{
	CheckNull(MenuSwitcher);
	CheckNull(MainMenu);
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UCMenu::QuitGame()
{
	UWorld* world = GetWorld();
	CheckNull(world);
	
	APlayerController* controller = world->GetFirstPlayerController();
	
	controller->ConsoleCommand("Quit");
}

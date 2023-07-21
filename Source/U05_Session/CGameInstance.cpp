#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CMenuBase.h"
#include "Widgets/CMenu.h"
#include "OnlineSubsystem.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	CLog::Log("GameInstance::Constructor Called");

	CHelpers::GetClass(&MenuWidgetClass, "/Game/Widgets/WB_Menu");
	CHelpers::GetClass(&InGameWidgetClass, "/Game/Widgets/WB_InGame");
	//CLog::Log(MenuWidgetClass->GetName());
}

// BeginPlay
void UCGameInstance::Init()
{
	Super::Init();

	CLog::Log("GameInstance::Init Called");

	IOnlineSubsystem* oss = IOnlineSubsystem::Get();

	if (!!oss)
	{
		CLog::Log("OSS Name : " + oss->GetSubsystemName().ToString());

		IOnlineSessionPtr sessionInterface = oss->GetSessionInterface();

		//Nullüũ
		if (sessionInterface.IsValid())
		{

		}

		else
		{

		}
	}

	else
	{
		CLog::Log("OSS Not Found!!");
	}
}

void UCGameInstance::LoadMenu()
{
	CheckNull(MenuWidgetClass);

	Menu = CreateWidget<UCMenu>(this, MenuWidgetClass);
	CheckNull(Menu);

	Menu->SetOwingGameInstance(this);

	Menu->Attach();
}

void UCGameInstance::LoadInGameMenu()
{
	CheckNull(InGameWidgetClass);

	UCMenuBase* inGameWidget = CreateWidget<UCMenuBase>(this, InGameWidgetClass);
	CheckNull(inGameWidget);

	inGameWidget->SetOwingGameInstance(this);

	inGameWidget->Attach();
}

void UCGameInstance::Host()
{
	if(!!Menu)
		Menu->Detach();
	
	CLog::Print("Host");
	
	UWorld* world = GetWorld();
	CheckNull(world);

	//-> Everybody Move to Play Map
	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::Join(const FString& InAddress)
{
	if (!!Menu)
		Menu->Detach();

	CLog::Print("Join to " + InAddress);
	//GetEngine()->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("Join to %s"), InAddress));
	
	// Same Code
	//UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//GetWorld()->GetFirstPlayerController();
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::ReturnToMainMenu()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

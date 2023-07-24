#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CMenuBase.h"
#include "Widgets/CMenu.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = L"My Session";

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

		// oss는 interface기 때문에, GetSessionInterface에 대한 재정의가 무조건 있음.
		// NULL일 수가 없음
		SessionInterface = oss->GetSessionInterface();		// Session Event Binding
		
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionComplete);
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
	if (SessionInterface.IsValid())
	{
		auto session = SessionInterface->GetNamedSession(SESSION_NAME);

		if (!!session)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
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

void UCGameInstance::OnCreateSessionComplete(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, L"CreateSessionComplete");

	// 세션 생성 실패
	if (InSuccess == false)
	{
		CLog::Log("Could not create Session!!");
		return;
	}

	// 세션 생성 성공
	CLog::Log("Session Name : " + InSessionName.ToString());

	if (!!Menu)
		Menu->Detach();

	CLog::Print("Host");

	UWorld* world = GetWorld();
	CheckNull(world);

	//-> Everybody Move to Play Map
	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::OnDestroySessionComplete(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, L"DestroySessionComplete");

	if(InSuccess == true)
		CreateSession();
}

void UCGameInstance::CreateSession()
{
	FOnlineSessionSettings sessionSettings;
	SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
}

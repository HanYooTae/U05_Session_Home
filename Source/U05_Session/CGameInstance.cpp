#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CMenuBase.h"
#include "Widgets/CMenu.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = L"GameSession";
const static FName SESSION_SETTINGS_KEY = TEXT("SessionKey");

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
		// Session Event Binding
		SessionInterface = oss->GetSessionInterface();
		
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionComplete);
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

void UCGameInstance::Host(const FString& InSessionName)
{
	DesiredSessionName = InSessionName;

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

void UCGameInstance::Join(uint32 InSessionIndex)
{
	CheckFalse(SessionInterface.IsValid());
	CheckFalse(SessionSearch.IsValid());
	
	if (!!Menu)
		Menu->Detach();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[InSessionIndex]);
}

void UCGameInstance::ReturnToMainMenu()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::FindSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		CLog::Log("Starting Find Session");

		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
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
	world->ServerTravel("/Game/Maps/Lobby?listen");
}

void UCGameInstance::OnDestroySessionComplete(FName InSessionName, bool InSuccess)
{
	UE_LOG(LogTemp, Error, L"DestroySessionComplete");

	if(InSuccess == true)
		CreateSession();
}

void UCGameInstance::OnFindSessionComplete(bool InSuccess)
{
	if (InSuccess == true &&
		Menu != nullptr &&
		SessionSearch.IsValid())
	{
		TArray<FSessionData> foundSession;

		CLog::Log("Finished Find Session");

		// 검색 결과
		CLog::Log("========<Find Session Results>========");
		for (const auto& searchResult : SessionSearch->SearchResults)
		{
			FSessionData data;
			data.MaxPlayers = searchResult.Session.SessionSettings.NumPublicConnections;
			data.CurrentPlayers = data.MaxPlayers - searchResult.Session.NumOpenPublicConnections;
			data.HostUserName = searchResult.Session.OwningUserName;

			FString sessionName;	// value값 return
			if (searchResult.Session.SessionSettings.Get(SESSION_SETTINGS_KEY, sessionName))
			{
				data.Name = sessionName;
			}
			else
			{
				CLog::Log("Session Settings Key Not Found");
			}

			CLog::Log(" -> Session ID : " + data.Name);
			CLog::Log(" -> Ping : " + FString::FromInt(searchResult.PingInMs));

			foundSession.Add(data);
		}
		CLog::Log("======================================");
		
		Menu->SetSessionList(foundSession);
	}
}

void UCGameInstance::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult)
{
	FString address;		// 세션의 IP주소를 address에 리턴해줌

	// 조인 실패
	if (SessionInterface->GetResolvedConnectString(InSessionName, address) == false)
	{
		switch (InResult)
		{
		case EOnJoinSessionCompleteResult::SessionIsFull:
			CLog::Log("SessionIsFull");
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			CLog::Log("SessionDoesNotExist");
			break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
			CLog::Log("CouldNotRetrieveAddress");
			break;
		case EOnJoinSessionCompleteResult::AlreadyInSession:
			CLog::Log("AlreadyInSession");
			break;
		case EOnJoinSessionCompleteResult::UnknownError:
			CLog::Log("UnknownError");
			break;
		}
		return;
	}

	// 조인 성공
	CLog::Print("Join to" + address);
	
	// Same Code
	//UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//GetWorld()->GetFirstPlayerController();
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			sessionSettings.bIsLANMatch = true;
			sessionSettings.bUsesPresence = false;
		}
		else
		{
			sessionSettings.bIsLANMatch = false;
			sessionSettings.bUsesPresence = true;
		}

		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.Set(SESSION_SETTINGS_KEY, DesiredSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

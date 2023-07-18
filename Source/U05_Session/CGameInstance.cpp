#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	CLog::Log("GameInstance::Constructor Called");

	CHelpers::GetClass(&MenuWidgetClass, "/Game/Widgets/WB_Menu");
	//CLog::Log(MenuWidgetClass->GetName());
}

// BeginPlay
void UCGameInstance::Init()
{
	Super::Init();

	CLog::Log("GameInstance::Init Called");
}

void UCGameInstance::LoadMenu()
{
	CheckNull(MenuWidgetClass);

	UUserWidget* menu = CreateWidget(this, MenuWidgetClass);
	CheckNull(menu);

	menu->AddToViewport();

	menu->bIsFocusable = true;

	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(menu->TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}

void UCGameInstance::Host()
{
	CLog::Print("Host");
	
	//-> Everybody Move to Play Map
	UWorld* world = GetWorld();
	CheckNull(world);

	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::Join(const FString& InAddress)
{
	CLog::Print("Join to " + InAddress);
	//GetEngine()->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("Join to %s"), InAddress));
	
	// Same Code
	//UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//GetWorld()->GetFirstPlayerController();
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}
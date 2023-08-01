#include "CLobbyGameMode.h"
#include "Global.h"

void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NumberOfPlayers++;
	CLog::Print("Player : " + FString::FromInt(NumberOfPlayers));

	UWorld* world = GetWorld();
	CheckNull(world);
	if (NumberOfPlayers >= 3)
	{
		world->ServerTravel("/Game/Maps/Play?listen");
	}
}

void ACLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	NumberOfPlayers--;
	CLog::Print("Player : " + FString::FromInt(NumberOfPlayers));
}
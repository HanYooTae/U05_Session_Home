#include "CButton.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CMovingPlatform.h"

ACButton::ACButton()
{
	CHelpers::CreateSceneComponent(this, &Box, "Box");
	CHelpers::CreateSceneComponent(this, &Mesh, "Mesh", Box);

	UStaticMesh* meshAsset;
	CHelpers::GetAsset(&meshAsset, "StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'");
	Mesh->SetStaticMesh(meshAsset);
	Mesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.01f));
	Mesh->SetRelativeLocation(FVector(0, 0, -30));

	UMaterialInstanceConstant* materialAsset;
	CHelpers::GetAsset(&materialAsset, "MaterialInstanceConstant'/Game/PlatForms/MAT_MovingPlatForm_Inst.MAT_MovingPlatForm_Inst'");
	Mesh->SetMaterial(0, materialAsset);

	Box->bHiddenInGame = false;
}

void ACButton::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACButton::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ACButton::EndOverlap);
}

void ACButton::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	for (ACMovingPlatform* platform : Platforms)
		platform->IncreaseActive();

	CLog::Log(character->GetName() + " in");
}

void ACButton::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	for (ACMovingPlatform* platform : Platforms)
		platform->DecreaseActive();

	CLog::Log(character->GetName() + " out");
}

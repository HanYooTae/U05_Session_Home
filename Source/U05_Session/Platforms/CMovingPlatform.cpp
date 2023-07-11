#include "CMovingPlatform.h"

ACMovingPlatform::ACMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(L"StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'");
	if(meshAsset.Succeeded())
		GetStaticMeshComponent()->SetStaticMesh(meshAsset.Object);

	GetStaticMeshComponent()->SetRelativeScale3D(FVector(1.f, 1.f, 0.2f));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
}

void ACMovingPlatform::Tick(float DeltaTime)
{
	// ��ġ�̵�
	Super::Tick(DeltaTime);
	
	// 1. ���� ���� ��ġ ������
	FVector location = GetActorLocation();

	// 2. ��ġ ���
	location += FVector(1, 0, 0) * Speed * DeltaTime;

	// 3. ��ġ ����
	SetActorLocation(location);
}

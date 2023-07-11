#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CMovingPlatform.generated.h"

UCLASS()
class U05_SESSION_API ACMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ACMovingPlatform();

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		float Speed = 20.f;
};

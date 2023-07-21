#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMenuInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class U05_SESSION_API IIMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Host() = 0;
	virtual void Join(const FString& InAddress) = 0;
	virtual void ReturnToMainMenu() = 0;
};

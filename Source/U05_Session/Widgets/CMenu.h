#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMenu.generated.h"

UCLASS()
class U05_SESSION_API UCMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	class UButton* HostButton;
	class UButton* JoinButton;
};

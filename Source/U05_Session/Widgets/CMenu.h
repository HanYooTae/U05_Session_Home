#pragma once

#include "CoreMinimal.h"
#include "CMenuBase.h"
#include "CMenu.generated.h"

UCLASS()
class U05_SESSION_API UCMenu : public UCMenuBase
{
	GENERATED_BODY()
	
public:
	UCMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

public:
	void SetSessionList(TArray<FString> InSessionIDs);
	void SetSelectedRowIndex(uint32 InIndex);

private:
	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void QuitGame();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;


	UPROPERTY(meta = (BindWidget))
		class UScrollBox* SessionList;

private:
	// UUserWidet(부모여도 상관없음)
	TSubclassOf<class UCSessionRow> SessionRowClass;

	TOptional<uint32> SelectedRowIndex;
};

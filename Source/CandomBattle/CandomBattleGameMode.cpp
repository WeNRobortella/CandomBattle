// Fill out your copyright notice in the Description page of Project Settings.


#include "CandomBattleGameMode.h"

#include "CandomBattleGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ACandomBattleGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (UCandomBattleGameInstance* GameInstance = GetGameInstance<UCandomBattleGameInstance>())
		GameInstance->CurrentLevel = *UGameplayStatics::GetCurrentLevelName(this);
}

void ACandomBattleGameMode::ShowGameOverHUD(APlayerController* Controller)
{
	if (Controller)
	{
		GameOverHUDWidget = CreateWidget<UGameOverHUD>(Controller, GameOverHUDClass);
		if (GameOverHUDWidget)
		{
			GameOverHUDWidget->AddToPlayerScreen();
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameOverHUDWidget->TakeWidget());
			Controller->SetInputMode(InputMode);
			Controller->SetShowMouseCursor(true);
		}
	}
}
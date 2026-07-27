// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GameOverHUD.h"

#include "CandomBattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API ACandomBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverHUD> GameOverHUDClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGameOverHUD* GameOverHUDWidget;
	
	virtual void BeginPlay() override;
	
	void ShowGameOverHUD(APlayerController* Controller);
};

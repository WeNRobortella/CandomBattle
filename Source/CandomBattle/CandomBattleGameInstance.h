// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CandomBattleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API UCandomBattleGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FName CurrentLevel;
};

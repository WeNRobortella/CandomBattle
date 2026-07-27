// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* HPText;
	
	void SetHPText(int NewHP);
};

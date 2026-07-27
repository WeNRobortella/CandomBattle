// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "GameOverHUD.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API UGameOverHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* GameOverMessage;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Continue;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Quit;
	
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void OnContinueClicked();
	UFUNCTION()
	void OnQuitClicked();
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverHUD.h"

#include "Kismet/GameplayStatics.h"

void UGameOverHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Continue->OnClicked.AddDynamic(this, &UGameOverHUD::OnContinueClicked);
	Quit->OnClicked.AddDynamic(this, &UGameOverHUD::OnQuitClicked);
}

void UGameOverHUD::OnContinueClicked()
{
	if (APlayerController* Controller = GetOwningPlayer())
	{
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->SetShowMouseCursor(false);
	}
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UGameOverHUD::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
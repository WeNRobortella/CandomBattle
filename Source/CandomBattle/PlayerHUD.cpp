// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::SetHPText(int NewHP)
{
	HPText->SetText(FText::FromString(FString::Printf(TEXT("VIDA: %d"), NewHP)));
}
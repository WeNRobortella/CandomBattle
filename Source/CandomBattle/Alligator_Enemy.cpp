// Fill out your copyright notice in the Description page of Project Settings.


#include "Alligator_Enemy.h"

AAlligator_Enemy::AAlligator_Enemy()
{
	
}

void AAlligator_Enemy::BeginPlay()
{
	ActorStartingLocationX = GetActorLocation().X;
	Amplitude /= 2;
	BaseX = ActorStartingLocationX - Amplitude;
}

void AAlligator_Enemy::Tick(float DeltaTime)
{
	PrimaryActorTick.bCanEverTick = true;
	
	ActivateMovement();
}

void AAlligator_Enemy::ActivateMovement()
{
	if (CanMove && IsAlive)
	{
		float Time = GetWorld()->GetTimeSeconds();
		float XOffset = FMath::Sin(Time * Frequency) * Amplitude;
		SetActorLocation(FVector(BaseX + XOffset, GetActorLocation().Y, GetActorLocation().Z));
	}
}
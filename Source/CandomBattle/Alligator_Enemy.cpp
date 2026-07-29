// Fill out your copyright notice in the Description page of Project Settings.


#include "Alligator_Enemy.h"

AAlligator_Enemy::AAlligator_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;	
}

void AAlligator_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	ActorStartingLocationX = GetActorLocation().X;
	Amplitude /= 2.f;
	BaseX = ActorStartingLocationX - Amplitude;
}

void AAlligator_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
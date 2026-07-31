// Fill out your copyright notice in the Description page of Project Settings.


#include "Alligator_Enemy.h"

AAlligator_Enemy::AAlligator_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	HitBox->SetupAttachment(RootComponent);
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
		float Direction = Time * Frequency;
		float XOffset = FMath::Sin(Direction) * Amplitude;
		SetActorLocation(FVector(BaseX + XOffset, GetActorLocation().Y, GetActorLocation().Z));
		UpdateDirection(Direction);
	}
}

void AAlligator_Enemy::UpdateDirection(float Direction)
{
	FRotator CurrentRotation = GetActorRotation();
	Direction = FMath::Fmod(FMath::RadiansToDegrees(Direction), 360.f);
	if (Direction > 90.f && Direction <= 270.f)
	{
		if (CurrentRotation.Yaw == 180.f)
			SetActorRotation(FRotator(CurrentRotation.Pitch, 0.f, CurrentRotation.Roll));
	}
	else
	{
		if (CurrentRotation.Yaw == 0.f)
			SetActorRotation(FRotator(CurrentRotation.Pitch, 180.f, CurrentRotation.Roll)); 
	}
}
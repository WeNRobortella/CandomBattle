// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "PlayerCharacter.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PlayerDetectorComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	PlayerDetectorComp->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerDetectorComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerDetectorBeginOverlap);
	PlayerDetectorComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnPlayerDetectorEndOverlap);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentStateCheck();
}

void AEnemy::OnPlayerDetectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		CurrentState = EEnemyState::Patrol;
	}
}

void AEnemy::OnPlayerDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		CurrentState = EEnemyState::Idle;
	}
}

void AEnemy::CurrentStateCheck()
{
	switch (CurrentState)
	{
	case EEnemyState::Idle:
		if (CanMove)
			CanMove = false;
		break;
	case EEnemyState::Patrol:
		if (!CanMove)
			CanMove = true;
		break;
	default:
		break;
	}
}

bool AEnemy::ReceiveDamage(int Value)
{
	HitPoints -= Value;
	
	if (HitPoints <= 0 && IsAlive)
	{
		HitPoints = 0;
		IsAlive = false;
		CanMove = false;
		return true;
	}
	return false;
}
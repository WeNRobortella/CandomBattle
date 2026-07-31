// Fill out your copyright notice in the Description page of Project Settings.


#include "Spider_Enemy.h"

#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

ASpider_Enemy::ASpider_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitboxComp = CreateDefaultSubobject<USphereComponent>(TEXT("HitboxComp"));
	HitboxComp->SetupAttachment(RootComponent);
}

void ASpider_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	ActorStartingLocationZ = GetActorLocation().Z;
	
	HitboxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpider_Enemy::OnHitboxBeginOverlap);
	
	Amplitude /= 2.f;
	BaseZ = ActorStartingLocationZ - Amplitude;
}

void ASpider_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ActivateMovement();
}

void ASpider_Enemy::ActivateMovement()
{
	if (CanMove && IsAlive)
	{
		float Time = GetWorld()->GetTimeSeconds();
		float ZOffset = FMath::Sin(Time * Frequency) * Amplitude;
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, BaseZ + ZOffset));
	}
}

void ASpider_Enemy::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->ReceiveDamage(AttackPower, GetActorLocation());
	}
}

bool ASpider_Enemy::ReceiveDamage(int Value)
{
	if (Super::ReceiveDamage(Value))
	{
		HitboxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        HitboxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
        
        GetAnimInstance()->JumpToNode(FName("JumpDefeat"), FName("SpiderEnemyStateMachine"));
	}
	return false;
}
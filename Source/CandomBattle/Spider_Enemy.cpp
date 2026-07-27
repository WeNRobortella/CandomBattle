// Fill out your copyright notice in the Description page of Project Settings.


#include "Spider_Enemy.h"

#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

ASpider_Enemy::ASpider_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HurtboxComp = CreateDefaultSubobject<USphereComponent>(TEXT("HurtboxComp"));
	HurtboxComp->SetupAttachment(RootComponent);
}

void ASpider_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	ActorStartingLocationZ = GetActorLocation().Z;
	
	HurtboxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpider_Enemy::OnHurtboxBeginOverlap);
	
	Amplitude /= 2.f;
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
		float BaseZ = ActorStartingLocationZ - Amplitude;
		float ZOffset = FMath::Sin(Time * Frequency) * Amplitude;
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, BaseZ + ZOffset));
	}
	// if (CanMove && IsAlive)
	// {
	// 	if (ActorStartingLocationZ >= GetActorLocation().Z && ActorStartingLocationZ - MovementRange <= GetActorLocation().Z && GetVelocity().Z == 0)
	// 	{
	// 		GetCharacterMovement()->Velocity.Z = GetCharacterMovement()->GetMaxSpeed() * MovementState;
	// 	}
	// }
	// else
	// {
	// 	if (GetVelocity().Z != 0.f)
	// 	{
	// 		GetCharacterMovement()->StopMovementImmediately();
	// 	}
	// }
	// if (ActorStartingLocationZ < GetActorLocation().Z || ActorStartingLocationZ - MovementRange > GetActorLocation().Z)
	// {
	// 	GetCharacterMovement()->StopMovementImmediately();
	// 	MovementState *= -1;
	// 	if (ActorStartingLocationZ < GetActorLocation().Z)
	// 	{
	// 		double MovementExcess = ActorStartingLocationZ - GetActorLocation().Z;
	// 		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ActorStartingLocationZ + MovementExcess));
	// 	}
	// 	else if (ActorStartingLocationZ - MovementRange > GetActorLocation().Z)
	// 	{
	// 		double MovementExcess = (ActorStartingLocationZ - MovementRange) - GetActorLocation().Z;
	// 		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, (ActorStartingLocationZ - MovementRange) + MovementExcess));
	// 	}
	// }
}

void ASpider_Enemy::OnHurtboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
		HurtboxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        HurtboxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
        
        GetAnimInstance()->JumpToNode(FName("JumpDefeat"), FName("SpiderEnemyStateMachine"));
	}
	return false;
}
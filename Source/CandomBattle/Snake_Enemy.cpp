// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake_Enemy.h"

#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASnake_Enemy::ASnake_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitboxComp = CreateDefaultSubobject<UBoxComponent>("HitboxComp");
	HitboxComp->SetupAttachment(RootComponent);
}

void ASnake_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	StartingLocation = GetActorLocation();
	
	HitboxComp->OnComponentBeginOverlap.AddDynamic(this, &ASnake_Enemy::OnHitboxBeginOverlap);
}

void ASnake_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ActivateAttack();
	ActivateMovement(DeltaTime);
}

bool ASnake_Enemy::ReceiveDamage(int Value)
{
	if (Super::ReceiveDamage(Value))
	{
		HitboxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HitboxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		
		GetAnimInstance()->JumpToNode(FName("JumpDefeat"), FName("SnakeEnemyStateMachine"));
		
		return true;
	}
	return false;
}

void ASnake_Enemy::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->ReceiveDamage(AttackPower, GetActorLocation());
	}
}

void ASnake_Enemy::ActivateAttack()
{
	if (CanMove && IsAlive && !CanAttack)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
        {
        	DistanceToPlayer = Player->GetActorLocation() - GetActorLocation();
			DirectionToPlayer = DistanceToPlayer.GetSafeNormal();
			DistanceToPlayer = DistanceToPlayer.GetAbs();
			CanAttack = true;
			CurrentAttackState = EAttackState::Attack;
        }
	}
}

void ASnake_Enemy::ActivateMovement(float DeltaTime)
{
	if (IsAlive && CanAttack && CurrentAttackState != EAttackState::Passive)
	{
		float FrameTackleSpeed = TackleSpeed * DeltaTime;
		if (DistanceToPlayer.X > 0)
		{
			SetActorLocation(FVector(GetActorLocation().X + FrameTackleSpeed * DirectionToPlayer.X, GetActorLocation().Y, GetActorLocation().Z));
			DistanceToPlayer.X = DistanceToPlayer.X - FrameTackleSpeed * FMath::Abs(DirectionToPlayer.X);
		}
		else if (DistanceToPlayer.X < 0)
		{
			SetActorLocation(FVector(GetActorLocation().X - DistanceToPlayer.X * FMath::Sign(DirectionToPlayer.X), GetActorLocation().Y, GetActorLocation().Z));
			DistanceToPlayer.X = 0;
		}
		if (DistanceToPlayer.Z > 0)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + FrameTackleSpeed * DirectionToPlayer.Z));
			DistanceToPlayer.Z = DistanceToPlayer.Z - FrameTackleSpeed * FMath::Abs(DirectionToPlayer.Z);
		}
		else if (DistanceToPlayer.Z < 0)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,GetActorLocation().Z - DistanceToPlayer.Z * FMath::Sign(DirectionToPlayer.Z)));
			DistanceToPlayer.Z = 0;
		}
		if (DistanceToPlayer.X == 0 && DistanceToPlayer.Z == 0 && CurrentAttackState != EAttackState::Recharge)
		{
			CurrentAttackState = EAttackState::Recharge;
			DistanceToPlayer = StartingLocation - GetActorLocation();
			DirectionToPlayer = DistanceToPlayer.GetSafeNormal();
			DistanceToPlayer = DistanceToPlayer.GetAbs();
			GetWorldTimerManager().SetTimer(AttackCooldown, this, &ASnake_Enemy::AttackRecharged, 1.f, false, 2.f);
		}
	}
}

void ASnake_Enemy::AttackRecharged()
{
	CanAttack = false;
	CurrentAttackState = EAttackState::Passive;
}
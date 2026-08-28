// Fill out your copyright notice in the Description page of Project Settings.


#include "Alligator_Enemy.h"
#include "PlayerCharacter.h"

AAlligator_Enemy::AAlligator_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	HitboxComp = CreateDefaultSubobject<UBoxComponent>("HitBoxComp");
	HitboxComp->SetupAttachment(RootComponent);
}

void AAlligator_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	ActorStartingLocationX = GetActorLocation().X;
	Amplitude /= 2.f;
	BaseX = ActorStartingLocationX - Amplitude;
	
	HitboxComp->OnComponentBeginOverlap.AddDynamic(this, &AAlligator_Enemy::OnHitBoxBeginOverlap);
}

void AAlligator_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ActivateMovement(DeltaTime);
}

void AAlligator_Enemy::ActivateMovement(float DeltaTime)
{
	if (CanMove && IsAlive)
	{
		MovementTimer = FMath::Fmod(MovementTimer + DeltaTime, (2 * UE_DOUBLE_PI) / Frequency);
		float Direction = MovementTimer * Frequency;
		float XOffset = FMath::Cos(Direction) * Amplitude;
		SetActorLocation(FVector(BaseX + XOffset, GetActorLocation().Y, GetActorLocation().Z));
		UpdateDirection(Direction);
	}
}

void AAlligator_Enemy::UpdateDirection(float Direction)
{
	FRotator CurrentRotation = GetActorRotation();
	Direction = FMath::Fmod(FMath::RadiansToDegrees(Direction), 360.f);
	if (Direction > 0.f && Direction <= 180.f)
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

void AAlligator_Enemy::OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->ReceiveDamage(AttackPower, GetActorLocation());
	}
}

bool AAlligator_Enemy::ReceiveDamage(int Value)
{
	if (Super::ReceiveDamage(Value))
	{
		HitboxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HitboxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		
		GetAnimInstance()->JumpToNode(FName("JumpDefeat"), FName("AlligatorEnemyStateMachine"));
		
		return true;
	}
	return false;
}
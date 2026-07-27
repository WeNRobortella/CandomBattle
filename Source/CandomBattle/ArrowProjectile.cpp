// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"

#include "Enemy.h"

// Sets default values
AArrowProjectile::AArrowProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitboxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	SetRootComponent(HitboxComp);
	
	SpriteComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	SpriteComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SpriteComp->SetLooping(false);

	HitboxComp->OnComponentBeginOverlap.AddDynamic(this, &AArrowProjectile::OnHitboxBeginOverlap);
	SpriteComp->OnFinishedPlaying.AddDynamic(this, &AArrowProjectile::OnAnimationFinished);
	
	if (GetActorRotation().Yaw >= 180.f)
		MovementDirection *= -1;
}

// Called every frame
void AArrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AnimationFinished)
	{
		FVector CurrentLocation = GetActorLocation();
		float DistanceToMove = Speed * MovementDirection;
		FVector NewLocation = FVector(CurrentLocation.X + DistanceToMove * DeltaTime, CurrentLocation.Y, CurrentLocation.Z);
		SetActorLocation(NewLocation);
	}
}

void AArrowProjectile::OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->ReceiveDamage(AttackPower);
		
		GetWorldTimerManager().ClearTimer(ArrowDeleteTimerHandle);
		Destroy();
	}
}

void AArrowProjectile::OnAnimationFinished()
{
	AnimationFinished = true;
	
	GetWorldTimerManager().SetTimer(ArrowDeleteTimerHandle, this, &AArrowProjectile::ArrowDelete, 3.f, false);
}

void AArrowProjectile::ArrowDelete()
{
	Destroy();
}
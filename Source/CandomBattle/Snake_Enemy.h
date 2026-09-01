// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"

#include "Components/BoxComponent.h"

#include "Snake_Enemy.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	Passive,
	Attack,
	Recharge
};

UCLASS()
class CANDOMBATTLE_API ASnake_Enemy : public AEnemy
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* HitboxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TackleSpeed = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanAttack = false;
	
	FVector StartingLocation;
	FVector DistanceToPlayer;
	FVector DirectionToPlayer;
	
	EAttackState CurrentAttackState = EAttackState::Passive;
	
	FTimerHandle AttackCooldown;
	
	ASnake_Enemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	bool ReceiveDamage(int Value);
	void ActivateAttack();
	void ActivateMovement(float DeltaTime);
	void AttackRecharged();
	
	
	UFUNCTION()
	void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

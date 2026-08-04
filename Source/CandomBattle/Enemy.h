// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "Components/SphereComponent.h"

#include "Enemy.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEnemyState: uint8
{
	Idle,
	Patrol,
	Chase,
	Dead
};

UCLASS()
class CANDOMBATTLE_API AEnemy : public APaperZDCharacter
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* PlayerDetectorComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CanMove = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAlive = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackPower = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoints = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState CurrentState = EEnemyState::Idle;
	
	AEnemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnPlayerDetectorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnPlayerDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void CurrentStateCheck();
	virtual bool ReceiveDamage(int Value);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"

#include "Alligator_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API AAlligator_Enemy : public AEnemy
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* HitboxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ActorStartingLocationX = 0;
	
	float MovementTimer = 0.f;
	float BaseX = 0;
	
	AAlligator_Enemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void ActivateMovement(float DeltaTime);
	void UpdateDirection(float Direction);
	bool ReceiveDamage(int value);
	
	UFUNCTION()
	void OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

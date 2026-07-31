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
	UBoxComponent* HitBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ActorStartingLocationX = 0;
	
	float BaseX = 0;
	
	AAlligator_Enemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void ActivateMovement();
	void UpdateDirection(float Direction);
};

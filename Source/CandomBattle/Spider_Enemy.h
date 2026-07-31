// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"

#include "PaperFlipbookComponent.h"

#include "Spider_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API ASpider_Enemy : public AEnemy
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* HitboxComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	double ActorStartingLocationZ = 0;
	
	float BaseZ = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude = 0.f;
	
	ASpider_Enemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void ActivateMovement();
	
	virtual bool ReceiveDamage(int Value) override;
};

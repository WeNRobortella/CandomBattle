// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"

#include "ArrowProjectile.generated.h"

UCLASS()
class CANDOMBATTLE_API AArrowProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* HitboxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* SpriteComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackPower = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MovementDirection = 1;
	
	FTimerHandle ArrowDeleteTimerHandle;
	
	bool AnimationFinished = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnAnimationFinished();
	
	UFUNCTION()
	void ArrowDelete();
};

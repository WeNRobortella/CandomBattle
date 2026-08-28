// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"

#include "Platform.generated.h"

UENUM()
enum class EInterpAuthAttribute : uint8
{
	InterpBasePeriod,
	InterpMovementSpeed
};

UENUM()
enum class ERotationAuthAttribute : uint8
{
	RotationBasePeriod,
	RotationMovementSpeed
};

UCLASS()
class CANDOMBATTLE_API APlatform : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* OrbitArm;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComp;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* MovementPath;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* RotationRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInterpToMovementComponent* InterpToMoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URotatingMovementComponent* RotatingMoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ActivateInterpFactor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInterpAuthAttribute InterpAuthAttribute = EInterpAuthAttribute::InterpBasePeriod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InterpCycleFactor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpMovementSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpBasePeriod = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ActivateRotationFactor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERotationAuthAttribute RotationAuthAttribute = ERotationAuthAttribute::RotationBasePeriod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RotationCycleFactor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double RotationMovementSpeed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double RotationBasePeriod = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrembleSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FallingPlatform = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlatformTimeToFall = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlatformTimeToRespawn = 0.f;
	
	FTimerHandle FallingPlatformTimer;
	float TrembleAmplitude = 0.5f;
	float TrembleElapsedTime = 0.f;
	FVector FlipbookStartingPosition;

	// Sets default values for this actor's properties
	APlatform();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void FallingPlatformTimerEvent();
	
	UFUNCTION()
	void FallingPlatformEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void TremblingPlatformEffect();
	void CheckPlatformOutOfCameraView();
	void RespawnPlatform();
};

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

#include "Platform.generated.h"

UCLASS()
class CANDOMBATTLE_API APlatform : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* OrbitArm;
	
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComp;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* RotationRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInterpToMovementComponent* InterpToMoveComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URotatingMovementComponent* RotatingMoveComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* FlipbookComp;
	
	// Sets default values for this actor's properties
	APlatform();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

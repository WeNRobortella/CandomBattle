// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "Camera/CameraComponent.h"

#include "FallHazard.generated.h"

/**
 * 
 */
UCLASS()
class CANDOMBATTLE_API AFallHazard : public ATriggerBox
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* HazardCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime = 0.f;
	
	FTimerHandle FallingOutOfBoundsHandle;
	
	AFallHazard();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};

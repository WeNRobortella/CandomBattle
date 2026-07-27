// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "PaperZDAnimInstance.h"
#include "ArrowProjectile.h"
#include "Components/SceneComponent.h"
#include "FallHazard.h"
#include "PlayerHUD.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */

USTRUCT()
struct FMovementFrictionCache
{
	GENERATED_BODY()
	float GroundFriction = 0.f;
	float BrakingDecelerationWalking = 0.f;
};

UCLASS()
class CANDOMBATTLE_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* JumpInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* AttackInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* QuitInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HitPoints = 12;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackForce = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperZDAnimSequence* AttackAnimSequence;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAlive = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanMove = true;
	
	bool CanAttack = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Invulnerable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvulnerableTimer = 1.f;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArrowProjectile> ArrowProjectileToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ArrowProjectileSpawnPosition;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerHUD* PlayerHUDWidget;
	
	FMovementFrictionCache CachedFriction;
	FTimerHandle InvulnerableHandle;
	
	FZDOnAnimationOverrideEndSignature AttackOverrideEndMessenger;
	
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void Move(const FInputActionValue& Value);
	void BeginJump();
	void BreakJump();
	void UpdateDirection(float InputDirection);
	void ReceiveDamage(int Value);
	void ReceiveDamage(int Value, TOptional<FVector> SourceLocation);
	void Defeat();
	void InvulnerabilityEnd();
	void Attack();
	void AttackOverrideEnd(bool Completed);
	void CheckCharacterOutOfCameraView(AFallHazard* FallHazard);
	void QuitGame();
};

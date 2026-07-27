// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CandomBattleGameMode.h"

#include "PaperZDCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	ArrowProjectileSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ArrowProjectile"));
	ArrowProjectileSpawnPosition->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	
	AttackOverrideEndMessenger.BindUObject(this, &APlayerCharacter::AttackOverrideEnd);
	
	if (PlayerHUDClass)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			PlayerHUDWidget = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
			if (PlayerHUDWidget)
			{
				PlayerHUDWidget->AddToPlayerScreen();
				
				PlayerHUDWidget->SetHPText(HitPoints);
			}
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpInput, ETriggerEvent::Started, this, &APlayerCharacter::BeginJump);
		EnhancedInputComponent->BindAction(JumpInput, ETriggerEvent::Completed, this, &APlayerCharacter::BreakJump);
		EnhancedInputComponent->BindAction(AttackInput, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(QuitInput, ETriggerEvent::Triggered, this, &APlayerCharacter::QuitGame);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	float InputActionValue = Value.Get<float>();
	
	if (IsAlive && CanMove)
	{
		FVector InputDirection = FVector(1.0f, 0.0f, 0.0f);
		AddMovementInput(InputDirection, InputActionValue);
		UpdateDirection(InputActionValue);
	}
}

void APlayerCharacter::BeginJump()
{	
	if (IsAlive && CanMove)
	{
		Jump();
	}
}

void APlayerCharacter::BreakJump()
{
	StopJumping();
}

void APlayerCharacter::UpdateDirection(float InputDirection)
{
	FRotator CurrentRotation = Controller->GetControlRotation();
	
	if (InputDirection < 0.0f)
	{
		if (CurrentRotation.Yaw != 180.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 180.0f, CurrentRotation.Roll));
		}
	} else if (InputDirection > 0.0f)
	{
		if (CurrentRotation.Yaw != 0.0f)
		{
			Controller->SetControlRotation(FRotator(CurrentRotation.Pitch, 0.0f, CurrentRotation.Roll));
		}
	}
}

void APlayerCharacter::ReceiveDamage(int Value, TOptional<FVector> SourceLocation)
{
	if (IsAlive && !Invulnerable)
	{
		HitPoints -= Value; 
		if (PlayerHUDWidget)
			PlayerHUDWidget->SetHPText(HitPoints);
		Invulnerable = true;
		CanMove = false;
		
		GetWorldTimerManager().SetTimer(InvulnerableHandle, this, &APlayerCharacter::InvulnerabilityEnd, 1.f, false, InvulnerableTimer);
		
		if (SourceLocation.IsSet())
		{
			CachedFriction.GroundFriction = GetCharacterMovement()->GroundFriction;
            CachedFriction.BrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;
            
            GetCharacterMovement()->GroundFriction = 2.f;
            GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
            
            float KnockbackSide = FMath::Sign(GetActorLocation().X - (*SourceLocation).X);
            FVector KnockbackDir = FVector(KnockbackSide, 0.f, 1.f);
            
            LaunchCharacter((KnockbackDir * KnockbackForce), true, true);
		}
		
		Defeat();
	}
}

void APlayerCharacter::Defeat()
{
	if (HitPoints <= 0)
	{
		HitPoints = 0;
		if (PlayerHUDWidget)
			PlayerHUDWidget->SetHPText(HitPoints);
		IsAlive = false;
		CanMove = false;
		CanAttack = false;
		
		GetWorldTimerManager().ClearTimer(InvulnerableHandle);
		
		GetAnimInstance()->JumpToNode(FName("JumpDefeat"), FName("PlayerCharacterStateMachine"));
		
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (ACandomBattleGameMode* GameMode = Cast<ACandomBattleGameMode>(GetWorld()->GetAuthGameMode()))
			{
				GameMode->ShowGameOverHUD(PlayerController);
			}
		}
	}
}

void APlayerCharacter::InvulnerabilityEnd()
{
	Invulnerable = false;
	CanMove = true;
	
	GetCharacterMovement()->GroundFriction = CachedFriction.GroundFriction;
	GetCharacterMovement()->BrakingDecelerationWalking = CachedFriction.BrakingDecelerationWalking;
}

void APlayerCharacter::Attack()
{
	if (CanAttack)
	{
		CanMove = false;
        CanAttack = false;
        
        GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, FName("DefaultSlot"), 1.f, 0.f, AttackOverrideEndMessenger);
		
		AArrowProjectile* ArrowProjectile = GetWorld()->SpawnActor<AArrowProjectile>(ArrowProjectileToSpawn, ArrowProjectileSpawnPosition->GetComponentLocation(), GetActorRotation());
	}
}

void APlayerCharacter::AttackOverrideEnd(bool Completed)
{
	CanMove = true;
	CanAttack = true;
}

void APlayerCharacter::CheckCharacterOutOfCameraView(AFallHazard* FallHazard)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		FVector2D ScreenPosition;
		int32 ViewportSizeX, ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
		bool bIsOnScreen = PlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPosition, false);
		
		if (bIsOnScreen == false || ScreenPosition.X < 0 || ScreenPosition.X > ViewportSizeX || ScreenPosition.Y < 0 || ScreenPosition.Y > ViewportSizeY)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_None);
			GetCharacterMovement()->StopMovementImmediately();
			FallHazard->GetWorldTimerManager().ClearTimer(FallHazard->FallingOutOfBoundsHandle);
		}
	}
}

void APlayerCharacter::QuitGame()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}
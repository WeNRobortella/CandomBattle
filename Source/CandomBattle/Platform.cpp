// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform.h"

#include "Math/UnitConversion.h"
#include "Splines/SplineMath.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlatform::APlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OrbitArm = CreateDefaultSubobject<USceneComponent>(FName("OrbitArm"));
	SetRootComponent(OrbitArm);

	ArrowComp = CreateEditorOnlyDefaultSubobject<UArrowComponent>(FName("ArrowComp"));
	if (ArrowComp)
		ArrowComp->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->BodyInstance.DOFMode = EDOFMode::CustomPlane;
	BoxComp->BodyInstance.bLockXRotation = true;
	BoxComp->BodyInstance.bLockYRotation = true;
	BoxComp->BodyInstance.bLockYTranslation = true;
	BoxComp->BodyInstance.bLockZRotation = true;

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(FName("FlipbookComp"));
	FlipbookComp->SetupAttachment(BoxComp);

	MovementPath = CreateEditorOnlyDefaultSubobject<USplineComponent>(TEXT("MovementPath"));
	if (MovementPath)
	{
		MovementPath->SetupAttachment(RootComponent);
		MovementPath->SetRelativeLocation(FVector(0.f, 1.f, 0.f));
	}
	RotationRadius = CreateEditorOnlyDefaultSubobject<USphereComponent>(FName("RotationRadius"));
	if (RotationRadius)
		RotationRadius->SetupAttachment(RootComponent);

	InterpToMoveComp = CreateDefaultSubobject<UInterpToMovementComponent>(FName("InterpToMoveComp"));
	InterpToMoveComp->SetUpdatedComponent(RootComponent);

	RotatingMoveComp = CreateDefaultSubobject<URotatingMovementComponent>(FName("RotatingMoveComp"));
	RotatingMoveComp->SetUpdatedComponent(RootComponent);
}

void APlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MovementPath && InterpToMoveComp)
	{
		MovementPath->ClearSplinePoints(false);
		for (int32 i = 0; i < InterpToMoveComp->ControlPoints.Num(); ++i)
		{
			MovementPath->AddSplinePoint(InterpToMoveComp->ControlPoints[i].PositionControlPoint,
			                             InterpToMoveComp->ControlPoints[i].bPositionIsRelative
				                             ? ESplineCoordinateSpace::Local
				                             : ESplineCoordinateSpace::World, false);
			MovementPath->SetSplinePointType(i, ESplinePointType::Linear, false);
		}
		MovementPath->UpdateSpline();

		if (ActivateInterpFactor)
		{
			float Distance = MovementPath->GetSplineLength();
			float* InterpPeriod = &InterpToMoveComp->Duration;
			if (InterpAuthAttribute == EInterpAuthAttribute::InterpBasePeriod)
				InterpMovementSpeed = Distance / (InterpBasePeriod * InterpCycleFactor);
			if (InterpAuthAttribute == EInterpAuthAttribute::InterpMovementSpeed)
				InterpBasePeriod = Distance / (InterpMovementSpeed * InterpCycleFactor);
			*InterpPeriod = InterpBasePeriod * InterpCycleFactor;
		}

		if (RotationRadius)
		{
			const FVector Relloc = BoxComp->GetRelativeLocation();
			const double Radius = FMath::Sqrt(FMath::Square(Relloc.X) + FMath::Square(Relloc.Z));
			RotationRadius->SetSphereRadius(Radius);

			if (ActivateRotationFactor)
			{
				double* RotationPeriod = &RotatingMoveComp->RotationRate.Pitch;
				if (RotationAuthAttribute == ERotationAuthAttribute::RotationBasePeriod)
					RotationMovementSpeed = (2 * UE_DOUBLE_PI * Radius) / (RotationBasePeriod * RotationCycleFactor);
				if (RotationAuthAttribute == ERotationAuthAttribute::RotationMovementSpeed)
					RotationBasePeriod = (2 * UE_DOUBLE_PI * Radius) / (RotationMovementSpeed * RotationCycleFactor);
				*RotationPeriod = 360. / (RotationBasePeriod * RotationCycleFactor);
			}
		}
	}
}

// Called when the game starts or when spawned
void APlatform::BeginPlay()
{
	Super::BeginPlay();

	if (FallingPlatform)
	{
		FlipbookStartingPosition = FlipbookComp->GetRelativeLocation();
		BoxComp->SetNotifyRigidBodyCollision(true);
		BoxComp->OnComponentHit.AddDynamic(this, &APlatform::FallingPlatformEvent);
	}
}

// Called every frame
void APlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FallingPlatform)
	{
		TrembleElapsedTime = FMath::Fmod(TrembleElapsedTime + DeltaTime, (2 * UE_DOUBLE_PI) / TrembleSpeed);
		TremblingPlatformEffect();
	}
}

void APlatform::TremblingPlatformEffect()
{
	FlipbookComp->SetRelativeLocation(FlipbookStartingPosition + FVector(TrembleAmplitude * FMath::Cos(TrembleSpeed * TrembleElapsedTime), 0.f, 0.f));
}

void APlatform::FallingPlatformEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                     FVector NormalImpulse, const FHitResult& Hit)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		GetWorldTimerManager().SetTimer(FallingPlatformTimer, this, &APlatform::FallingPlatformTimerEvent, 1.f, false,
		                                PlatformTimeToFall);
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, "FallingPlatform Activated");
	}
}

void APlatform::FallingPlatformTimerEvent()
{
	BoxComp->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(FallingPlatformTimer, this, &APlatform::CheckPlatformOutOfCameraView, 1.f, true,
	                                1.f);
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, "FallingPlatform Physics Simulating");
}

void APlatform::CheckPlatformOutOfCameraView()
{
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
	{
		FVector2D ScreenPosition;
		int32 ViewportSizeX, ViewportSizeY;
		Controller->GetViewportSize(ViewportSizeX, ViewportSizeY);
		bool bIsOnScreen = Controller->ProjectWorldLocationToScreen(BoxComp->GetComponentLocation(), ScreenPosition,
		                                                            false);

		if (bIsOnScreen == false || ScreenPosition.X < 0 || ScreenPosition.X > ViewportSizeX || ScreenPosition.Y < 0 ||
			ScreenPosition.Y > ViewportSizeY)
		{
			BoxComp->SetSimulatePhysics(false);
			FlipbookComp->SetHiddenInGame(true);
			BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BoxComp->AttachToComponent(OrbitArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			GetWorldTimerManager().ClearTimer(FallingPlatformTimer);
			GetWorldTimerManager().SetTimer(FallingPlatformTimer, this, &APlatform::RespawnPlatform, 1.f, false, PlatformTimeToRespawn);
		}
	}
}

void APlatform::RespawnPlatform()
{
	FlipbookComp->SetHiddenInGame(false);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, "RespawnPlatform Activated");
}

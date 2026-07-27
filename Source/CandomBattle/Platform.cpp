// Fill out your copyright notice in the Description page of Project Settings.


#include "Platform.h"

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
	
	RotationRadius = CreateEditorOnlyDefaultSubobject<USphereComponent>(FName("RotationRadius"));
	if (RotationRadius)
		RotationRadius->SetupAttachment(RootComponent);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	
	InterpToMoveComp = CreateDefaultSubobject<UInterpToMovementComponent>(FName("InterpToMoveComp"));
	InterpToMoveComp->SetUpdatedComponent(RootComponent);
	
	RotatingMoveComp = CreateDefaultSubobject<URotatingMovementComponent>(FName("RotatingMoveComp"));
	RotatingMoveComp->SetUpdatedComponent(RootComponent);
	
	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(FName("FlipbookComp"));
	FlipbookComp->SetupAttachment(BoxComp);
	
}

void APlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (RotationRadius)
	{
		const FVector Relloc = BoxComp->GetRelativeLocation();
		const float Radius = FMath::Sqrt(FMath::Square(Relloc.X) + FMath::Square(Relloc.Z));
		RotationRadius->SetSphereRadius(Radius);
	}
}

// Called when the game starts or when spawned
void APlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


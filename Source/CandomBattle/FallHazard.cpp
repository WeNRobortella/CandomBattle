// Fill out your copyright notice in the Description page of Project Settings.


#include "FallHazard.h"

#include "PlayerCharacter.h"

AFallHazard::AFallHazard()
{
	UBoxComponent* HazardBox = Cast<UBoxComponent>(GetCollisionComponent());
	HazardBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HazardBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HazardBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	
	HazardCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HazardCamera"));
	HazardCamera->SetupAttachment(RootComponent);
}

void AFallHazard::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->Invulnerable = false;
		Player->ReceiveDamage(999, TOptional<FVector>());
		
		if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
		{
			PlayerController->SetViewTargetWithBlend(HazardCamera->GetOwner(), BlendTime);
		}
		
		FTimerDelegate FallHazardDelegate = FTimerDelegate::CreateUObject(Player, &APlayerCharacter::CheckCharacterOutOfCameraView, this);
		GetWorldTimerManager().SetTimer(FallingOutOfBoundsHandle, FallHazardDelegate, 0.2f, true, 0.2f);
	}
}
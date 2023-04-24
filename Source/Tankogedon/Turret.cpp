// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Cannon.h"
#include "Kismet/KismetMathLibrary.h"
#include "HealthComponent.h"
// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (BodyMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimer;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimer , this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
}

void ATurret::Targeting()
{

	if (!isPlayerSeen())
	{
		return;
	}

	if (!PlayerPawn)
		return;

	if (IsPlayerInRange())
	{
		RotateToPlayer();

		if (CanFire())
		{
			Fire();
		}
	}

}

void ATurret::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetTargetLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));

}

bool ATurret::IsPlayerInRange()
{

	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	if (!isPlayerSeen())
		return false;
	FVector targetDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetDir, DirToPlayer)));
	return aimAngle <= Accurency;
}

bool ATurret::isPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = CannonSetupPoint->GetComponentLocation();
	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(this);

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, params))
	{
		if (hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Purple, false, 0.5f, 0, 5);
			return hitResult.GetActor() == PlayerPawn;

		}
	}
	DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
	return false;
}
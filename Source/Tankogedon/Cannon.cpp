#include "Cannon.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(SceneComp);

}

void ACannon::Fire()
{
	if (!(isReadyToFire() && ammoCount > 0))
	{
		return;
	}
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, FString::Printf(TEXT("Fire projectile \n Ammo: %d"), ammoCount - 1));
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams();
		traceParams.AddIgnoredActor(this);
		traceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red,false,1.0f,0,5);
			if (hitResult.GetActor())
			{
				AActor* OverlappedActor = hitResult.GetActor();
				UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *OverlappedActor->GetName());
				OverlappedActor->Destroy();

			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}

	}
	bReadyToFire = false;
	ammoCount--;
	GetWorld()->GetTimerManager().SetTimer(ReloadTime, this, &ACannon::Reload, FireRate, false);
}

void ACannon::FireSpecial()
{
	if (!(isReadyToFire() && ammoCount > 0))
	{
		return;
	}
	if (CannonType == ECannonType::FireSpecial)
	{
		GetWorld()->GetTimerManager().SetTimer(FireSpecialTime, this, &ACannon::FireSpecialLogic, 0.25f, true);
	}
	bReadyToFire = false;
	ammoCount--;

}

bool ACannon::isReadyToFire()
{
	return bReadyToFire;
}

void ACannon::Reload()
{
	bReadyToFire = true;
	bFireSpecial = 0;
}

void ACannon::FireSpecialLogic()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Fire special \n Ammo: %d"), ammoCount));
	bFireSpecial++;
	if (sprayCount == bFireSpecial)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireSpecialTime);
		GetWorld()->GetTimerManager().SetTimer(ReloadTime, this, &ACannon::Reload, FireSpecialRate, false);
	}
}

void ACannon::ChangeCannon()
{

}

void ACannon::AddAmmo(int count)
{
	ammoCount += count;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();

	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



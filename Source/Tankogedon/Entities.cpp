#include "Entities.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "HealthComponent.h"
#include "Cannon.h"
// Sets default values
AEntities::AEntities()
{
	PrimaryActorTick.bCanEverTick = true;


	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	//TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretComponent"));
	//TurretMesh->SetupAttachment(BodyMesh);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &AEntities::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &AEntities::Die);

}

void AEntities::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);
}

void AEntities::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AEntities::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void AEntities::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass)
	{
		return;
	}
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TempCannonClass = newCannonClass;
}

void AEntities::ChangeCannon()
{
	if (CannonClass && SecondCannonClass)
	{
		CannonClass = SecondCannonClass;
		SecondCannonClass = TempCannonClass;
		SetupCannon(CannonClass);
		TempCannonClass = nullptr;
	}
}

void AEntities::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AEntities::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void AEntities::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

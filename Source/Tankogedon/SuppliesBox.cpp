// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppliesBox.h"
#include "Components/StaticMeshComponent.h"
#include "Cannon.h"
#include "TankPawn.h"
// Sets default values
ASuppliesBox::ASuppliesBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	SupplesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SupplesMesh"));
	SupplesMesh->SetupAttachment(SceneComp);
	SupplesMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SupplesMesh->SetCollisionProfileName("OverlapAll");
	SupplesMesh->OnComponentBeginOverlap.AddDynamic(this, &ASuppliesBox::OnMeshOverlapBegin);

}

void ASuppliesBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		TankPawn->AddAmmo(AmmoCount);
		Destroy();

	}
}

// Called when the game starts or when spawned



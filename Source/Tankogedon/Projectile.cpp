#include "Projectile.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Cannon.h"
#include "DamageTaker.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneComp);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &AProjectile::Move, MoveRate, true, 0.0f);
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;
	FQuat Rotation = FQuat::Identity;
	bool sweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		startPos,
		endPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		params
	);

	DrawDebugSphere(GetWorld(), startPos, ExplodeRadius, 5, FColor::Green, false, 2.0f);

	HitResultCheck(sweepResult, AttackHit);
}

void AProjectile::HitResultCheck(bool sweepResult, TArray<FHitResult>& AttackHit)
{
	if (sweepResult)
	{
		for (FHitResult hitResult : AttackHit)
		{
			AActor* otherActor = hitResult.GetActor();
			if (!otherActor)
				continue;
			IDamageTaker* damageTakerActor = Cast<IDamageTaker>(otherActor);
			if (damageTakerActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;
				damageTakerActor->TakeDamage(damageData);
			}
			else
			{
				UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(otherActor->GetRootComponent());
				if (mesh)
				{
					if (mesh->IsSimulatingPhysics())
					{
						FVector forceVector = otherActor->GetActorLocation() - GetActorLocation();
						forceVector.Normalize();
						mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
					}
				}
			}
			Destroy();
		}
	}
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* owner = GetOwner();
	AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	if (OtherActor != owner || OtherActor != OwnerByOwner)
	{

		if (OtherActor)
		{
			IDamageTaker* DamageActor = Cast<IDamageTaker>(OtherActor);
			if (DamageActor)
			{
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = owner;
				damageData.DamageMaker = this;

				DamageActor->TakeDamage(damageData);

			}
			else
			{
				UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
				if (mesh)
				{
					if (mesh->IsSimulatingPhysics())
					{
						FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
						forceVector.Normalize();
						mesh->AddForce(forceVector * PushForce, NAME_None, true);
						//mesh->AddImpulse(forceVector * PushForce, NAME_None, true);

					}
					else
					{
						OtherActor->Destroy();
					}
				}
				//UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OtherActor->GetName());
			}
			Destroy();
		}
	}

}


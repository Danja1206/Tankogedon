// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Start();

protected:
	// Called when the game starts or when spawned

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float MoveRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float PushForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float ExplodeRadius = 5;

	FTimerHandle MovementTimer;

	UFUNCTION()
		virtual void Move();

	void Explode();
	void HitResultCheck(bool sweepResult, TArray<FHitResult>& AttackHit);

	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			bool bFromSweep, const FHitResult& SweepResult);


};

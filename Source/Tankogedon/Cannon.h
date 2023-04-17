// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Cannon.generated.h"

class UArrowComponent;

UCLASS()
class TANKOGEDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

	void Fire();
	bool FireAI();
	void FireAILogic();
	void FireSpecial();
	bool isReadyToFire();
	void Reload();
	void FireSpecialLogic();
	void ChangeCannon();
	void AddAmmo(int count);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireSpecialRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int ammoCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int sprayCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		class UParticleSystemComponent* ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		class UAudioComponent* AudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int fireAITime = 5;

	int bFireSpecial = 0;
	int bFireAI = 0;
	bool bReadyToFire = true;
	FTimerHandle ReloadTime;
	FTimerHandle FireSpecialTime;
	FTimerHandle FireAITime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

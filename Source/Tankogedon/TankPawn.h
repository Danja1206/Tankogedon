// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities.h"
#include "DamageTaker.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDON_API ATankPawn : public AEntities
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);

	void RotationRight(float Value);

	void Move(float DeltaTime);

	void AddAmmo(int count);

	TArray<FVector> GetPatrollingPath() const { return PatrollingPath; };
	float GetMovementAccurency() const { return MovementAccurency; };

	FVector GetTurretForwardVector() const;

	void RotateTurretTo(FVector TargetPosition);

	FVector GetEyesPosition() const;






protected:

	virtual void BeginPlay() override;

	//TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretComponent"));
	//TurretMesh->SetupAttachment(BodyMesh);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (MakeEditWidget = true))
		TArray<FVector> PatrollingPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float MovementAccurency = 50;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* TurretMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Rotation")
		float TurretRotationInterpolationKey = 0.5f;

	float targetForwardAxisValue = 0.0f;

	float targetRotateAxisValue = 0.0f;

	float axisRotateRight = 0.0f;

	UPROPERTY()
		class ATankPlayerController* TankController;


	// Called to bind functionality to input

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject< UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//move
	moveMethod(DeltaTime);

	//rotation
	targetRotateAxisValue = FMath::Lerp(targetRotateAxisValue, axisRotateRight,DeltaTime);
	float NewRotation = GetActorRotation().Yaw + targetRotateAxisValue * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, NewRotation, 0.f));

	//turret rotation
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePosition();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator TurretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = TurretRotation.Pitch;
		targetRotation.Roll = TurretRotation.Roll;

		FRotator newTurretRotation = FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey);
		TurretMesh->SetWorldRotation(newTurretRotation);
	}



}

void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;

	

}

void ATankPawn::RotationRight(float Value)
{
	axisRotateRight = Value;
}

void ATankPawn::moveMethod(float DeltaTime)
{
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector movePosition = CurrentPosition + ForwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime;
	SetActorLocation(movePosition, true);
}

void ATankPawn::AddAmmo(int count)
{
	if (Cannon)
	{
		Cannon->AddAmmo(count);
	}
}


void ATankPawn::BeginPlay()
{

	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotationRight", this, &ATankPlayerController::RotationRight);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float Value)
{
	if (TankPawn) 
	{
		TankPawn->MoveForward(Value);
	}
}

void ATankPlayerController::RotationRight(float Value)
{
	if (TankPawn)
	{
		TankPawn->RotationRight(Value);
	}
}

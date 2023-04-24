// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TankPawn->MoveForward(1.0f);

	TankPawn->RotationRight(GetRotationValue());

	Targeting();
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	if (!TankPawn)
	{
		return;
	}

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPath();
	for (FVector point : points)
	{
		PatrollingPoints.Add(point + pawnLocation);
	}
	CurrentPatrollingIndex = 0;
}

float ATankAIController::GetRotationValue()
{
	FVector CurrentPoint = PatrollingPoints[CurrentPatrollingIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();
	if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccurency)
	{
		CurrentPatrollingIndex++;
		if (CurrentPatrollingIndex >= PatrollingPoints.Num())
		{
			CurrentPatrollingIndex = 0;
		}
	}

	FVector moveDirection = CurrentPoint - PawnLocation;
	moveDirection.Normalize();
	FVector forwardDirection = TankPawn->GetActorForwardVector();
	FVector rightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
	float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));

	float RotationValue = 0.0f;
	if (forwardAngle > 2.0f)
	{
		RotationValue = 1.0f;
	}
	if (rightAngle > 90)
	{
		RotationValue = -RotationValue;
	}
	return RotationValue;
}

void ATankAIController::Targeting()
{
	if (!isPlayerSeen() || !isPlayerRange())
	{
		return;
	}

	if (CanFire())
	{
		Fire();

	}
	else
	{
		RotateToPlayer();
	}
}

void ATankAIController::RotateToPlayer()
{
	TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}

bool ATankAIController::isPlayerRange()
{
	if(!PlayerPawn)
		return false;
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::CanFire()
{
	if (!isPlayerSeen())
	{
		return false;
	}
	FVector targetingDir = TankPawn->GetTurretForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();

	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));

	return AimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	TankPawn->Fire();
}

bool ATankAIController::isPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = TankPawn->GetEyesPosition();
	FCollisionQueryParams params = FCollisionQueryParams();
	params.AddIgnoredActor(TankPawn);

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

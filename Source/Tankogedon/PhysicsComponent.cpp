// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsComponent.h"

// Sets default values for this component's properties
UPhysicsComponent::UPhysicsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue)
{
	TArray<FVector> trajectory;
	FVector GravityVector(0.0f, 0.0f, Gravity);

	for (float time = 0; time < MaxTime; time += TimeStep)
	{
		FVector position = StartPos + Velocity * time + GravityVector * time * time / 2;
		if (position.Z <= MinZValue)
		{
			break;
		}
		trajectory.Add(position);

	}
	return trajectory;
}


// Called when the game starts
void UPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


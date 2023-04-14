// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{

}

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	CurrentHealth -= DamageData.DamageValue;
	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound())
			OnDie.Broadcast();
	}
	else
	{
		if(OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(DamageData.DamageValue);
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth/MaxHealth ;
}

void UHealthComponent::AddHealth(float newHealth)
{
	CurrentHealth += MaxHealth;
	if (CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = MaxHealth;
	
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStruct.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireTrace = 0 UMETA(DisplayName = "Use Trace"),
	FireProjectile = 1 UMETA(DisplayName = "Use projectile"),
	FireSpecial = 1 UMETA(DisplayName = "Use special")
};

/**
 * 
 */
UCLASS()
class TANKOGEDON_API UGameStruct : public UObject
{
	GENERATED_BODY()
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HorrorGameMode.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled,AActor*, VictimActor,AActor*, KillerActor,AController*, KillerController);


/**
 * 
 */
UCLASS()
class HORROR_API AHorrorGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled OnActorKilled;
};

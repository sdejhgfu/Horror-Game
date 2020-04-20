// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Guns/GunBaseClass.h"
#include "TraceGun.generated.h"

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY();
public:

	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
		FVector_NetQuantize TraceEnd;
};

/**
 * 
 */
UCLASS()
class HORROR_API ATraceGun : public AGunBaseClass
{
	GENERATED_BODY()
	
	virtual void Fire() override;

	FHitScanTrace HitScanTrace;

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);
};

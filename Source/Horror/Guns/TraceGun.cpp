// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/Public/TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Horror.h"

void ATraceGun::Fire()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
		return;

	if (CurrentClipBulletCount <= 0)
		return;

	if (bIsReloading)
		return;

	//trace from pawn eyes to crosshair
	FVector EyeLocation;
	FRotator EyeRotation;
	FVector ShotDirection;



	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	ShotDirection = EyeRotation.Vector();

	//creating bullet spread
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

	FVector TraceEnd = EyeLocation + (ShotDirection * SightRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;
	FHitResult Hit;

	//particle "Target" param
	FVector TracerEndPoint = TraceEnd;
	EPhysicalSurface SurfaceType = SurfaceType_Default;

	if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
	{
		//hit blocked deal dmg

		AActor* HitActor = Hit.GetActor();

		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		float ActualDamage = BaseDamage;
		if (SurfaceType == SURFACE_FLESHVULNERABLE)
		{
			ActualDamage *= BonusDamageMultiplier;
		}
		UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, GetInstigatorController(), MyOwner, DamageType);


		PlayImpactEffect(SurfaceType, Hit.ImpactPoint);

		TracerEndPoint = Hit.ImpactPoint;
		HitScanTrace.SurfaceType = SurfaceType;
	}

	
	
	//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.f, 0, 1.f);
	

	PlayFireEffect(TracerEndPoint);
	
	HitScanTrace.TraceEnd = TracerEndPoint;

	LastFireTime = GetWorld()->TimeSeconds;

	CurrentClipBulletCount = (int)FMath::Clamp(CurrentClipBulletCount - 1, 0, MagazineSize);

	bCanReload = true;
}

void ATraceGun::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = ImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}
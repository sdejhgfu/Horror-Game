// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBaseClass.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
AGunBaseClass::AGunBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh Comp");
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	RateOfFire = 600;

	CurrentBulletCount = MagazineSize;
}

// Called when the game starts or when spawned
void AGunBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
	TimeBetweenShots = 60 / RateOfFire;

	
}

// Called every frame
void AGunBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#pragma region Relaoding

void AGunBaseClass::Reload()
{
	//check to see if can reload
	if (bCanReload && !bIsReloading)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &AGunBaseClass::Reloaded, ReloadTime, false, ReloadTime);
		bIsReloading = true;
	}
}

void AGunBaseClass::Reloaded()
{
	bCanReload = false;
	bIsReloading = false;
	CurrentBulletCount = MagazineSize;
}


bool AGunBaseClass::CanReload() const
{
	return bCanReload;
}
int AGunBaseClass::GetCurrentBulletCount() const
{
	return CurrentBulletCount;
}


#pragma endregion

#pragma region Firing

void AGunBaseClass::StartFire()
{
	if (CurrentBulletCount <= 0)
		return;


	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AGunBaseClass::Fire, TimeBetweenShots, bFullyAutomatic, FirstDelay);
}

void AGunBaseClass::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}


void AGunBaseClass::Fire()
{

}

#pragma endregion

#pragma region Playing Fire Effect

void AGunBaseClass::PlayFireEffect(FVector TracerEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

	if (TracerEffect)
	{
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

#pragma endregion
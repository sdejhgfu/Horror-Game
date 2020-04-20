// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBaseClass.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;

//TODO Set up Blueprint Base for artists and double check to make sure everything works with a gun
UCLASS()
class HORROR_API AGunBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBaseClass();
	
	void StartFire();

	void StopFire();

	bool CanReload() const;

	int GetCurrentBulletCount() const;

	int GetTotalBulletCount() const;

	void Reload();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Reloaded();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool bFullyAutomatic = true;

	/*RPM - Bullets per minute fired by weapon*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float RateOfFire = 1;
	
	//derived from RateOfFire
	float TimeBetweenShots;

	float LastFireTime;

	FTimerHandle TimerHandle_TimeBetweenShots;

	virtual void Fire();

	void PlayFireEffect(FVector TracerEndPoint);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
		USkeletalMeshComponent*	MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BaseDamage = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BonusDamageMultiplier = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	/*Effect for when objects are hit*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* ImpactEffect;

	/*Effect for when bodies are hit*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	/*Bullet Spread in Degrees*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.f))
		float BulletSpread = 5;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	/*Guns Range for Dmg to be applied*/
	UPROPERTY(EditDefaultsOnly, Category = "Sight")
		float SightRange = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		int MagazineSize = 30;

	/*Weapons Max Ammo Capacity excluding clip*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		int MaxAmmoAmount = 100;

	/*Weapons Current Ammo amount excluding current clip*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int AmmoAmount;

	int CurrentClipBulletCount;

	FTimerHandle TimerHandle_ReloadTime;

	bool bCanReload;

	bool bIsReloading;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float ReloadTime = 1.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

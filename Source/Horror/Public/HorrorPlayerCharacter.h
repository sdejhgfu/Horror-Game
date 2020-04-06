// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HorrorPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class HORROR_API AHorrorPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHorrorPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveSideways(float Value);

	void BeginCrouch();
	void EndCrouch();

	void CharacterJump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV = 65.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1f, ClampMax = 100))
		float ZoomInterpSpeed = 20.f;

	float DefaultFOV;

	bool bWantsToZoom;

	void BeginZoom();

	void EndZoom();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StartAttack();

	UFUNCTION(BlueprintCallable, Category = "Player")
		void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Player")
		int GetBulletCount() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
		void Reload();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.


//#include "HorrorPlayerCharacter.h
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"
#include "Engine/World.h"
#include "..\Public\HorrorPlayerCharacter.h"

// Sets default values
AHorrorPlayerCharacter::AHorrorPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArmComp);

	

	ZoomedFOV = 65.f;
	ZoomInterpSpeed = 20.f;
}

// Called when the game starts or when spawned
void AHorrorPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHorrorPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHorrorPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHorrorPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveSideways", this, &AHorrorPlayerCharacter::MoveSideways);
	PlayerInputComponent->BindAxis("LookUp", this, &AHorrorPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &AHorrorPlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AHorrorPlayerCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AHorrorPlayerCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHorrorPlayerCharacter::CharacterJump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AHorrorPlayerCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AHorrorPlayerCharacter::EndZoom);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHorrorPlayerCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AHorrorPlayerCharacter::StopAttack);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AHorrorPlayerCharacter::Reload);
}

#pragma region Jumping

void AHorrorPlayerCharacter::CharacterJump()
{
	if (CanJump())
	{
		Jump();
	}
}

#pragma endregion

#pragma region Crouching

void AHorrorPlayerCharacter::BeginCrouch()
{
	Crouch();
}

void AHorrorPlayerCharacter::EndCrouch()
{
	UnCrouch();
}

#pragma endregion

#pragma region Forward/Strafe Movement

void AHorrorPlayerCharacter::MoveSideways(float Value)
{
	AddMovementInput(GetActorRightVector()*Value);
}

void AHorrorPlayerCharacter
::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector()*Value);
}

#pragma endregion

#pragma region ZoomFunctions

void AHorrorPlayerCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void AHorrorPlayerCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void AHorrorPlayerCharacter::StartAttack()
{
}

void AHorrorPlayerCharacter::StopAttack()
{
}

#pragma endregion

int AHorrorPlayerCharacter::GetBulletCount() const
{
//	return CurrentWeapon->GetCurrentBulletCount();
	return 0;
}

void AHorrorPlayerCharacter::Reload()
{
}

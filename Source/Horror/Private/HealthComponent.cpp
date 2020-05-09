// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Public/HorrorGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth;
	// ...
	AActor* Owner = GetOwner();

	bIsDead = false;
	
	//Assign the delegate to taking dmg
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamage);
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::Heal(float HealAmount)
{
	if (Health <= 0.f || HealAmount <= 0.f)
	{
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}

void UHealthComponent::HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBody, AActor* DamageCauser)
{
	if (Damage <= 0 || bIsDead)
		return;

	//check to see if they are on the same team
	if (IsFriendly(DamagedActor, DamageCauser) && DamageCauser != DamagedActor)
		return;


	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);


	bIsDead = Health <= 0.f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBody, DamageCauser);
	if (bIsDead)
	{
		bIsDead = true;

		
		AHorrorGameMode* HGM = Cast<AHorrorGameMode>(GetWorld()->GetAuthGameMode());
		if (HGM)
		{
			HGM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, DamageCauser->GetInstigatorController());
		}
	}

}

bool UHealthComponent::IsFriendly(AActor * ActorA, AActor * ActorB)
{
	//assume enemy
	if (ActorA == nullptr || ActorB == nullptr)
		return false;


	UHealthComponent*  HealthCompA = Cast<UHealthComponent>(ActorA->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent*  HealthCompB = Cast<UHealthComponent>(ActorB->GetComponentByClass(UHealthComponent::StaticClass()));

	//assume enemy
	if (HealthCompA == nullptr || HealthCompB == nullptr)
		return false;

	return 	HealthCompA->TeamNum == HealthCompB->TeamNum;

}

float UHealthComponent::GetHealth() const
{
	return Health;
}
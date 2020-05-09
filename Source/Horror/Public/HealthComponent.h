// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AActor;
//OnHealthChanged Event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBody, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORROR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	float GetHealth() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
		uint8 TeamNum = 255;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Component")
		static bool IsFriendly(AActor* ActorA, AActor* ActorB);

	//event to happen when 
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnHealthChangedSignature OnHealthChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
		void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBody, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
		float Health = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
		float MaxHealth = 100;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
		void Heal(float HealAmount);

	bool bIsDead;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Nightmare_BaseClass.generated.h"

UCLASS()
class DREAMDEFENDER_API ANightmare_BaseClass : public ACharacter
{
	GENERATED_BODY()
	
public:	
	//Variable pour initialiser la vie du cote blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float fBaseHealth;
	

	// Sets default values for this actor's properties
	ANightmare_BaseClass();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Set la vie actuelle du cauchemard
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void SetCurrentHealth(float health){ fHealth = health; };

	//Get la vie actuelle du cauchemard
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		float GetCurrentHealth(){ return fHealth; };

	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void ReceiveDamage(float damageTaken, float lightDamageFrequency);

	UFUNCTION(BlueprintImplementableEvent, Category = "Switch Function")
		void Die();

	UFUNCTION(BlueprintImplementableEvent, Category = "Switch Function")
		void UpdateTransparency(float lightDamageFrequency);


private:
	
	//Vie du cauchemard
	UPROPERTY()
		float fHealth;
	
	
	
};

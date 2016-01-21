// Fill out your copyright notice in the Description page of Project Settings.

#include "DreamDefender.h"
#include "Nightmare_BaseClass.h"


// Sets default values
ANightmare_BaseClass::ANightmare_BaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANightmare_BaseClass::BeginPlay()
{
	Super::BeginPlay();
	fHealth = fBaseHealth;
	
}

// Called every frame
void ANightmare_BaseClass::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ANightmare_BaseClass::ReceiveDamage(float damageTaken, float lightDamageFrequency){
	this->SetCurrentHealth(fHealth - damageTaken);
	if (fHealth <= 0){
		this->Die();
	}
	UpdateTransparency(lightDamageFrequency);
}


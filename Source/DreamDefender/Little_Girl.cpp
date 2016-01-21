// Fill out your copyright notice in the Description page of Project Settings.

#include "DreamDefender.h"
#include "Little_Girl.h"


// Sets default values
ALittle_Girl::ALittle_Girl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALittle_Girl::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALittle_Girl::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


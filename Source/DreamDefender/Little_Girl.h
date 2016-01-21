// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Little_Girl.generated.h"

UCLASS()
class DREAMDEFENDER_API ALittle_Girl : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALittle_Girl();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

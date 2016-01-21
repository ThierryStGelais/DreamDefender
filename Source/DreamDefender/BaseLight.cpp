// Fill out your copyright notice in the Description page of Project Settings.

#include "DreamDefender.h"
#include "BaseLight.h"
#include "Nightmare_BaseClass.h"
//#include "Engine.h"

// Sets default values
ABaseLight::ABaseLight(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	Super::PostInitializeComponents();

	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, "PointLight");
	RootComponent = PointLight;
	PointLight->SetVisibility(false);

	SpotLight = ObjectInitializer.CreateDefaultSubobject<USpotLightComponent>(this, "SpotLight");
	//RootComponent = SpotLight;
	SpotLight->AttachTo(RootComponent, NAME_None, EAttachLocation::KeepRelativeOffset, false);
	SpotLight->SetVisibility(false);


	HitDetectionMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "HitDetectionMesh");
	HitDetectionMesh->AttachTo(SpotLight,NAME_None, EAttachLocation::KeepRelativeOffset, false);

	HitDetectionMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseLight::OnOverlapBegin);
	HitDetectionMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseLight::OnOverlapEnd);


	
}

ABaseLight::ABaseLight(class UStaticMeshComponent* cone, class USpotLightComponent* spotlight)
{
	SpotLight = spotlight;
	HitDetectionMesh = cone;

}

// Called when the game starts or when spawned
void ABaseLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseLight::Tick( float DeltaTime )
{	
	Super::Tick( DeltaTime );

}


//Lorsqu'un acteur entre dans le cone de detection
void ABaseLight::OnOverlapEnd_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANightmare_BaseClass* enemy = Cast<ANightmare_BaseClass>(OtherActor);
	//Si ce n'est pas un ennemi
	if (enemy == NULL){
		
	}
	//Si c'est un ennemi
	else{
		RemoveEnemyOfArrays(enemy);
	}
}


//Lorsqu'un acteur entre dans cone de detection
void ABaseLight::OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("On actor begin overlap!"));
	ANightmare_BaseClass* enemy = Cast<ANightmare_BaseClass>(OtherActor);
	//Si ce n'est pas un ennemi
	if (enemy == NULL){
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("C'est pas un ennemi!"));
	}
	//Si c'est un ennemi
	else{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy in collision zone!"));
		AddEnemyToArrays(enemy);
	}
}

//Ajoute un ennemi aux deux tableaux
void ABaseLight::AddEnemyToArrays(AActor* actor){
	ArrayOfEnemies.Push(actor);
	ArrayOfActorsExclusion.Push(actor);
}

//Enleve un ennemi aux deux tableaux
void ABaseLight::RemoveEnemyOfArrays(AActor* actor){
	ArrayOfEnemies.Remove(actor);
	ArrayOfActorsExclusion.Remove(actor);
}

//Update des ennemis en collision avec la lumiere
void ABaseLight::EnemiesInLightUpdate(){
	bool doSound = false;
	bool enemyEvent = false;
	FVector enemyPosition;
	//si la lumiere est allumee
	if (bIsOn){
		if (ArrayOfEnemies.Num() > 0){
			for (int i = 0; i < ArrayOfEnemies.Num(); i++){

				if (ArrayOfEnemies[i] != NULL){
					enemyEvent = CheckEnemyLineOfSight(ArrayOfEnemies[i]);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, enemyEvent ? TEXT("true") : TEXT("false"));
				}

				//On veut juste faire un son!!
				if (doSound == false){
					doSound = enemyEvent;
				}

				if (enemyEvent){
					ANightmare_BaseClass* enemy = Cast<ANightmare_BaseClass>(ArrayOfEnemies[i]);
					enemy->ReceiveDamage(fDamageValue, fDamageTime);
					if (doSound && !HadSoundPlayedInLastCheck){
						PlaySoundOnNightmaresHit(enemy);
						HadSoundPlayedInLastCheck = true;
					}
					else{
						HadSoundPlayedInLastCheck = false;
					}
					
				}
			}
			
		}
		else if (HadSoundPlayedInLastCheck){
			StopSoundOnNightmaresExit();
			HadSoundPlayedInLastCheck = false;
		}
	}

}

//Verifie le lineOfSight entre le point d'origine de la lumiere et la cible
bool ABaseLight::CheckEnemyLineOfSight(AActor* actor){

	const FName LightTraceDebug("LightTrace");
	GetWorld()->DebugDrawTraceTag = LightTraceDebug;
	FVector start = this->GetActorLocation();
	FVector finish = actor->GetActorLocation();
	FHitResult hitResult;
	FCollisionQueryParams queryParams;
	FCollisionResponseParams responseParams;

	queryParams.AddIgnoredActors(ArrayOfActorsExclusion);

	// EFFECTUE LES TRACES DANS LE JEU (DEBUG)
	//queryParams.TraceTag = LightTraceDebug;
	
	


	//On otient l'origine du mesh l'ennemi dans le monde et ses points max/mins du mesh dans box extent
	ANightmare_BaseClass* enemy = Cast<ANightmare_BaseClass>(actor);

	if (enemy!=NULL)
	{
		FVector BoxExtent = enemy->GetMesh()->Bounds.BoxExtent;
		FVector Origin = enemy->GetMesh()->Bounds.Origin;

		//Point du mesh dans le monde à l'extrémité haut et bas
		FVector finishUp = Origin + FVector(0.0f, 0.0f, BoxExtent.Z);
		FVector finishDown = Origin + FVector(0.0f, 0.0f, -BoxExtent.Z);

		//Point du mesh dans le monde à l'extrémité avant et derrière
		FVector finishForward = Origin + FVector(-BoxExtent.X, 0.0f, 0.0f);
		FVector finishBackward = Origin + FVector(BoxExtent.X, 0.0f, 0.0f);

		//Point du mesh dans le monde à l'extrémité gauche et droite
		FVector finishLeft = Origin + FVector(0.0f, -BoxExtent.Y, 0.0f);
		FVector finishRight = Origin + FVector(0.0f, BoxExtent.Y, 0.0f);

		//Raycast vers l'origine
		bool isOriginBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, Origin, ECC_WorldStatic, queryParams, responseParams);

		//Raycast vers le dessus du mesh et le bas du mesh
		bool isUpBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, finishUp, ECC_WorldStatic, queryParams, responseParams);
		bool isDownBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, finishDown, ECC_WorldStatic, queryParams, responseParams);

		//Raycast vers l'avant du mesh et l'arrière
		bool isForwardBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, finishForward, ECC_WorldStatic, queryParams, responseParams);
		bool isBackWardBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, finishBackward, ECC_WorldStatic, queryParams, responseParams);

		//Raycast vers la gauche et la droite du mesh
		bool isLeftBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, finishLeft, ECC_WorldStatic, queryParams, responseParams);
		bool isRightBlocked = GetWorld()->LineTraceSingleByChannel(hitResult, start, finishRight, ECC_WorldStatic, queryParams, responseParams);



		//Si un des raycasts n'est pas bloqué, alors on fait des dégâts
		if (!isOriginBlocked || !isUpBlocked || !isDownBlocked || !isForwardBlocked || !isBackWardBlocked || !isLeftBlocked || !isRightBlocked){
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ray Passed!"));
			return true;
			
		}
		else{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ray Blocked!"));
			return false;
			
		}
	}
	return false;

}

void ABaseLight::PrintEnemiesInArray(){
	if (ArrayOfEnemies.Num() >= 1){
		for (int i = 0; i < ArrayOfEnemies.Num(); i++){
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("J'ai un ennemi dans mon Array"));
		}
	}
	else{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("J'ai pas d'ennemi dans mon Array"));
	}
}

void ABaseLight::PrintArrayOfActorsExclusion(){
	if (ArrayOfActorsExclusion.Num() >= 1){
		for (int i = 0; i < ArrayOfActorsExclusion.Num(); i++){
			if (ArrayOfActorsExclusion[i] != NULL){
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ArrayOfActorsExclusion[i]->GetName());
			}
		}
	}
	else{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Il n'y rien dans l'array d'exclusion"));
	}
}

void ABaseLight::ToggleEquipToActor(AActor * actor){
	if (bIsEquipped){
		bIsEquipped = !bIsEquipped;
		ArrayOfActorsExclusion.Remove(actor);
	}
	else{
		bIsEquipped = !bIsEquipped;
		ArrayOfActorsExclusion.Add(actor);
	}
}

void ABaseLight::ToggleLight(){
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ToggleLight est calle"));
	bIsOn = !bIsOn;
	if (bIsOn) 
	{ 
		HitDetectionMesh->SetVisibility(true);
		if (SpotLight->IsVisible()){
			SpotLight->SetIntensity(fLightIntensityOn);
			
		}
		else{
			PointLight->SetIntensity(fLightIntensityOn);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ToggleLight ON"));
	}
	else{ 
		HitDetectionMesh->SetVisibility(false);
		if (SpotLight->IsVisible()){
			SpotLight->SetIntensity(fLightIntensityOff);
		}
		else{
			PointLight->SetIntensity(fLightIntensityOff);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ToggleLight OFF"));

	}
}


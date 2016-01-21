// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "BaseLight.generated.h"

UCLASS()
class DREAMDEFENDER_API ABaseLight : public AActor
{
	GENERATED_BODY()
		
	
public:	




	//Lumiere de type SpotLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Components")
	class USpotLightComponent* SpotLight;

	//Lumiere de type SpotLight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Components")
	class UPointLightComponent* PointLight;



	//Cone de hit detection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch Components")
	class UStaticMeshComponent* HitDetectionMesh;

	


	// Sets default values for this actor's properties
	ABaseLight(const FObjectInitializer& ObjectInitializer);

	//Constructeur de mesh custom avec lumiere custom
	//UFUNCTION(BlueprintCallable, Category = "Constructeur BaseLight")
	ABaseLight( class UStaticMeshComponent* cone,  class USpotLightComponent* spotlight);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	

	UFUNCTION(BlueprintNativeEvent, Category = "Switch Functions")
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		void OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Functions")
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		void OnOverlapEnd_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	//Verifie le line of sight de la lumiere
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		bool CheckEnemyLineOfSight(AActor * actor);

	//On verifie s'il y a des ennemis dans la lumiere et on assigne les degats
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void EnemiesInLightUpdate();

	//Toggle la lumière entre allumé ou éteint
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void ToggleLight();

	
	//On allume la lumière
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void SetLightOn(){ bIsOn = true; SpotLight->Intensity = fLightIntensityOn; PointLight->Intensity = fLightIntensityOn; };

	//On éteint la lumière
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void SetLightOff(){ bIsOn = false; SpotLight->Intensity = fLightIntensityOff; PointLight->Intensity = fLightIntensityOff; };



	/* DEBUT GET/SET  */
	//Get isLightOn
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		bool GetLightOn(){ return bIsOn; };


	//Set intensité de la lumière
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void SetLightIntensity(float intensity){ fLightIntensityOn = intensity;};

	//Get intensité de la lumière
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		float GetLightIntensity(){ return SpotLight->Intensity; };

	//Dégât de lumière par cycle de la boucle check all enemies
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void SetDamageValue(float damage){ fDamageValue = damage;};

	//Get damage value
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		float GetDamageValue(){ return fDamageValue; };

	//fréquence des dégâts sur les ennemis en secondes
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void SetDamageTime(float time){ fDamageTime = time; }

	//Retourne la fréquence des dégâts sur les ennemis en secondes
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		float GetDamageTime(){ return fDamageTime; };


	/*FIN  GET/SET */




	//Ajoute l'ennemi aux tableaux
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void AddEnemyToArrays(AActor * actor);
	//Enleve l'ennemi aux tableaux
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void RemoveEnemyOfArrays(AActor * actor);

	//Fonction de debuggage montrant si on a des ennemis dans ArrayOfEnemies lorsqu'on serait suppose en avoir
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void PrintEnemiesInArray();

	//Fonction de debuggage qui print l'array des acteurs à exclure
	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void PrintArrayOfActorsExclusion();

	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void ToggleEquipToActor(AActor * actor);

	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void AddParentActorToIgnore(AActor * actor){
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Acteur a ignoré:"+actor->GetName());
			ArrayOfActorsExclusion.Push(actor);
	};

	UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		void RemoveParentActorToIgnore(AActor * actor){
		ArrayOfActorsExclusion.Remove(actor);
	};

	//Évènement qui joue un son quand un ou plusieurs ennemis sont touchés par la lumière
	UFUNCTION(BlueprintImplementableEvent, Category = "Switch Function")
		void PlaySoundOnNightmaresHit(AActor* NightmareRef);

	UFUNCTION(BlueprintImplementableEvent, Category = "Switch Function")
		void StopSoundOnNightmaresExit();
	




private:
	//Est-ce qu'on a joué un son lord du dernier checkenemylineofsight ?
	bool HadSoundPlayedInLastCheck = false;
	//Tableau d'ennemis qui seront à traiter dans la zone de dégâts de la lumière
	TArray<AActor*> ArrayOfEnemies;
	//Tableau d'ennemis et d'objets ne bloquant pas les raycasts ex: un ennemi derrière un autre ne bloquera pas un raycast
	TArray<AActor*> ArrayOfActorsExclusion;


	//Intensité lumiere
	UPROPERTY()
		float fLightIntensityOn = 5000.0f;
	UPROPERTY()
		float fLightIntensityOff = 0.0f;



	//La lumiere est-elle allume?

	UPROPERTY()
		bool bIsOn = true;

	//Est-ce que la lumière est équipé?
	UPROPERTY()
		bool bIsEquipped = false;


	//Float qui représente les degats effectuer par la lumiere
	UPROPERTY()
		float fDamageValue = 1.0f;

	//Float qui représente le temps en seconde de la fréquence des dégâts
	UPROPERTY()
		float fDamageTime = 1.0f;
	

	
	

};

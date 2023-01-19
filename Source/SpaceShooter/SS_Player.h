// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Components/AudioComponent.h"
#include "SS_Player.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASS_Player();


	USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ParticleSystems;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ExplosionFX;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ShipMesh;
	
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent* DeathExplosionSound;
	
	// Display Settings


	UPROPERTY(EditAnywhere)
		float Field_Width;

	UPROPERTY(EditAnywhere)
		float Field_Height;

	// Player Stats

	UPROPERTY(BlueprintReadOnly)
		float Max_Health;

	UPROPERTY(BlueprintReadOnly)
		float Current_Health;

	UPROPERTY(BlueprintReadOnly)
		float Max_Armor;

	UPROPERTY(BlueprintReadOnly)
		float Current_Armor;

	UPROPERTY(EditAnywhere)
		float Max_Velocity;

	UPROPERTY(BlueprintReadOnly)
		float PlayerScore;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> BP_WeaponProjectile;

	FVector Current_Location;
	FRotator Current_Rotation;
	FVector New_Location;

	bool bIsFiring;
	float WeaponFireRate;
	float TimeSinceLastShot;

	bool bHit;
	bool bDead;

	float Current_X_Velocity;
	float Current_Y_Velocity;


	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);
	
	
	// Player Atttack
	void FireWeapon();
	void StartFiring();
	void StopFiring();
	void CollectablePickUp();

	//Functions
	UFUNCTION()
		void OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Enemy.h"
#include "Hazard.h"



#include "SS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ASS_GameMode : public AGameMode
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> Enemy;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AHazard> HazardTemplate;


	UPROPERTY(EditAnywhere)
		UAudioComponent* MusicTrack;

	//constructor
	
	ASS_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<AHazard> GetHazardTemplate; // ()

public:

	int32 SpawnTimer;
	bool bPlayerDead;

	UFUNCTION()
		void RestartLevel(FName LevelName);

	bool bStart;

protected:

	FActorSpawnParameters AsteroidsSpawnParameters;
	FVector AsteroidSpawnLocation;
	FRotator AsteroidSpawnRotation;
	UWorld* ThisWorld;


};
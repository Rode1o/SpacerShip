// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_GameMode.h"


ASS_GameMode::ASS_GameMode() {


	SpawnTimer = 0.f;
	MusicTrack = CreateDefaultSubobject<UAudioComponent>(TEXT("Music"));
	MusicTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	AsteroidSpawnLocation = FVector(1000.f, 1000.f, 1000.f);
	AsteroidSpawnRotation = FRotator(0.f, 0.f, 0.f);
	ThisWorld = GetWorld();


}

void ASS_GameMode::BeginPlay()
{

	Super::BeginPlay();

	MusicTrack->Play();

	bPlayerDead = false;


}

void ASS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SpawnTimer = FMath::RandRange(0, 1000);

	if (Enemy) {
		if (SpawnTimer > 990) {
			FVector EnemySpawnLocation(0.f, 0.f, 0.f);
			FRotator EnemySpawnRotator(0.f, 0.f, 0.f);

			AActor* NewEnemy = GetWorld()->SpawnActor(Enemy, &EnemySpawnLocation, &EnemySpawnRotator, AsteroidsSpawnParameters);

			if (NewEnemy)
				NewEnemy->SetActorLocation(FVector(1000.f, FMath::RandRange(-400, 400), 0.f));
		}
	}

	// Spawn Enemy

	if (HazardTemplate) {
		if (SpawnTimer > 995) {
			AActor* NewHazard = GetWorld()->SpawnActor(HazardTemplate, &AsteroidSpawnLocation, &AsteroidSpawnRotation, AsteroidsSpawnParameters);

			if (NewHazard) {
				NewHazard->SetActorLocation(FVector(-500.f, -500.f, -500.f));
			}
		}
	}

}

void ASS_GameMode::RestartLevel(FName LevelName)
{
}

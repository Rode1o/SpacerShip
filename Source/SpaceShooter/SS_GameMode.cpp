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

	if (Score_Widget_Class != nullptr) {

		Score_Widget = CreateWidget<UUserWidget>(GetWorld(), Score_Widget_Class);
		Score_Widget->AddToViewport();

	}

	if (Shield_And_Armor_Widget_Class != nullptr) {

		Shield_Armor_Widget = CreateWidget<UUserWidget>(GetWorld(), Shield_And_Armor_Widget_Class);
		Shield_Armor_Widget->AddToViewport();

	}
	if (Restart_Widget_Class != nullptr) {

		Restart_Widget = CreateWidget<UUserWidget>(GetWorld(), Restart_Widget_Class);
		Restart_Widget->AddToViewport();

		Restart_Widget->SetVisibility(ESlateVisibility::Hidden);
	}

	PC_Ref = GetWorld()->GetFirstPlayerController();
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

	if (bPlayerDead) {
		Restart_Widget->SetVisibility(ESlateVisibility::Visible);
		PC_Ref->bShowMouseCursor = true;
		bPlayerDead = false;
	}

}

void ASS_GameMode::RestartLevel(FName LevelName)
{
}

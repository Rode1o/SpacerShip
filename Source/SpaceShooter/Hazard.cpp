// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"

// Sets default values
AHazard::AHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	AsteroidExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AsteroidExplosionFX"));
	AsteroidExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AsteroidExplosionSound"));


	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);



	Spawn_X_Velocity = 500.f;
	Spawn_Y_Velocity = 0.f;
	SelfDestructTimer = 1.f;

	bHit = false;


	
}



// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();

	Initial_Rotation = 0.f;

	Initial_X_Location = 1500.f;
	Initial_Y_Location = FMath::RandRange(-400, 400);

	RandSize = FMath::RandRange(3, 8);


	this->SetActorScale3D(FVector(RandSize, RandSize, RandSize));

	AsteroidExplosionSound->Deactivate();
	AsteroidExplosionSound->bStopWhenOwnerDestroyed = false;

	AsteroidExplosionFX->Deactivate();

	OnActorBeginOverlap.AddDynamic(this, &AHazard::OnBeginOverlap);

	
}

// Called every frame
void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(this != nullptr);

	Initial_Rotation += DeltaTime;
	Initial_X_Location += DeltaTime * Spawn_X_Velocity;
	Initial_Y_Location += DeltaTime * Spawn_Y_Velocity;

	this->SetActorRotation(FRotator(Initial_Rotation * 100.f, Initial_Rotation* 50.f, 0.f));
	this->SetActorLocation(FVector(Initial_X_Location, Initial_Y_Location, 0.f));






}

void AHazard::OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor)
{
}
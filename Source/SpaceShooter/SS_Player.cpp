// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Player.h"
#include "SS_GameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASS_Player::ASS_Player()
{
	
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));

	ParticleSystems = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	DeathExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));


	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); 
	ParticleSystems->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	/// Player STATS
	
	Max_Velocity = 250.f;
	Current_X_Velocity = 0.f;
	Current_Y_Velocity = 0.f;

	bIsFiring = false;
	WeaponFireRate = 0.25f;
	TimeSinceLastShot = 0.f;

	PlayerScore = 0.f;
}






// Called when the game starts or when spawned
void ASS_Player::BeginPlay()
{
	Super::BeginPlay();

	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();

	bHit = false;
	bDead = false;

	// START STATS
	ExplosionFX->Deactivate();
	DeathExplosionSound->Deactivate();
	Max_Health = 100.f;
	Current_Health = 100.f;
	Max_Armor = 100.f;
	Current_Armor = 100.f;

	OnActorBeginOverlap.AddDynamic(this, &ASS_Player::OnBeginOverlap);

	
}

// Called every frame
void ASS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Current_X_Velocity != 0 || Current_Y_Velocity != 0) {

		New_Location = FVector(Current_Location.X + (Current_X_Velocity * DeltaTime),
			Current_Location.Y + (Current_Y_Velocity * DeltaTime), 0);

		this->SetActorLocation(New_Location);
		Current_Location = New_Location;
	}

	if (Current_Y_Velocity > 0.1f) {

		this->SetActorRotation(FRotator(Current_Rotation.Pitch + 45.f, Current_Rotation.Yaw, Current_Rotation.Roll));

	}
	else if ((Current_Y_Velocity < -0.1f)){

		this->SetActorRotation(FRotator(Current_Rotation.Pitch - 45.f, Current_Rotation.Yaw, Current_Rotation.Roll));
	}
	else {
		this->SetActorRotation(FRotator(Current_Rotation));
	}


	// Check Bounders 

	if (this->GetActorLocation().X > Field_Width) {

		Current_Location = FVector(Field_Width - 1, Current_Location.Y, Current_Location.Z);
	}

	if (this->GetActorLocation().X < -Field_Width) {

		Current_Location = FVector(-Field_Width + 1, Current_Location.Y, Current_Location.Z);
	}

	if (this->GetActorLocation().Y > Field_Height) {

		Current_Location = FVector(Current_Location.X, Field_Height - 1, Current_Location.Z);
	}

	if (this->GetActorLocation().Y < -Field_Height) {

		Current_Location = FVector(Current_Location.X, -Field_Height + 1, Current_Location.Z);
	}


	if (bIsFiring) {
		if (TimeSinceLastShot > WeaponFireRate) {
			FireWeapon();
			TimeSinceLastShot = 0.f;
		}
	}

	TimeSinceLastShot += DeltaTime;
	// handle player hit

	if (bHit) {
		bDead = true;
		this->ShipMesh->SetVisibility(false);
		this->ParticleSystems->SetVisibility(false);
		this->ExplosionFX->Activate();
		this->DeathExplosionSound->Activate();
		this->SetActorTickEnabled(false);


		ASS_GameMode* GameModeRef = Cast<ASS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameModeRef->bPlayerDead = true;
	}


}

// Called to bind functionality to input
void ASS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASS_Player::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASS_Player::MoveUp);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASS_Player::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASS_Player::StopFiring);

}


void ASS_Player::FireWeapon()
{
	FActorSpawnParameters Params = {};
	Params.Owner = this;

	AActor* SpawnedProjectile = GetWorld()->SpawnActor(BP_WeaponProjectile, &Current_Location, &Current_Rotation, Params);

}

void ASS_Player::StartFiring()
{

	bIsFiring = true;

}

void ASS_Player::StopFiring()
{
	bIsFiring = false;

}


void ASS_Player::MoveRight(float AxisValue)
{
	Current_X_Velocity = Max_Velocity * AxisValue;


}

void ASS_Player::MoveUp(float AxisValue)
{
	Current_Y_Velocity = Max_Velocity * AxisValue;
}


void ASS_Player::CollectablePickUp()
{
	if (Current_Armor < 100.f && Current_Health == 100.f) {
		Current_Armor += 10.f;

		if (Current_Armor > 100.f)
			Current_Armor = 100.f;
	}
	else if (Current_Health < 100.f){

		Current_Health += 10.f;

		if (Current_Health > 100.f)
			Current_Health = 100.f;

	}


}

void ASS_Player::OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Asteroid") || OtherActor->ActorHasTag("EnemyShip")) {

		if (Current_Armor > 0.f) {
			Current_Armor -= 10.f;

			if (Current_Armor < 0.f)
				Current_Armor = 0.f;
		} else if (Current_Health > 0.f) {
			Current_Health -= 10.f;

			if (Current_Health <= 0.f){
				Current_Health = 0.f;
				bHit = true;

			}
		}

	}



	if (OtherActor->ActorHasTag("EnemyProjectile")) {

		if (Current_Armor > 0.f) {
			Current_Armor -= 25.f;

			if (Current_Armor < 0.f)
				Current_Armor = 0.f;
		}
		else if (Current_Health > 0.f) {
			Current_Health -= 25.f;

			if (Current_Health <= 0.f) {
				Current_Health = 0.f;
				bHit = true;

			}
		}

	}

}
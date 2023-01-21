// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "SS_Player.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));


	PlaneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


	ProjectileSpeed = 1000.f;
	bHit = false;
	bDestroy = false;
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CurrentLocation = this->GetActorLocation();

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = FVector(CurrentLocation.X + (ProjectileSpeed * DeltaTime), CurrentLocation.Y, CurrentLocation.Z);

	this->SetActorLocation(NewLocation);
	CurrentLocation = NewLocation;


	if (bHit) {
		this->Destroy();
	}

//
	//if (CurrentLocation.X > PlayerRef->Field_Width ) {
	//
		//this->Destroy();
	//}
}

void AProjectile::OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor)
{

	// if collide with other actor
	if (OtherActor) {
		ASS_Player* OwningPlayer = Cast<ASS_Player>(this->GetOwner());
		if (OtherActor->ActorHasTag("Bounds") || OtherActor->ActorHasTag("EndPoint")) {

			bHit = true;

		}
		if (this->ActorHasTag("EnemyProjectile") && OtherActor->ActorHasTag("Player"))
			bHit = true;

		if (OtherActor->ActorHasTag("EnemyShip")) {
			if (OwningPlayer) {
				OwningPlayer->PlayerScore += 50.f;
				bHit = true;
			}
		}

		if (OtherActor->ActorHasTag("Asteroid")) {
			if (OwningPlayer) {
				OwningPlayer->PlayerScore += 10.f;
				bHit = true;
			}
		}

	}



}

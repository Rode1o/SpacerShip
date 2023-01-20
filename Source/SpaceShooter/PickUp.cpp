// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));


	CollisionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}



// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APickUp::OnBeginOverlap);



}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	this->SetActorLocation(FVector(this->GetActorLocation().X - (DeltaTime * 100.f), this->GetActorLocation().Y, 0.f));



}

void APickUp::OnBeginOverlap(AActor* PickUpActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {

		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->CollectablePickUp();

		this->Destroy();

	}

	if (OtherActor->ActorHasTag("Bounds")) {

		this->Destroy();
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowOrb.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInfo.h"
#include <random>
#include "Assignment/AssignmentCharacter.h"

// Sets default values
AThrowOrb::AThrowOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->InitSphereRadius(15.0f);
	RootComponent = sphereComp;
	sphereComp->SetSimulatePhysics(true);
	sphereComp->SetCollisionProfileName(TEXT("PhysicsActor"));
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

// Called when the game starts or when spawned
void AThrowOrb::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentHit.AddDynamic(this, &AThrowOrb::OnHit);
	originalResponse = sphereComp->GetCollisionResponseToChannel(ECC_Pawn);
	
}

// Called every frame
void AThrowOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowOrb::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	ECollisionResponse response = sphereComp->GetCollisionResponseToChannel(ECC_Pawn);
	if (response == ECR_Ignore)
	{
		sphereComp->SetCollisionResponseToChannel(ECC_Pawn, originalResponse);
	}

	AAssignmentCharacter* player = Cast<AAssignmentCharacter>(otherActor);
	if (player)
	{
		if (collectSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, collectSound, GetActorLocation());
		}
		player->OnGrab(this, hitComp);
		player->health -= healthPenalty;
	}
}


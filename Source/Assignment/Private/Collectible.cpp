// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInfo.h"
#include <random>
#include "Assignment/AssignmentCharacter.h"


int ACollectible::totalValue = 0;

// Sets default values
ACollectible::ACollectible()
{
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
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	
	sphereComp->OnComponentHit.AddDynamic(this, &ACollectible::OnHit);
	originalResponse = sphereComp->GetCollisionResponseToChannel(ECC_Pawn);

	
	value = FMath::RandRange(1, 5);
	totalValue += value;
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectible::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	AAssignmentCharacter* player = Cast<AAssignmentCharacter>(otherActor);
	if (player)
	{
		if (collectSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, collectSound, GetActorLocation());
		}
		player->health -= value;
		PlayerInfo::GetInstance()->collectibleValue += value;

		Destroy();
	}
}


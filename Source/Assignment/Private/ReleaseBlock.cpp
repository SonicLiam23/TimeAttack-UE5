// Fill out your copyright notice in the Description page of Project Settings.


#include "ReleaseBlock.h"
#include "ThrowOrb.h"
#include "PlayerInfo.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AReleaseBlock::AReleaseBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	// make the mesh the default cube (and ensure it actually loads it)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (cubeMesh.Succeeded())
	{
		mesh->SetStaticMesh(cubeMesh.Object);
	}
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mesh->SetCollisionProfileName(TEXT("BlockAll"));

	// disable movement on hit
	mesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AReleaseBlock::BeginPlay()
{
	Super::BeginPlay();

	mesh->OnComponentHit.AddDynamic(this, &AReleaseBlock::OnHit);
	
}

// Called every frame
void AReleaseBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReleaseBlock::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *otherActor->GetName());
	if (!isActivated)
	{
		AThrowOrb* throwOrb = Cast<AThrowOrb>(otherActor);
		if (throwOrb)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit with throw orb"));
			PlayerInfo::GetInstance()->releaseBlocksHit++;
			isActivated = true;
			UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(activatedMaterial, this);
			mesh->SetMaterial(0, dynamicMaterial);
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "PlayerInfo.h"
#include "Assignment/AssignmentCharacter.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	mesh->OnComponentHit.AddDynamic(this, &ATarget::OnHit);
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	mesh->SetVisibility(false);
	mesh->SetSimulatePhysics(false);

	
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerInfo::GetInstance()->releaseBlocksHit >= PlayerInfo::GetInstance()->releaseBlocksNeeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("fhjdiwhfkjds"));
		mesh->SetVisibility(true);
		mesh->SetSimulatePhysics(true);
		SetActorTickEnabled(false);
	}

}

void ATarget::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	AAssignmentCharacter* player = Cast<AAssignmentCharacter>(otherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("TARGET GET"));
		PlayerInfo::GetInstance()->targetGet = true;
		player->OnGrab(this, hitComp);
	}
}



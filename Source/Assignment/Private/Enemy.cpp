// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "Assignment/AssignmentCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PlayerInfo.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currVelocity = FVector::Zero();
	speed = 100.0f;
	radius = 500.0f;
	distSqrd = BIG_NUMBER;
	backToBase = false;

	aiPercComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightComponent"));
	sightConfig->SightRadius = radius;
	sightConfig->LoseSightRadius = sightConfig->SightRadius + 100.0f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->SetMaxAge(0.5f);

	aiPercComp->ConfigureSense(*sightConfig);
	aiPercComp->SetDominantSense(sightConfig->GetSenseImplementation());
	aiPercComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	collider = GetCapsuleComponent();

	
	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	baseLoc = GetActorLocation();
	collider->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerOverlapBegin);
	collider->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnPlayerOverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!currVelocity.IsZero())
	{
		FVector newLoc = GetActorLocation() + (currVelocity * DeltaTime);
		SetActorLocation(newLoc);
	}

	if (target)
	{
		FVector dir = target->GetActorLocation() - GetActorLocation();
		dir.Z = 0;
		dir = dir.GetSafeNormal();
		
		SetActorRotation(dir.Rotation());

		if (FVector::DistSquared2D(GetActorLocation(), target->GetActorLocation()) > 4000.0f)
		{
			currVelocity = dir * speed;
		}
		else
		{
			currVelocity = FVector::Zero();
		}
	}
	else if (backToBase)
	{
		FVector dir = baseLoc - GetActorLocation();
		dir.Z = 0;
		SetActorRotation(dir.Rotation());
		float distanceSquared = FVector::DistSquared2D(GetActorLocation(), baseLoc);

		if (distanceSquared > 10.0f)
		{
			currVelocity = dir.GetSafeNormal() * speed;
		}
		else
		{
			currVelocity = FVector::ZeroVector;
			backToBase = false;
			distSqrd = BIG_NUMBER;
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnSensed(const TArray<AActor*>& testActors)
{
	for (AActor* actor : testActors)
	{
		FString n = actor->GetName();
		FActorPerceptionBlueprintInfo info;
		aiPercComp->GetActorsPerception(actor, info);

		if (!info.LastSensedStimuli.IsEmpty() && info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			target = actor;
			backToBase = false;
		}
		else
		{
			target = nullptr;
			backToBase = true;
		}
	}
}

void AEnemy::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	AAssignmentCharacter* player = Cast<AAssignmentCharacter>(otherActor);
}

void AEnemy::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!damageTick.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(damageTick, this, &AEnemy::DamagePlayer, 0.2f, true);
		}
	}
}

void AEnemy::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (damageTick.IsValid()) 
	{
		GetWorld()->GetTimerManager().ClearTimer(damageTick);
	}
}

void AEnemy::DamagePlayer()
{
	PlayerInfo::GetInstance()->health -= 5;
}


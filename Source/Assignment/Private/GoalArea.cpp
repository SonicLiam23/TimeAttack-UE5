// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalArea.h"
#include "PlayerInfo.h"
#include "Components/StaticMeshComponent.h"
#include "Assignment/AssignmentCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoalArea::AGoalArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Component"));
	RootComponent = collisionBox;

	collisionBox->OnComponentHit.AddDynamic(this, &AGoalArea::OnHit);
}

// Called when the game starts or when spawned
void AGoalArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoalArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerInfo::GetInstance()->health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LOSE"));
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetWidgetToFocus(nullptr); // Or set this to a specific widget if needed

			PC->SetInputMode(InputMode);
		}
		// LOSE CONDITION
		UGameplayStatics::OpenLevel(this, FName("Menu"));
		PlayerInfo::GetInstance()->SetGameState(EUserPlayStateEnum::LOSE);
	}


}

void AGoalArea::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	AAssignmentCharacter* player = Cast<AAssignmentCharacter>(otherActor);
	if (player)
	{
		if (PlayerInfo::GetInstance()->targetGet && PlayerInfo::GetInstance()->collectibleValue >= PlayerInfo::GetInstance()->collectibleValueNeeded)
		{
			// WIN CONDITION
			UE_LOG(LogTemp, Warning, TEXT("YOU WIN"));
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				PC->bShowMouseCursor = true;

				FInputModeUIOnly InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputMode.SetWidgetToFocus(nullptr); // Or set this to a specific widget if needed

				PC->SetInputMode(InputMode);
			}

			UGameplayStatics::OpenLevel(this, FName("Menu"));
			PlayerInfo::GetInstance()->SetGameState(EUserPlayStateEnum::WIN);
		}
	}

}


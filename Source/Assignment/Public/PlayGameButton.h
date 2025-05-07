// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PlayGameButton.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class ASSIGNMENT_API UPlayGameButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	UFUNCTION()
	void OnPlayClicked();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class UTextBlock; class UCanvasPanel; class UPlayGameButton;

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateInfo();
	UPROPERTY(meta = (BindWidget))
	UTextBlock* infoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* menuText;

	UPROPERTY(meta = (BindWidget))
	UPlayGameButton* playButton;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeConstruct() override;
};

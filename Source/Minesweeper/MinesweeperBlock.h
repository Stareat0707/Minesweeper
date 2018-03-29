// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "ConstructorHelpers.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "MinesweeperBlock.generated.h"

UCLASS()
class MINESWEEPER_API AMinesweeperBlock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinesweeperBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPaperSpriteComponent* Block;

	UTextRenderComponent* MineNum;

	bool bActive;
	bool bFlag;
	bool bMine;
	bool bQuestion;

	bool bLeftMousePressed;
	bool bRightMousePressed;

	int32 NearMineNum;

	UPaperSprite* BlueSprite;

	UPaperSprite* FlagSprite;

	UPaperSprite* WhiteSprite;

	UPaperSprite* GreenSprite;

	TArray<AMinesweeperBlock*> NearBlocks;

	UFUNCTION()
	void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnReleased(UPrimitiveComponent* TouchedComponent, FKey ButtonReleased);

	void AutoDig();

	void Dig();

	void SetFlag();

	void SetNearMineNum();
};

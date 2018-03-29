// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MinesweeperBlock.h"
#include "MinesweeperPawn.h"
#include "MinesweeperPlayerController.h"
#include "UserWidget.h"
#include "MinesweeperGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPER_API AMinesweeperGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMinesweeperGameModeBase();

protected:

	virtual void BeginPlay() override;

public:

	void GameOver(bool bWin, UPaperSpriteComponent* SpriteComponent);

	void IsWin();

	void ReceiveFirstInput(AMinesweeperBlock* Block);

	UFUNCTION(BlueprintCallable)
	void SetDifficulty(int32 Width, int32 Height, int32 MineNum);

	void SetMines(AMinesweeperBlock* Block);

	void SetNearBlocks();

	void SpawnBlocks();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	void StartTimer();

	UFUNCTION()
	void TimeUp();

	bool bFirstClick;
	bool bOnGame;

	UPROPERTY(BlueprintReadOnly)
	bool bWin;

	int32 Width;
	int32 Height;
	int32 MineNum;

	UPROPERTY(BlueprintReadOnly)
	int32 VirtualMineNum;

	UPROPERTY(BlueprintReadOnly)
	int32 PlayMinute;

	UPROPERTY(BlueprintReadOnly)
	int32 PlaySecond;

	FTimerHandle TimerHandle;

	UPaperSprite* MineSprite;

	UUserWidget* HUD;

	UClass* StartMenuClass;
	UClass* InGameClass;
	UClass* GameOverClass;

	TArray<AMinesweeperBlock*> Blocks;
};

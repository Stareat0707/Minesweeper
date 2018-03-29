// Fill out your copyright notice in the Description page of Project Settings.

#include "MinesweeperGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AMinesweeperGameModeBase::AMinesweeperGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerControllerClass = AMinesweeperPlayerController::StaticClass();

	DefaultPawnClass = AMinesweeperPawn::StaticClass();

	bFirstClick = true;
	bOnGame = true;

	MineSprite = ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Sprite/MinesweeperMineSprite")).Object;

	StartMenuClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserWidget/MinesweeperStartMenu.MinesweeperStartMenu_C")).Class;
	InGameClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserWidget/MinesweeperInGame.MinesweeperInGame_C")).Class;
	GameOverClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UserWidget/MinesweeperGameOver.MinesweeperGameOver_C")).Class;
}

void AMinesweeperGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), StartMenuClass);
	HUD->AddToViewport();
}

void AMinesweeperGameModeBase::GameOver(bool bWin, UPaperSpriteComponent* SpriteComponent = NULL)
{
	this->bWin = bWin;

	if (!bWin)
	{
		SpriteComponent->SetSprite(MineSprite);
	}

	CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), GameOverClass)->AddToViewport();

	UKismetSystemLibrary::K2_PauseTimerHandle(this, TimerHandle);

	bOnGame = false;
}

void AMinesweeperGameModeBase::IsWin()
{
	int32 ActiveBlockNum = 0;

	for (AMinesweeperBlock* block : Blocks)
	{
		if (block->bActive)
		{
			++ActiveBlockNum;
		}
	}

	if (Blocks.Num() - MineNum == ActiveBlockNum)
	{
		GameOver(true);
	}
}

void AMinesweeperGameModeBase::ReceiveFirstInput(AMinesweeperBlock* Block)
{
	SetMines(Block);

	SetNearBlocks();

	bFirstClick = false;
}

void AMinesweeperGameModeBase::SetDifficulty(int32 Width, int32 Height, int32 MineNum)
{
	this->Width = Width;

	this->Height = Height;

	this->VirtualMineNum = this->MineNum = MineNum;
}

void AMinesweeperGameModeBase::SetMines(AMinesweeperBlock* Block)
{
	int32 MaxMineNum = MineNum;

	while (MineNum > 0)
	{
		AMinesweeperBlock* Temp = Blocks[UKismetMathLibrary::RandomIntegerInRange(0, Blocks.Num() - 1)];

		if (!(Temp->bMine || (Temp == Block)))
		{
			Temp->bMine = true;
			--MineNum;
		}
	}

	MineNum = MaxMineNum;
}

void AMinesweeperGameModeBase::SetNearBlocks()
{
	for (AMinesweeperBlock* block : Blocks)
	{
		int32 ArrayIndex = Blocks.Find(block);
		for (int32 i = ArrayIndex / Width - 1; i <= ArrayIndex / Width + 1; ++i)
		{
			if (UKismetMathLibrary::InRange_IntInt(i, 0, Height, true, false))
			{
				for (int32 j = ArrayIndex % Width - 1; j <= ArrayIndex % Width + 1; ++j)
				{
					if (UKismetMathLibrary::InRange_IntInt(j, 0, Width, true, false))
					{
						if (UKismetMathLibrary::NotEqual_ObjectObject(Blocks[Width * i + j], block))
						{
							block->NearBlocks.Add(Blocks[Width * i + j]);
						}
					}
				}
			}
		}
	}
}

void AMinesweeperGameModeBase::SpawnBlocks()
{
	for (int i = 0; i < Width * Height; ++i)
	{
		Blocks.Add(GetWorld()->SpawnActor<AMinesweeperBlock>(FVector(i%Width * 100 - (Width - 1) * 50, 5, i / Width * 100 - (Height - 1) * 50), FRotator(0)));
	}
}

void AMinesweeperGameModeBase::StartGame()
{
	SpawnBlocks();

	bOnGame = true;

	HUD->RemoveFromParent();

	HUD = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), InGameClass);
	HUD->AddToViewport();
}

void AMinesweeperGameModeBase::StartTimer()
{
	TimerHandle = UKismetSystemLibrary::K2_SetTimer(this, FString("TimeUp"), 1.0f, true);
}

void AMinesweeperGameModeBase::TimeUp()
{
	if (++PlaySecond == 60)
	{
		++PlayMinute;

		PlaySecond = 0;
	}
}

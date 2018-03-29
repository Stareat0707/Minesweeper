// Fill out your copyright notice in the Description page of Project Settings.

#include "MinesweeperBlock.h"
#include "MinesweeperGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMinesweeperBlock::AMinesweeperBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BlueSprite = ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Sprite/MinesweeperBlueSprite")).Object;
	FlagSprite = ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Sprite/MinesweeperFlagSprite")).Object;
	WhiteSprite = ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Sprite/MinesweeperWhiteSprite")).Object;
	GreenSprite = ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Sprite/MinesweeperGreenSprite")).Object;

	Block = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Block"));
	RootComponent = Block;
	Block->SetSprite(BlueSprite);

	MineNum = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MineNum"));
	MineNum->SetupAttachment(RootComponent);
	MineNum->SetRelativeLocation(FVector(-32.5f, 5.0f, -50.0f));
	MineNum->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	MineNum->SetRelativeScale3D(FVector(7.400269f, 5.0f, 3.846f));
	MineNum->SetText(FText());
	MineNum->SetTextRenderColor(FColor(0, 128, 0));
	MineNum->SetHiddenInGame(true);

	Block->OnClicked.AddDynamic(this, &AMinesweeperBlock::OnClicked);
	Block->OnReleased.AddDynamic(this, &AMinesweeperBlock::OnReleased);
}

// Called when the game starts or when spawned
void AMinesweeperBlock::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinesweeperBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinesweeperBlock::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		bLeftMousePressed = true;
	}
	else if (ButtonPressed == EKeys::RightMouseButton)
	{
		bRightMousePressed = true;
	}
}

void AMinesweeperBlock::OnReleased(UPrimitiveComponent* TouchedComponent, FKey ButtonReleased)
{
	AMinesweeperGameModeBase*  GameMode = Cast<AMinesweeperGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		if (GameMode->bOnGame)
		{

			if (GameMode->bFirstClick)
			{
				GameMode->StartTimer();
			}

			if (bLeftMousePressed && bRightMousePressed)
			{
				AutoDig();
				GameMode->IsWin();
			}
			else
			{
				if (bLeftMousePressed)
				{
					Dig();
					GameMode->IsWin();
				}
				else
				{
					SetFlag();
				}
			}
		}
	}

	if (ButtonReleased == EKeys::LeftMouseButton)
	{
		bLeftMousePressed = false;
	}
	else
	{
		bRightMousePressed = false;
	}
}

void AMinesweeperBlock::AutoDig()
{
	int32 NearFlagNum = 0;

	if (bActive)
	{
		for (AMinesweeperBlock* block : NearBlocks)
		{
			if (block->bFlag)
			{
				++NearFlagNum;
			}
		}

		if (NearMineNum == NearFlagNum)
		{
			for (AMinesweeperBlock* block : NearBlocks)
			{
				block->Dig();
			}
			bRightMousePressed = bLeftMousePressed = false;
		}
	}
}

void AMinesweeperBlock::Dig()
{
	AMinesweeperGameModeBase*  GameMode = Cast<AMinesweeperGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (GameMode)
	{
		if (GameMode->bFirstClick)
		{
			GameMode->ReceiveFirstInput(this);
		}

		if (!(bActive || bFlag || bQuestion))
		{
			if (bMine)
			{
				GameMode->GameOver(false, Block);
			}
			else
			{
				bActive = true;
				Block->SetSprite(WhiteSprite);
				SetNearMineNum();
			}
		}
	}
}

void AMinesweeperBlock::SetFlag()
{
	AMinesweeperGameModeBase*  GameMode = Cast<AMinesweeperGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (!bActive)
	{
		if (bFlag)
		{
			bFlag = false;
			bQuestion = true;
			Block->SetSprite(GreenSprite);
			MineNum->SetText(TEXT("?"));
			MineNum->SetTextRenderColor(FColor(32, 128, 32));
			++(GameMode->VirtualMineNum);
		}
		else if(bQuestion)
		{
			bQuestion = false;
			Block->SetSprite(BlueSprite);
			MineNum->SetTextRenderColor(FColor(0, 128, 0));
			MineNum->SetText(TEXT(""));
		}
		else
		{
			bFlag = true;
			Block->SetSprite(FlagSprite);
			--(GameMode->VirtualMineNum);
		}
	}
}

void AMinesweeperBlock::SetNearMineNum()
{
	for (AMinesweeperBlock* block : NearBlocks)
	{
		if (block->bMine)
		{
			++NearMineNum;
		}
	}
	if (NearMineNum != 0)
	{
		MineNum->SetText(FText::AsNumber(NearMineNum));
		MineNum->SetHiddenInGame(false);
	}
	else
	{
		for (AMinesweeperBlock* block : NearBlocks)
		{
			block->Dig();
		}
	}
}

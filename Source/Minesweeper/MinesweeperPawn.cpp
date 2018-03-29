// Fill out your copyright notice in the Description page of Project Settings.

#include "MinesweeperPawn.h"

// Sets default values
AMinesweeperPawn::AMinesweeperPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0.0f, 960.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

// Called when the game starts or when spawned
void AMinesweeperPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinesweeperPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinesweeperPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &AMinesweeperPawn::MiddleMouseButtonPressed);
	PlayerInputComponent->BindAction("MiddleMouseButton", IE_Released, this, &AMinesweeperPawn::MiddleMouseButtonReleased);

	PlayerInputComponent->BindAxis("MouseX", this, &AMinesweeperPawn::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &AMinesweeperPawn::MouseY);
	PlayerInputComponent->BindAxis("MouseWheelAxis", this, &AMinesweeperPawn::MouseWheelAxis);
}

void AMinesweeperPawn::MiddleMouseButtonPressed()
{
	bMiddleMousePressed = true;
}

void AMinesweeperPawn::MiddleMouseButtonReleased()
{
	bMiddleMousePressed = false;
}

void AMinesweeperPawn::MouseX(float AxisValue)
{
	if (bMiddleMousePressed)
	{
		AddActorLocalOffset(FVector(AxisValue * 10.0f, 0.0f, 0.0f));
	}
}

void AMinesweeperPawn::MouseY(float AxisValue)
{
	if (bMiddleMousePressed)
	{
		AddActorLocalOffset(FVector(0.0f, 0.0f, AxisValue * 10.0f));
	}
}

void AMinesweeperPawn::MouseWheelAxis(float AxisValue)
{
	AddActorLocalOffset(FVector(0.0f, AxisValue * 50.0f, 0.0f));
}

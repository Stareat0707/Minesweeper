// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MinesweeperPawn.generated.h"

UCLASS()
class MINESWEEPER_API AMinesweeperPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinesweeperPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USceneComponent* Root;

	UCameraComponent* Camera;

	bool bMiddleMousePressed;

	void MiddleMouseButtonPressed();

	void MiddleMouseButtonReleased();

	void MouseX(float AxisValue);

	void MouseY(float AxisValue);

	void MouseWheelAxis(float AxisValue);
};

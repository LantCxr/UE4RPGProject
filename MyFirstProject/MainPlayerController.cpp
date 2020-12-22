// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	//将HUD显示到屏幕上
	HUDOverlay->AddToViewport();
	
	//设置显示
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	//隐藏敌人血条，只在规定时间内显示
	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar) {
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}

		//初始化2D坐标
		FVector2D Alignment(0.f, 0.f);
		//对齐坐标
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	//隐藏菜单面板
	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu) {
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

/// <summary>
/// 显示敌人血条
/// </summary>
void AMainPlayerController::DisplayEnemyHealthBar() {
	if (EnemyHealthBar) {
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

/// <summary>
/// 移除敌人血条
/// </summary>
void AMainPlayerController::RemoveEnemyHealthBar() {
	if (EnemyHealthBar) {
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

/// <summary>
/// 计时器
/// </summary>
/// <param name="DeltaTime"></param>
void AMainPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (EnemyHealthBar) {
		
		//通过3d坐标获取2d坐标
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 70.f;
		PositionInViewport.X -= 70.f;
		//设置2D大小
		FVector2D SizeInViewport(300.f, 25.f);

		//设置血条显示位置
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

/// <summary>
/// 显示菜单
/// </summary>
void AMainPlayerController::DisplayPauseMenu_Implementation() {
	if (PauseMenu) {
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

/// <summary>
/// 隐藏菜单
/// </summary>
void AMainPlayerController::RemovePauseMenu_Implementation() {
	if (PauseMenu) {
		GameModeOnly();
		bShowMouseCursor = false;
		bPauseMenuVisible = false;
	}
}

/// <summary>
/// 切换暂停状态
/// </summary>
void AMainPlayerController::TogglePauseMenu() {
	if (bPauseMenuVisible) {
		RemovePauseMenu();
	}
	else {
		DisplayPauseMenu();
	}
}

void AMainPlayerController::GameModeOnly() {
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
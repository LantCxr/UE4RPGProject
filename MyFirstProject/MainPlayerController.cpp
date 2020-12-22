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

	//��HUD��ʾ����Ļ��
	HUDOverlay->AddToViewport();
	
	//������ʾ
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	//���ص���Ѫ����ֻ�ڹ涨ʱ������ʾ
	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar) {
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}

		//��ʼ��2D����
		FVector2D Alignment(0.f, 0.f);
		//��������
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	//���ز˵����
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
/// ��ʾ����Ѫ��
/// </summary>
void AMainPlayerController::DisplayEnemyHealthBar() {
	if (EnemyHealthBar) {
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

/// <summary>
/// �Ƴ�����Ѫ��
/// </summary>
void AMainPlayerController::RemoveEnemyHealthBar() {
	if (EnemyHealthBar) {
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

/// <summary>
/// ��ʱ��
/// </summary>
/// <param name="DeltaTime"></param>
void AMainPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (EnemyHealthBar) {
		
		//ͨ��3d�����ȡ2d����
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 70.f;
		PositionInViewport.X -= 70.f;
		//����2D��С
		FVector2D SizeInViewport(300.f, 25.f);

		//����Ѫ����ʾλ��
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

/// <summary>
/// ��ʾ�˵�
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
/// ���ز˵�
/// </summary>
void AMainPlayerController::RemovePauseMenu_Implementation() {
	if (PauseMenu) {
		GameModeOnly();
		bShowMouseCursor = false;
		bPauseMenuVisible = false;
	}
}

/// <summary>
/// �л���ͣ״̬
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
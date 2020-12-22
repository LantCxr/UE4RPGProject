// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYFIRSTPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// �༭����UMG����Դ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "HUDOverlayAsset")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/// <summary>
	/// ���������ڿؼ�����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	UUserWidget* HUDOverlay;

	/// <summary>
	/// �༭����UMG����Դ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	/// <summary>
	/// ���������ڿؼ�����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	UUserWidget* EnemyHealthBar;

	/// <summary>
	/// ������ͣ���
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	TSubclassOf<class UUserWidget> WPauseMenu;

	/// <summary>
	/// ��ͣ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	UUserWidget* PauseMenu;

	/// <summary>
	/// �л���ͣ״̬
	/// </summary>
	void TogglePauseMenu();

	/// <summary>
	/// �Ƿ���ʾѪ��UI
	/// true����
	/// false����
	/// </summary>
	bool bEnemyHealthBarVisible;

	/// <summary>
	/// ��ʾ����Ѫ��
	/// </summary>
	void DisplayEnemyHealthBar();
	
	/// <summary>
	/// �Ƴ�����Ѫ��
	/// </summary>
	void RemoveEnemyHealthBar();

	/// <summary>
	/// ��������
	/// </summary>
	FVector EnemyLocation;

	/// <summary>
	/// �Ƿ���ʾ��ͣ����
	/// true����
	/// false����
	/// </summary>
	bool bPauseMenuVisible;

	/// <summary>
	/// ��ʾ����Ѫ��
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "HUD")
	void DisplayPauseMenu();

	/// <summary>
	/// ���ز˵�
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "HUD")
	void RemovePauseMenu();

	/// <summary>
	/// 
	/// </summary>
	void GameModeOnly();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};

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
	/// 编辑器内UMG的资源引用
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "HUDOverlayAsset")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/// <summary>
	/// 创建后用于控件变量
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	UUserWidget* HUDOverlay;

	/// <summary>
	/// 编辑器内UMG的资源引用
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	/// <summary>
	/// 创建后用于控件变量
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	UUserWidget* EnemyHealthBar;

	/// <summary>
	/// 设置暂停面板
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	TSubclassOf<class UUserWidget> WPauseMenu;

	/// <summary>
	/// 暂停
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDOverlayAsset")
	UUserWidget* PauseMenu;

	/// <summary>
	/// 切换暂停状态
	/// </summary>
	void TogglePauseMenu();

	/// <summary>
	/// 是否显示血条UI
	/// true：是
	/// false：否
	/// </summary>
	bool bEnemyHealthBarVisible;

	/// <summary>
	/// 显示敌人血条
	/// </summary>
	void DisplayEnemyHealthBar();
	
	/// <summary>
	/// 移除敌人血条
	/// </summary>
	void RemoveEnemyHealthBar();

	/// <summary>
	/// 敌人坐标
	/// </summary>
	FVector EnemyLocation;

	/// <summary>
	/// 是否显示暂停界面
	/// true：是
	/// false：否
	/// </summary>
	bool bPauseMenuVisible;

	/// <summary>
	/// 显示敌人血条
	/// </summary>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable,Category = "HUD")
	void DisplayPauseMenu();

	/// <summary>
	/// 隐藏菜单
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

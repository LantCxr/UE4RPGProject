// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

enum class EWeaponState :uint8
{
	EWS_Pickup	 UMETA(DisplayName = "Pickup"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_MAX		 UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class MYFIRSTPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	FString Name;

	FORCEINLINE void SetWeqponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeqponState() { return WeaponState; }

	/// <summary>
	/// 武器粒子效果是否允许存在
	/// true：允许
	/// false：不允许
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	bool bWeaponParticle;

	/// <summary>
	/// 武器装备提示音
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	/// <summary>
	/// 武器攻击音效
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;

	/// <summary>
	/// 战斗碰撞盒子
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class UBoxComponent* CombatCollision;

	/// <summary>
	/// 伤害
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Combat")
	float Damage;

	/// <summary>
	/// 伤害模板
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	/// <summary>
	/// 装备武器者
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Weapon) { WeaponInstigator = Weapon; }

public:

	/// <summary>
	/// 骨骼网络
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	/// <summary>
	/// 碰撞开始事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/// <summary>
	/// 碰撞结束事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	/// <summary>
	/// 装备
	/// </summary>
	/// <param name="Char"></param>
	void Equip(class AMain* Char);

	/// <summary>
	/// 碰撞开始事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 碰撞结束事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 攻击开始蒙太奇
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ActiveCollision();

	/// <summary>
	/// 攻击结束蒙太奇
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

private:

	/// <summary>
	/// 武器状态
	/// Pickup：拾取状态
	/// Equipped 装备状态
	/// </summary>
	EWeaponState WeaponState;


};

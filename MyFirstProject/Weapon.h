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
	/// ��������Ч���Ƿ��������
	/// true������
	/// false��������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	bool bWeaponParticle;

	/// <summary>
	/// ����װ����ʾ��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	/// <summary>
	/// ����������Ч
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;

	/// <summary>
	/// ս����ײ����
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class UBoxComponent* CombatCollision;

	/// <summary>
	/// �˺�
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Combat")
	float Damage;

	/// <summary>
	/// �˺�ģ��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	/// <summary>
	/// װ��������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Weapon) { WeaponInstigator = Weapon; }

public:

	/// <summary>
	/// ��������
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	/// <summary>
	/// ��ײ��ʼ�¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/// <summary>
	/// ��ײ�����¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	/// <summary>
	/// װ��
	/// </summary>
	/// <param name="Char"></param>
	void Equip(class AMain* Char);

	/// <summary>
	/// ��ײ��ʼ�¼���ʹ��virtual���壬��������̳�
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
	/// ��ײ�����¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// ������ʼ��̫��
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ActiveCollision();

	/// <summary>
	/// ����������̫��
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

private:

	/// <summary>
	/// ����״̬
	/// Pickup��ʰȡ״̬
	/// Equipped װ��״̬
	/// </summary>
	EWeaponState WeaponState;


};

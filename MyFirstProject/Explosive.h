// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class MYFIRSTPROJECT_API AExplosive : public AItem
{
	GENERATED_BODY()

public:
		AExplosive();

		/// <summary>
		/// ��ը�˺�
		/// </summary>
		UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Damage")
		float Damage;

		/// <summary>
		/// �˺�����
		/// </summary>
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

public:

	/// <summary>
	/// ��ײ��ʼ�¼�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


	/// <summary>
	/// ��ײ�����¼�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};

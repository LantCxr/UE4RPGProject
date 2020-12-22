// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class MYFIRSTPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	/// <summary>
	/// ������״��ײ
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Collision")
	class USphereComponent* CollisionVolume;

	/// <summary>
	/// �����������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	class UStaticMeshComponent* Mesh;

	/// <summary>
	/// ����״̬�µ�����Ч��
	/// EditAnywhere ��ʵ����Ĭ�϶����Ա༭��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystemComponent* IdleParticlesComponent;

	/// <summary>
	/// ��ײ�������Ч��
	/// EditAnywhere ��ʵ����Ĭ�϶����Ա༭��
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystem* OverlapParticles;
	
	/// <summary>
	/// ��ײ�������Ч��
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Sounds")
	class USoundCue* OverlapSound;
	
	/// <summary>
	/// ��ת����
	/// false���ر�
	/// true������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	bool bRotate;

	/// <summary>
	/// ÿ֡��ת�Ƕ�
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	float RotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
		
	/// <summary>
	/// ��ײ�����¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

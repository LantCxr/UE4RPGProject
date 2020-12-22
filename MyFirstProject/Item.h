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
	/// 基础形状碰撞
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Collision")
	class USphereComponent* CollisionVolume;

	/// <summary>
	/// 基础网格组件
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	class UStaticMeshComponent* Mesh;

	/// <summary>
	/// 空闲状态下的粒子效果
	/// EditAnywhere ：实例和默认都可以编辑他
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystemComponent* IdleParticlesComponent;

	/// <summary>
	/// 碰撞后的粒子效果
	/// EditAnywhere ：实例和默认都可以编辑他
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystem* OverlapParticles;
	
	/// <summary>
	/// 碰撞后的声音效果
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Sounds")
	class USoundCue* OverlapSound;
	
	/// <summary>
	/// 旋转开关
	/// false：关闭
	/// true：开启
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	bool bRotate;

	/// <summary>
	/// 每帧旋转角度
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
	/// 碰撞开始事件，使用virtual定义，方便子类继承
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
	/// 碰撞结束事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class MYFIRSTPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float value);

	void MoveRight(float value);

	void PitchCamera(float axisValue);

	void YawCamera(float axisValue);

	/// <summary>
	/// 用于存储摄像机X，Y轴信息
	/// </summary>
	FVector2D CameraInput;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnyWhere,Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnyWhere, Category = "Mesh")
	class USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnyWhere, Category = "Mesh")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnyWhere, Category = "Mesh")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnyWhere, Category = "Movement")
	class UColliderMovementComponent* OurMovementComponent;

	//重写父类移动组件
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* InMesh) { MeshComponent = InMesh; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }
	FORCEINLINE void SetSphereComponent(USphereComponent* InSphere) { SphereComponent = InSphere; }

	FORCEINLINE UCameraComponent* GetCarmeraComponent() { return CameraComponent; }
	FORCEINLINE void SetCarmeraComponent(UCameraComponent* InCamera) { CameraComponent = InCamera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArmComponent; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArm) { SpringArmComponent = InSpringArm; }
};

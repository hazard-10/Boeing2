// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "KATSDKWarpper.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KATVRUniversalSDKBPLibrary.generated.h"

#pragma pack(push,1)
USTRUCT(BlueprintType)
struct FKATDeviceDesc
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FString	device;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FString	serialNumber;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	int		deviceType; //0. Err 1. Tread Mill 2. Tracker 
};

USTRUCT(BlueprintType)
struct FDeviceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	bool	btnPressed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	bool	isBatteryCharging;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	float	batteryLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	int		firmwareVersion;

	FDeviceData& operator=(const DeviceData& data)
	{
		memcpy(this, &data, sizeof(DeviceData));
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FTreadMillData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FString 	deviceName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	bool		connected;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FQuat 		bodyRotationRaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FVector 	moveSpeed;


	FTreadMillData& operator=(const TreadMillData& data)
	{
		deviceName = data.deviceName;
		connected = data.connected;
		//Convert From Standard Left Handness to Unreal

		bodyRotationRaw = Quaternion::unityToUnreal(data.bodyRotationRaw);
		

		//Convert Movement to WorldSpace
		moveSpeed = Vector3::unityToUnreal(data.moveSpeed);
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FKATTreadMillMemoryData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FTreadMillData	treadMillData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FDeviceData		deviceDatas0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FDeviceData		deviceDatas1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FDeviceData		deviceDatas2;

	char			extraData[128];
};

//Enviorment Install Path
USTRUCT(BlueprintType)
struct FInstallPaths
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FString steamInstallPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FString oculusInstallPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KATVRUniversalSDK")
	FString KATLibraryPath;
};

#pragma pack(pop)
UCLASS()
class UKATVRUniversalSDKBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Quaternion Mul Vector", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static FVector QuaterMul(const FQuat& q, const FVector& v);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Connected KAT Treadmill Count", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static int DeviceCount();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get KAT Treadmill Device Description", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static FKATDeviceDesc GetDevicesDesc(int index);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get KAT Treadmill Walk Status", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static FKATTreadMillMemoryData GetWalkStatus(const FString& sn = "");

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get VR Framework Path", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static FInstallPaths GetVRPath();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Raw Vibrate Interface", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static void Vibrate(const FString& sn, float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Raw LED Interface", Keywords = "treadmill KAT"), Category = "KATVRUniversalSDK")
	static void LED(const FString& sn, float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Vibrate With Const Amplitude", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	VibrateConst(float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LED With Const Amplitude", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	LEDConst(float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Vibrate In Seconds", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	VibrateInSeconds(float amplitude, float duration);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Vibrate Once", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	VibrateOnce(float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Vibrate For", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	VibrateFor(float duration, float frequency, float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LED In Seconds", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	LEDInSeconds(float amplitude, float duration);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LED Once", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	LEDOnce(float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LED For", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static void	LEDFor(float duration, float frequency, float amplitude);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Last Calibrated Time Escaped", Keywords = "treadmill KAT Extension"), Category = "KATVRExtensionSDK")
	static float GetLastCalibratedTimeEscaped();

private:
	static KATSDKWarpper sdkWarpper;
};

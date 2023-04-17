// Copyright Epic Games, Inc. All Rights Reserved.

#include "KATVRUniversalSDKBPLibrary.h"
#include "KATVRUniversalSDK.h"

KATSDKWarpper UKATVRUniversalSDKBPLibrary::sdkWarpper = KATSDKWarpper();

UKATVRUniversalSDKBPLibrary::UKATVRUniversalSDKBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


FVector UKATVRUniversalSDKBPLibrary::QuaterMul(const FQuat& q, const FVector& v)
{
	return q * v;
}

int UKATVRUniversalSDKBPLibrary::DeviceCount()
{
	return sdkWarpper.DeviceCount();
}

FKATDeviceDesc UKATVRUniversalSDKBPLibrary::GetDevicesDesc(int index)
{
	FKATDeviceDesc fdd;
	auto r = sdkWarpper.GetDevicesDesc(index);

	fdd.device = r.device;
	fdd.deviceType = r.deviceType;
	fdd.serialNumber = r.serialNumber;

	return fdd;
}

FKATTreadMillMemoryData UKATVRUniversalSDKBPLibrary::GetWalkStatus(const FString& sn)
{
	FKATTreadMillMemoryData tmmd;
	auto r = sdkWarpper.GetWalkStatus(TCHAR_TO_ANSI(*sn));
	tmmd.deviceDatas0 = r.deviceDatas[0];
	tmmd.deviceDatas1 = r.deviceDatas[1];
	tmmd.deviceDatas2 = r.deviceDatas[2];
	tmmd.treadMillData = r.treadMillData;

	return tmmd;
}

FInstallPaths UKATVRUniversalSDKBPLibrary::GetVRPath()
{
	FInstallPaths fip;
	auto r = sdkWarpper.GetVRPath();
	fip.KATLibraryPath = r.KATLibraryPath;
	fip.oculusInstallPath = r.oculusInstallPath;
	fip.steamInstallPath = r.steamInstallPath;

	return fip;
}

void UKATVRUniversalSDKBPLibrary::Vibrate(const FString& sn, float amplitude)
{
	sdkWarpper.Vibrate(TCHAR_TO_ANSI(*sn), amplitude);
}

void UKATVRUniversalSDKBPLibrary::LED(const FString& sn, float amplitude)
{
	sdkWarpper.LED(TCHAR_TO_ANSI(*sn), amplitude);
}

void UKATVRUniversalSDKBPLibrary::VibrateConst(float amplitude) 
{
	sdkWarpper.VibrateConst(amplitude);
}

void UKATVRUniversalSDKBPLibrary::LEDConst(float amplitude)
{
	sdkWarpper.LEDConst(amplitude);
}

void UKATVRUniversalSDKBPLibrary::VibrateInSeconds(float amplitude, float duration)
{
	sdkWarpper.VibrateInSeconds(amplitude, duration);
}

void UKATVRUniversalSDKBPLibrary::VibrateOnce(float amplitude) 
{
	sdkWarpper.VibrateOnce(amplitude);
}

void UKATVRUniversalSDKBPLibrary::VibrateFor(float duration, float frequency, float amplitude) 
{
	sdkWarpper.VibrateFor(duration, frequency, amplitude);
}

void UKATVRUniversalSDKBPLibrary::LEDInSeconds(float amplitude, float duration) 
{
	sdkWarpper.LEDInSeconds(amplitude, duration);
}

void UKATVRUniversalSDKBPLibrary::LEDOnce(float amplitude) 
{
	sdkWarpper.LEDOnce(amplitude);
}

void UKATVRUniversalSDKBPLibrary::LEDFor(float duration, float frequency, float amplitude)
{
	sdkWarpper.LEDFor(duration, frequency, amplitude);
}

float UKATVRUniversalSDKBPLibrary::GetLastCalibratedTimeEscaped()
{
	return sdkWarpper.GetLastCalibratedTimeEscaped();
}


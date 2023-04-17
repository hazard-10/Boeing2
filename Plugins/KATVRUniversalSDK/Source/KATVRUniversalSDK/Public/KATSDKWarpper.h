#pragma once

#include "Core.h"
#include <functional>

#pragma pack(push,1)
struct KATDeviceDesc
{
	char	device[64];
	char	serialNumber[13];
	int		pid;
	int		vid;
	int		deviceType; //0. Err 1. Tread Mill 2. Tracker 
};

struct DeviceData
{
	bool	btnPressed;
	bool	isBatteryCharging;
	float	batteryLevel;
	char	firmwareVersion;
};

struct Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	static FQuat toFQuat(const Quaternion& q)
	{
		return FQuat(q.x, q.y, q.z, q.w);
	}

	static FQuat unityToUnreal(const Quaternion& uq)
	{
		auto fq = toFQuat(uq);
		FVector axis;
		float angle;
		fq.ToAxisAndAngle(axis, angle);
		axis = FVector(axis.Z, -axis.X, axis.Y);

		return FQuat(FVector::UpVector, FMath::DegreesToRadians(90.0f))  *  FQuat(axis, -angle);
	}
};

struct Vector3
{
public:
	float x;
	float y;
	float z;

	static FVector toFVector(const Vector3 v)
	{
		return FVector(v.x, v.y, v.z);
	}

	static FVector unityToUnreal(const Vector3 uv)
	{
		return FVector(uv.x, uv.z, uv.y);
	}
};

struct TreadMillData
{
	char 		deviceName[64];
	bool		connected;
	double		lastUpdateTimePoint;

	Quaternion 		bodyRotationRaw;

	Vector3 	moveSpeed;
};

struct KATTreadMillMemoryData
{
	TreadMillData	treadMillData;

	DeviceData  deviceDatas[3];

	char		extraData[128];
};

struct InstallPaths
{
	char steamInstallPath[1024];
	char oculusInstallPath[1024];
	char KATLibraryPath[1024];
};

class KATSDKWarpper
{
private:
	void* _sdkHandle;
	
	void* handle()
	{
		auto sdkPath = FPaths::ProjectPluginsDir() + "KATVRUniversalSDK/ThirdParty/KAT/" + "KATNativeSDK.dll";
		if (_sdkHandle == nullptr)
		{
			auto arr = *sdkPath;

			if (FPaths::FileExists(sdkPath))
			{
				_sdkHandle = FPlatformProcess::GetDllHandle(arr);
			}

			if (_sdkHandle == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("KAT Native SDK Not Found!"));
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::White, *sdkPath);
		return _sdkHandle;
	}

	std::function<int(void)>							deviceCountFunc;
	std::function<KATDeviceDesc(unsigned)>				getDevicesDescFunc;
	std::function<KATTreadMillMemoryData(const char*)>	getWalkStatusFunc;
	std::function<void(const char*)>					calibrateFunc;
	std::function<InstallPaths()>						getVRPathFunc;
	std::function<void(const char*,float)>				vibrateFunc;
	std::function<void(const char*,float)>				LEDFunc;

	std::function<void(float)>							vibrateConstFunc;
	std::function<void(float,float)>					vibrateInSecondsFunc;
	std::function<void(float)>							vibrateOnceFunc;
	std::function<void(float, float, float)>			vibrateForFunc;
	
	std::function<void(float)>							LEDConstFunc;
	std::function<void(float,float)>					LEDInSecondsFunc;
	std::function<void(float)>							LEDOnceFunc;
	std::function<void(float, float, float)>			LEDForFunc;

	std::function<double()>								GetLastCalibratedTimeEscapedFunc;

public:
	KATSDKWarpper() :_sdkHandle(nullptr)
	{

	}

	~KATSDKWarpper()
	{
		FPlatformProcess::FreeDllHandle(_sdkHandle);
	}

	//KAT Device Count
	int	DeviceCount()
	{
		if (handle() != nullptr)
		{
			if (deviceCountFunc == nullptr)
			{
				using funcType = int(void);
				deviceCountFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("DeviceCount")));
			}

			return deviceCountFunc();
		}

		return -1;
	}

	//KAT Device Description
	KATDeviceDesc GetDevicesDesc(unsigned index)
	{
		if (handle() != nullptr)
		{
			if (getDevicesDescFunc == nullptr)
			{
				using funcType = KATDeviceDesc(unsigned);
				getDevicesDescFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("GetDevicesDesc")));
			}

			return getDevicesDescFunc(index);
		}

		return KATDeviceDesc();
	}

	//Main Function to Get Treadmill output
	KATTreadMillMemoryData GetWalkStatus(const char* sn /* = nullptr*/)
	{
		if (handle() != nullptr)
		{
			if (getWalkStatusFunc == nullptr)
			{
				using funcType = KATTreadMillMemoryData(const char*);
				getWalkStatusFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("GetWalkStatus")));
			}

			return getWalkStatusFunc(sn);
		}

		return KATTreadMillMemoryData();
	}

	//Get Enviorment Install Path
	InstallPaths GetVRPath()
	{
		if (handle() != nullptr)
		{
			using funcType = InstallPaths();
			getVRPathFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("GetVRPath")));
			return getVRPathFunc();
		}

		return InstallPaths();
	}

	//Raw Vibrate Interface
	void Vibrate(const char* sn, float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(const char*,float);
			vibrateFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("Vibrate")));

			vibrateFunc(sn, amplitude);
		}
	}

	//Raw LED Interface
	void LED(const char* sn, float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(const char*, float);
			LEDFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("LED")));

			LEDFunc(sn, amplitude);
		}
	}

	//KAT PWM Functions Only Support Prime Devices

	//Start Vibrate with a const amplitude,0 for stop
	void VibrateConst(float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float);
			vibrateConstFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("VibrateConst")));

			vibrateConstFunc(amplitude);
		}
	}

	//Start LED with a const amplitude,0 for stop
	void LEDConst(float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float);
			LEDConstFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("LEDConst")));

			LEDConstFunc(amplitude);
		}
	}

	//Start Vibrate In a duration
	void VibrateInSeconds(float amplitude, float duration)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float,float);
			vibrateInSecondsFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("VibrateInSeconds")));
		
			vibrateInSecondsFunc(amplitude, duration);
		}
	}

	//Start Vibrate Once
	void VibrateOnce(float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float);
			vibrateOnceFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("VibrateOnce")));

			vibrateOnceFunc(amplitude);
		}
	}

	//Start Vibrate with duration and frequency, amplitude
	void VibrateFor(float duration, float frequency, float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float,float,float);
			vibrateForFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("VibrateFor")));

			vibrateForFunc(duration, frequency, amplitude);
		}
	}

	//Start LED In a duration
	void LEDInSeconds(float amplitude, float duration)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float, float);
			LEDInSecondsFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("LEDInSeconds")));

			LEDInSecondsFunc(amplitude, duration);
		}
	}

	//Start LED Once
	void LEDOnce(float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float);
			LEDOnceFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("amplitude")));

			LEDOnceFunc(amplitude);
		}
	}

	//Start LED  with duration and frequency, amplitude
	void LEDFor(float duration, float frequency, float amplitude)
	{
		if (handle() != nullptr)
		{
			using funcType = void(float,float,float);
			LEDForFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("LEDFor")));

			LEDFor(duration, frequency, amplitude);
		}
	}

	//Get time in seconeds since last calibration
	float GetLastCalibratedTimeEscaped()
	{
		if (handle() != nullptr)
		{
			using funcType = double();
			GetLastCalibratedTimeEscapedFunc = reinterpret_cast<funcType*>(FPlatformProcess::GetDllExport(handle(), TEXT("GetLastCalibratedTimeEscaped")));

			return (float)GetLastCalibratedTimeEscapedFunc();
		}
		return 100;
	}
};

#pragma pack(pop)
// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

public class KATVRUniversalSDK : ModuleRules
{


	public KATVRUniversalSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		//string dllDir = Path.Combine(ModuleDirectory, "ThirdParty/KAT");
		var fDir = Directory.GetParent(ModuleDirectory + "\\1.dum");
		var fstr = fDir.Parent.Parent.FullName + "\\ThirdParty\\KAT";

		foreach (string file in Directory.GetFiles(fstr))
		{
			System.Console.WriteLine("dll Found:" + file);
			RuntimeDependencies.Add(file);
			// delay load
			string filename = Path.GetFileName(file);
			PublicDelayLoadDLLs.Add(filename);
		}
	}
}

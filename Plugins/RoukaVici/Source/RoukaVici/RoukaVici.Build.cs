// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RoukaVici : ModuleRules
{
	public RoukaVici(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Json",
                "JSonUtilities",
                "UMG"
			}
			);

        string platformName = Target.Platform.ToString();
        string libFolder = Path.Combine(ModuleDirectory, "lib");

        if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
        {
            PublicLibraryPaths.Add(Path.Combine(libFolder, platformName));
            PublicAdditionalLibraries.Add("roukavici.lib");
            PublicAdditionalLibraries.Add("bluetoothserialport.lib");
            CopyToProjectBinaries(Path.Combine(libFolder, string.Concat(platformName, "/roukavici.dll")), Target);
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {

        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicAdditionalLibraries.Add(Path.Combine(libFolder, platformName, "libroukavici.dylib"));
            PublicAdditionalLibraries.Add(Path.Combine(libFolder, platformName, "libbluetoothserialport.dylib"));
        }
    }

    private void CopyToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesDir = Path.Combine(ModuleDirectory, "../../../..", "Binaries", Target.Platform.ToString());
        string Filename = Path.GetFileName(Filepath);

        //convert relative path 
        string FullBinariesDir = Path.GetFullPath(BinariesDir);

        if (!Directory.Exists(FullBinariesDir))
        {
            Directory.CreateDirectory(FullBinariesDir);
        }

        string FullExistingPath = Path.Combine(FullBinariesDir, Filename);
        bool ValidFile = false;

        //File exists, check if they're the same
        if (File.Exists(FullExistingPath))
        {
            int ExistingFileHash = HashFile(FullExistingPath);
            int TargetFileHash = HashFile(Filepath);
            ValidFile = ExistingFileHash == TargetFileHash;
            if (!ValidFile)
            {
                System.Console.WriteLine("RoukaVici Plugin: outdated lib detected.");
            }
        }

        //No valid existing file found, copy new dll
        if (!ValidFile)
        {
            System.Console.WriteLine("RoukaVici Plugin: Copied from " + Filepath + ", to " + Path.Combine(FullBinariesDir, Filename));
            File.Copy(Filepath, Path.Combine(FullBinariesDir, Filename), true);
        }
    }

    private int HashFile(string FilePath)
    {
        string DLLString = File.ReadAllText(FilePath);
        return DLLString.GetHashCode() + DLLString.Length;  //ensure both hash and file lengths match
    }
}

# Builds a .zip file for loading with BMBF
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./libs/arm64-v8a/libnjstweaks.so", "./libs/arm64-v8a/libbeatsaber-hook_1_2_4.so", "./bmbfmod.json" -DestinationPath "./NjsTweaks_v0.0.1.zip" -Update
}

# Builds a .qmod file
& $PSScriptRoot/build.ps1

if ($?) {
    Compress-Archive -Path "./mod.json", "./libs/arm64-v8a/libnjstweaks.so", "./libs/arm64-v8a/libbeatsaber-hook_2_2_5.so", "./libs/arm64-v8a/libquestui.so", "./libs/arm64-v8a/libcustom-types.so" -DestinationPath "./njstweaks_v0.0.3.zip" -Update
}

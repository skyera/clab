[CmdletBinding()]
param(
    [switch]$MinGW,
    [switch]$Run
)

$ErrorActionPreference = 'Stop'

function Invoke-Checked {
    param([string]$Program, [string[]]$Arguments)
    & $Program @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "$Program failed with exit code $LASTEXITCODE."
    }
}

try {
    foreach ($tool in @('cmake', 'ctest')) {
        if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) {
            throw "$tool is required. Install CMake and add its bin directory to PATH."
        }
    }

    if (-not (Test-Path -LiteralPath "$PSScriptRoot/third_party/uthash/src/uthash.h") -or
        -not (Test-Path -LiteralPath "$PSScriptRoot/third_party/Unity/src/unity.c")) {
        Invoke-Checked git @('-C', $PSScriptRoot, 'submodule', 'update', '--init', '--recursive')
    }

    if ($MinGW) {
        $buildDir = Join-Path $PSScriptRoot 'build-mingw'
        $generatorArgs = @('-G', 'MinGW Makefiles', '-DCMAKE_BUILD_TYPE=Release')
        $executable = Join-Path $buildDir 'uthash_demo.exe'
    } else {
        $buildDir = Join-Path $PSScriptRoot 'build-windows'
        $generatorArgs = @('-G', 'Visual Studio 17 2022', '-A', 'x64')
        $executable = Join-Path $buildDir 'Release/uthash_demo.exe'
    }

    Invoke-Checked cmake (@('-S', $PSScriptRoot, '-B', $buildDir, '-DBUILD_TESTING=ON') + $generatorArgs)
    Invoke-Checked cmake @('--build', $buildDir, '--config', 'Release')
    Invoke-Checked ctest @('--test-dir', $buildDir, '-C', 'Release', '--output-on-failure')
    Write-Host "Built: $executable"
    if ($Run) {
        Invoke-Checked $executable @()
    }
} catch {
    Write-Error $_ -ErrorAction Continue
    exit 1
}

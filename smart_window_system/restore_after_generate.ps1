# restore_after_generate.ps1
# STM32CubeIDE에서 Generate Code 실행 후 이 스크립트를 실행하세요.
# 역할: X-CUBE-AI Middlewares 복원 + CMSIS-DSP 복원 + .cproject include path 복구

$workspace   = $PSScriptRoot
$packBase    = "C:\Users\samsung\STM32Cube\Repository\Packs\STMicroelectronics\X-CUBE-AI\8.0.0\Middlewares"
$cmsisSource = "C:\Users\samsung\Downloads\CMSIS-DSP-main\CMSIS-DSP-main"
$cmsisTarget = "$workspace\Core\Src\CMSIS_DSP"

# ── 1. X-CUBE-AI NetworkRuntime (ST/AI) 복원 ─────────────────────────────
$aiInc = "$workspace\Middlewares\ST\AI\Inc"
$aiLib = "$workspace\Middlewares\ST\AI\Lib\GCC\ARMCortexM4"

if (-not (Test-Path "$aiLib\NetworkRuntime800_CM4_GCC.a")) {
    Write-Host "[1/3] X-CUBE-AI Middlewares 복원 중..." -ForegroundColor Yellow

    # Inc 헤더 복사
    New-Item -ItemType Directory -Force $aiInc | Out-Null
    Copy-Item -Recurse -Force "$packBase\ST\AI\Inc\*" "$aiInc\"

    # Lib 라이브러리 복사 (GCC/ARMCortexM4만 사용)
    New-Item -ItemType Directory -Force $aiLib | Out-Null
    Copy-Item -Force "$packBase\ST\AI\Lib\GCC\ARMCortexM4\NetworkRuntime800_CM4_GCC.a" "$aiLib\"

    Write-Host "      완료: $workspace\Middlewares\ST\AI" -ForegroundColor Green
} else {
    Write-Host "[1/3] X-CUBE-AI Middlewares 확인 OK" -ForegroundColor Green
}

# ── 2. CMSIS-DSP 소스 파일 복원 ──────────────────────────────────────────
if (-not (Test-Path "$cmsisTarget\Include\arm_math.h")) {
    Write-Host "[2/3] CMSIS-DSP 파일 복원 중..." -ForegroundColor Yellow
    foreach ($sub in @("Include", "PrivateInclude", "Source")) {
        $dst = "$cmsisTarget\$sub"
        New-Item -ItemType Directory -Force $dst | Out-Null
        Copy-Item -Recurse -Force "$cmsisSource\$sub\*" "$dst\"
    }
    Write-Host "      완료: $cmsisTarget" -ForegroundColor Green
} else {
    Write-Host "[2/3] CMSIS-DSP 파일 확인 OK" -ForegroundColor Green
}

# ── 3. .cproject include path 복구 ───────────────────────────────────────
$cprojectPath = "$workspace\.cproject"
$content = Get-Content $cprojectPath -Raw -Encoding UTF8

$newPaths = @(
    '								<listOptionValue builtIn="false" value="../Core/Src/CMSIS_DSP/Include"/>',
    '								<listOptionValue builtIn="false" value="../Core/Src/CMSIS_DSP/PrivateInclude"/>',
    '								<listOptionValue builtIn="false" value="../Core/Src/CMSIS_DSP/Include/dsp"/>'
)
$insertBlock = ($newPaths -join "`r`n") + "`r`n"

if ($content -match 'CMSIS_DSP') {
    Write-Host "[3/3] .cproject CMSIS-DSP 경로 확인 OK" -ForegroundColor Green
} else {
    Write-Host "[3/3] .cproject CMSIS-DSP 경로 복구 중..." -ForegroundColor Yellow
    $pattern    = '(\t+<listOptionValue builtIn="false" value="\.\./Core/Inc"/>)'
    $replacement = $insertBlock + '$1'
    $newContent  = [regex]::Replace($content, $pattern, $replacement)

    if ($newContent -eq $content) {
        Write-Host "      경고: ../Core/Inc 패턴을 찾지 못했습니다. 수동으로 include path를 추가하세요." -ForegroundColor Red
    } else {
        [System.IO.File]::WriteAllText($cprojectPath, $newContent, [System.Text.Encoding]::UTF8)
        Write-Host "      완료" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "완료! STM32CubeIDE에서 프로젝트를 Refresh(F5) 후 빌드하세요." -ForegroundColor Cyan

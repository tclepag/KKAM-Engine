@echo off
setlocal enabledelayedexpansion

set OUTDIR=%1
echo Moving compiled shaders to their source locations...

for /r "%OUTDIR%Content" %%f in (*.hlsl) do (
  set hlslName=%%~nf
  if exist "%OUTDIR%!hlslName!.cso" (
    echo Moving %OUTDIR%!hlslName!.cso to %%~dpf!hlslName!.cso
    move "%OUTDIR%!hlslName!.cso" "%%~dpf!hlslName!.cso"
  )
)
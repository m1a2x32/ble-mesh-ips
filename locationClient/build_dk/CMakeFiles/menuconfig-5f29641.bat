@echo off
cd /D D:\nrfCode\locationClient\build_dk\zephyr\kconfig || (set FAIL_LINE=2& goto :ABORT)
C:\ncs\toolchains\v2.1.0\opt\bin\cmake.exe -E env ZEPHYR_BASE=D:/nrfCode/BLECode/zephyr PYTHON_EXECUTABLE=C:/ncs/toolchains/v2.1.0/opt/bin/python.exe srctree=D:/nrfCode/BLECode/zephyr KERNELVERSION=0x3016300 CONFIG_=CONFIG_ KCONFIG_CONFIG=D:/nrfCode/locationClient/build_dk/zephyr/.config ARCH=arm ARCH_DIR=D:/nrfCode/BLECode/zephyr/arch BOARD_DIR=D:/nrfCode/BLECode/zephyr/boards/arm/nrf52840dk_nrf52840 BOARD_REVISION= KCONFIG_BINARY_DIR=D:/nrfCode/locationClient/build_dk/Kconfig ZEPHYR_TOOLCHAIN_VARIANT=zephyr TOOLCHAIN_KCONFIG_DIR=C:/ncs/toolchains/v2.1.0/opt/zephyr-sdk/cmake/zephyr TOOLCHAIN_HAS_NEWLIB=y EDT_PICKLE=D:/nrfCode/locationClient/build_dk/zephyr/edt.pickle NCS_MEMFAULT_FIRMWARE_SDK_KCONFIG=D:/nrfCode/BLECode/nrf/modules/memfault-firmware-sdk/Kconfig ZEPHYR_NRF_MODULE_DIR=D:/nrfCode/BLECode/nrf ZEPHYR_HOSTAP_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/hostap ZEPHYR_MCUBOOT_MODULE_DIR=D:/nrfCode/BLECode/bootloader/mcuboot ZEPHYR_MCUBOOT_KCONFIG=D:/nrfCode/BLECode/nrf/modules/mcuboot/Kconfig ZEPHYR_MBEDTLS_MODULE_DIR=D:/nrfCode/BLECode/modules/crypto/mbedtls ZEPHYR_MBEDTLS_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/mbedtls/Kconfig ZEPHYR_TRUSTED_FIRMWARE_M_MODULE_DIR=D:/nrfCode/BLECode/modules/tee/tf-m/trusted-firmware-m ZEPHYR_TRUSTED_FIRMWARE_M_KCONFIG=D:/nrfCode/BLECode/nrf/modules/trusted-firmware-m/Kconfig ZEPHYR_TFM_MCUBOOT_MODULE_DIR=D:/nrfCode/BLECode/modules/tee/tfm-mcuboot ZEPHYR_CJSON_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/cjson ZEPHYR_CJSON_KCONFIG=D:/nrfCode/BLECode/nrf/modules/cjson/Kconfig ZEPHYR_AZURE_SDK_FOR_C_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/azure-sdk-for-c ZEPHYR_AZURE_SDK_FOR_C_KCONFIG=D:/nrfCode/BLECode/nrf/modules/azure-sdk-for-c/Kconfig ZEPHYR_MEMFAULT_FIRMWARE_SDK_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/memfault-firmware-sdk ZEPHYR_CIRRUS_LOGIC_MODULE_DIR=D:/nrfCode/BLECode/modules/hal/cirrus-logic ZEPHYR_OPENTHREAD_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/openthread ZEPHYR_OPENTHREAD_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/openthread/Kconfig ZEPHYR_CANOPENNODE_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/canopennode ZEPHYR_CANOPENNODE_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/canopennode/Kconfig ZEPHYR_CHRE_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/chre ZEPHYR_CMSIS_MODULE_DIR=D:/nrfCode/BLECode/modules/hal/cmsis ZEPHYR_FATFS_MODULE_DIR=D:/nrfCode/BLECode/modules/fs/fatfs ZEPHYR_HAL_NORDIC_MODULE_DIR=D:/nrfCode/BLECode/modules/hal/nordic ZEPHYR_HAL_NORDIC_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/hal_nordic/Kconfig ZEPHYR_ST_MODULE_DIR=D:/nrfCode/BLECode/modules/hal/st ZEPHYR_LIBMETAL_MODULE_DIR=D:/nrfCode/BLECode/modules/hal/libmetal ZEPHYR_LIBLC3CODEC_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/liblc3codec ZEPHYR_LIBLC3CODEC_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/liblc3codec/Kconfig ZEPHYR_LITTLEFS_MODULE_DIR=D:/nrfCode/BLECode/modules/fs/littlefs ZEPHYR_LITTLEFS_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/littlefs/Kconfig ZEPHYR_LORAMAC_NODE_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/loramac-node ZEPHYR_LORAMAC_NODE_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/loramac-node/Kconfig ZEPHYR_LVGL_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/gui/lvgl ZEPHYR_LZ4_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/lz4 ZEPHYR_LZ4_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/lz4/Kconfig ZEPHYR_MIPI_SYS_T_MODULE_DIR=D:/nrfCode/BLECode/modules/debug/mipi-sys-t ZEPHYR_NANOPB_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/nanopb ZEPHYR_NANOPB_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/nanopb/Kconfig ZEPHYR_NRF_HW_MODELS_MODULE_DIR=D:/nrfCode/BLECode/modules/bsim_hw_models/nrf_hw_models ZEPHYR_OPEN_AMP_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/open-amp ZEPHYR_PICOLIBC_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/picolibc ZEPHYR_SEGGER_MODULE_DIR=D:/nrfCode/BLECode/modules/debug/segger ZEPHYR_TINYCBOR_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/tinycbor ZEPHYR_TINYCRYPT_MODULE_DIR=D:/nrfCode/BLECode/modules/crypto/tinycrypt ZEPHYR_TRACERECORDER_MODULE_DIR=D:/nrfCode/BLECode/modules/debug/TraceRecorder ZEPHYR_TRACERECORDER_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/TraceRecorder/Kconfig ZEPHYR_TF_M_TESTS_MODULE_DIR=D:/nrfCode/BLECode/modules/tee/tf-m/tf-m-tests ZEPHYR_PSA_ARCH_TESTS_MODULE_DIR=D:/nrfCode/BLECode/modules/tee/tf-m/psa-arch-tests ZEPHYR_ZCBOR_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/zcbor ZEPHYR_ZCBOR_KCONFIG=D:/nrfCode/BLECode/zephyr/modules/zcbor/Kconfig ZEPHYR_ZSCILIB_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/zscilib ZEPHYR_NRFXLIB_MODULE_DIR=D:/nrfCode/BLECode/nrfxlib ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR=D:/nrfCode/BLECode/modules/lib/matter SHIELD_AS_LIST= DTS_POST_CPP=D:/nrfCode/locationClient/build_dk/zephyr/zephyr.dts.pre DTS_ROOT_BINDINGS=D:/nrfCode/BLECode/nrf/dts/bindings?D:/nrfCode/BLECode/zephyr/dts/bindings C:/ncs/toolchains/v2.1.0/opt/bin/python.exe D:/nrfCode/BLECode/zephyr/scripts/kconfig/menuconfig.py D:/nrfCode/BLECode/zephyr/Kconfig || (set FAIL_LINE=3& goto :ABORT)
goto :EOF

:ABORT
set ERROR_CODE=%ERRORLEVEL%
echo Batch file failed at line %FAIL_LINE% with errorcode %ERRORLEVEL%
exit /b %ERROR_CODE%
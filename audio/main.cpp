#define MINIAUDIO_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include "miniaudio.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData;
    ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL);
    (void)pOutput;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: %s <output file>\n", argv[0]);
        return -1;
    }

    ma_result result;
    ma_encoder encoder;
    ma_encoder_config encoderConfig;
    ma_device_config deviceConfig;
    ma_device device;

    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100);

    if (ma_encoder_init_file(argv[1], &encoderConfig, &encoder) != MA_SUCCESS) {
        printf("Failed to initialize output file.\n");
        return -1;
    }

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format   = encoder.config.format;
    deviceConfig.capture.channels = encoder.config.channels;
    deviceConfig.sampleRate       = encoder.config.sampleRate;
    deviceConfig.dataCallback     = data_callback;
    deviceConfig.pUserData        = &encoder;

    ma_device_info* pCaptureDeviceInfos;
    ma_uint32 captureDeviceCount;
    result = ma_context_get_devices(NULL, &pCaptureDeviceInfos, &captureDeviceCount, NULL, NULL);
    if (result != MA_SUCCESS) {
        printf("Failed to retrieve capture devices.\n");
        return -4;
    }

    printf("Available capture devices:\n");
    for (ma_uint32 i = 0; i < captureDeviceCount; ++i) {
        printf("    %u: %s\n", i, pCaptureDeviceInfos[i].name);
    }

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
        return -2;
    }

    printf("Using capture device: %s\n", pCaptureDeviceInfos[0].name);

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }

    printf("Press Enter to stop recording...\n");
    getchar();
    
    ma_device_uninit(&device);
    ma_encoder_uninit(&encoder);

    return 0;
}

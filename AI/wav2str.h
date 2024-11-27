#include <iostream>
#include <vector>
#include <stdexcept>
#include <sndfile.hh>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#include <filesystem>
#include <fstream>


std::vector<std::string> read_tokens(const std::string& file_path);

std::vector<float> read_audio(const std::string& file_path, int& sample_rate);

std::vector<float> run_onnx_model(const std::string& model_path, const std::vector<float>& input_data);

std::string decode_output(const std::vector<float>& output_data, const std::vector<std::string>& tokens);

std::vector<float> generate_mel_spectrogram(const std::vector<float>& audio_data, int sample_rate, std::vector<int64_t>& input_shape);

int aiTest();

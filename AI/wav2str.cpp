#include "wav2str.h"

// 读取tokens文件
std::vector<std::string> read_tokens(const std::string& file_path) {
    std::vector<std::string> tokens;
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        tokens.push_back(line);
    }
    return tokens;
}

// 读取音频文件
std::vector<float> read_audio(const std::string& file_path, int& sample_rate) {
    SndfileHandle file(file_path);
    if (file.error()) {
        throw std::runtime_error("Error reading audio file");
    }
    sample_rate = file.samplerate();
    std::vector<float> audio_data(file.frames() * file.channels());
    file.readf(audio_data.data(), file.frames());
    return audio_data;
}

// 生成mel频谱图
std::vector<float> generate_mel_spectrogram(const std::vector<float>& audio_data, int sample_rate, std::vector<int64_t>& input_shape) {
    int n_mels = 80;
    int n_fft = 2048;
    int hop_length = 512;
    int n_frames = 1 + (audio_data.size() - n_fft) / hop_length;
    std::vector<float> mel_spectrogram(n_mels * n_frames, 0.0f);

    // 计算mel滤波器
    std::vector<std::vector<float>> mel_filters(n_mels, std::vector<float>(n_fft / 2 + 1, 0.0f));
    for (int m = 0; m < n_mels; ++m) {
        for (int k = 0; k <= n_fft / 2; ++k) {
            float mel = 2595.0f * std::log10(1.0f + k * sample_rate / (700.0f * n_fft));
            mel_filters[m][k] = std::max(0.0f, 1.0f - std::abs(mel - (m + 1) * 2595.0f / n_mels));
        }
    }

    // 计算STFT
    for (int t = 0; t < n_frames; ++t) {
        std::vector<float> frame(audio_data.begin() + t * hop_length, audio_data.begin() + t * hop_length + n_fft);
        std::vector<float> spectrum(n_fft / 2 + 1, 0.0f);
        for (int k = 0; k <= n_fft / 2; ++k) {
            for (int n = 0; n < n_fft; ++n) {
                spectrum[k] += frame[n] * std::cos(2.0 * M_PI * k * n / n_fft);
            }
        }
        for (int m = 0; m < n_mels; ++m) {
            for (int k = 0; k <= n_fft / 2; ++k) {
                mel_spectrogram[m * n_frames + t] += spectrum[k] * mel_filters[m][k];
            }
        }
    }

    input_shape = {1, n_mels, n_frames};
    return mel_spectrogram;
}

// 运行ONNX模型
std::vector<float> run_onnx_model(const std::string& model_path, std::vector<float>& input_data, const std::vector<int64_t>& input_shape) {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    Ort::SessionOptions session_options;
    Ort::Session session(env, model_path.c_str(), session_options);

    Ort::AllocatorWithDefaultOptions allocator;
    Ort::AllocatedStringPtr input_name = session.GetInputNameAllocated(0, allocator);
    Ort::AllocatedStringPtr output_name = session.GetOutputNameAllocated(0, allocator);

    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_data.data(), input_data.size(), input_shape.data(), input_shape.size());

    const char* input_names[] = {input_name.get()};
    const char* output_names[] = {output_name.get()};
    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, input_names, &input_tensor, 1, output_names, 1);
    float* float_array = output_tensors.front().GetTensorMutableData<float>();

    std::vector<float> output_data(float_array, float_array + output_tensors.front().GetTensorTypeAndShapeInfo().GetElementCount());
    return output_data;
}

// 将输出数据转换为字符串
std::string decode_output(const std::vector<float>& output_data, const std::vector<std::string>& tokens) {
    std::string result;
    for (float value : output_data) {
        int index = static_cast<int>(value);
        if (index >= 0 && index < tokens.size()) {
            result += tokens[index];
        }
    }
    return result;
}

int aiTest()
{
    try {
            std::string encoder_model_path = "/home/ayti/Desktop/whisper-tiny/tiny.en-encoder.onnx";
            std::string decoder_model_path = "/home/ayti/Desktop/whisper-tiny/tiny.en-decoder.onnx";
            std::string tokens_file_path = "/home/ayti/Desktop/whisper-tiny/tiny.en-tokens.txt";
            std::string audio_file_path = "/home/ayti/Desktop/whisper-tiny/0.wav";

            // 读取tokens
            std::vector<std::string> tokens = read_tokens(tokens_file_path);

            // 读取音频文件
            int sample_rate;
            std::vector<float> audio_data = read_audio(audio_file_path, sample_rate);

            // 生成mel频谱图
            std::vector<int64_t> input_shape;
            std::vector<float> mel_spectrogram = generate_mel_spectrogram(audio_data, sample_rate, input_shape);

            // 运行编码器模型
            std::vector<float> encoder_output = run_onnx_model(encoder_model_path, mel_spectrogram, input_shape);

            // 运行解码器模型
            std::vector<float> decoder_output = run_onnx_model(decoder_model_path, encoder_output, {1, static_cast<int64_t>(encoder_output.size())});

            // 解码输出
            std::string result = decode_output(decoder_output, tokens);

            std::cout << "Transcribed text: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        return 0;
}

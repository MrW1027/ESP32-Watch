#ifndef __MY_AI_CHAT_H
#define __MY_AI_CHAT_H
#include <wakeup_detect_houguoxiong_inferencing.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <driver/i2s.h>
static void audio_inference_callback(uint32_t n_bytes);
static void capture_samples(void* arg);
static bool microphone_inference_start(uint32_t n_samples);
static bool microphone_inference_record(void);
static int microphone_audio_signal_get_data(size_t offset, size_t length, float* out_ptr);
static void microphone_inference_end(void);

void mainChat(void* arg);
String getAccessToken(const char* api_key, const char* secret_key);
String baiduSTT_Send(String access_token, uint8_t* audioData, int audioDataSize);
String baiduErnieBot_Get(String access_token, String prompt);
void baiduTTS_Send(String access_token, String text);
void playAudio(uint8_t* audioData, size_t audioDataSize);
void clearAudio(void);
void playAudio_Zai(void);

#endif